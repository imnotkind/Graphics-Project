#include "CGraphics.h"



CGraphics::CGraphics()
{
}

CGraphics::~CGraphics()
{
}


void CGraphics::M_RenderUI(void)
{

	


}
void CGraphics::M_MoveCamera(void)
{
	static double t = 0.0;
	t += 0.00461;
	
	T2Double mouse = V_UserInput->M_MouseGet_Normalized();
	mouse[1] = clamp(mouse[1], DTR(-80), DTR(80));

	double th = mouse[0]; double pi = mouse[1] == 0 ? 0 : mouse[1] / abs(mouse[1]) * std::min(abs(mouse[1]), DTR(80.0));

	V_Camera_Look = glm::vec3(cos(-th) * cos(pi), sin(-th) *cos(pi), sin(-pi));

	Vec3d hor = glm::vec3(-sin(-th) *cos(pi), cos(-th) * cos(pi), 0);
	
	double speed = 0.03;
	if (V_UserInput->M_IfPressed('a', false)) V_Camera_Pos += hor *(float)speed;
	if (V_UserInput->M_IfPressed('d', false)) V_Camera_Pos -= hor  * (float)speed;
	if (V_UserInput->M_IfPressed('s', false)) V_Camera_Pos -= V_Camera_Look * (float)speed;
	if (V_UserInput->M_IfPressed('w', false)) V_Camera_Pos += V_Camera_Look * (float)speed;


	V_Camera_Look += V_Camera_Pos;

	return;

	V_Camera_Look = vec3(0, 0, 2);
	V_Camera_Pos = vec3(cos(t) * 6, sin(t) * 6,4);

	return;


}

void CGraphics::M_RenderFractal(void)
{
	for (int i = -50; i <= 50; i++)
	{
		float x = i * 0.1;
		M_DrawLine(Vec3d(x, -5.0, 0), Vec3d(x, 5.0, 0), T4Int(8, 255, 255, 255));
		M_DrawLine(Vec3d(-5.0, x, 0), Vec3d(5.0, x, 0), T4Int(8, 255, 255, 255));

	}


	static double t = 0.0;
	t += 0.01;
	V_Light1[0] = 5 * cos(t);
	V_Light1[1] = 5 * sin(t);
	V_Light1[2] = 5;

	T4Int rgba(255, 255, 0, 255);

	SRenderInfo ri;

	double rotate = 0.0;
	glm::vec3 p(0.0);
	double scale = 1.0;

	glm::mat4 m = V_CTM_View;
	m = glm::translate(m, p);
	m = glm::rotate(m, float(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(scale, scale, scale));

	ri.modelview = m;
	ri.projection = V_CTM_Project;

	ri.keeplight = false;
	ri.amb = Vec4d(0.1, 0.1, 0.1, 1.0);
	ri.dif = Vec4d(0.3, 0.3, 0.3, 1.0);
	ri.spc = Vec4d(0.3, 0.3, 0.3, 1.0);
	ri.light1 = V_CTM_View * Vec4d(V_Light1[0], V_Light1[1], V_Light1[2], 1);
	ri.normtrans = m;

	V_SM->M_UseProgram("prg4");
	auto l = V_SM->M_GetUniformLoc("t");
	glUniform1f(l, (float)t);

	int s = int(3 * sin(t*2) + 3 + 0.5);
	V_FDepth = s;
	vector<int> trace;
	for (int i = 0; i < s; i++) trace.push_back(i);

	for (int i = 0; i < 4; i++) ri.color[i] = rgba[i] / 255.0;
	
	float test = pow(2.0, s);



	V_Fractals["basic"]->M_Draw(ri, 2);
	return;

	auto tt = V_Fractals["basic"]->M_Trace(trace);
	auto tv = glm::vec3(tt[3][0], tt[3][1], tt[3][2]);
	//ri.modelview = ri.modelview * glm::translate(glm::mat4(1.0), -tv);
	V_Fractals["basic"]->M_Draw(ri, trace, s, s + 3);
	

	

	

	
	//M_DrawModel(Vec3d(0, 0, 0), "sphere", 0.1, 0, T4Int(255, 0, 255, 255));
}
void CGraphics::M_CallbackDisplay()
{
	M_ListenMessages();

	V_CTM_Temp = glm::mat4(1.0);

	static double count = 0;
	count += 0.02;
	M_MoveCamera();

	V_CurrentDrawing = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	V_SM->M_UseProgram("prg1");

	V_CTM_Project = glm::mat4(1.0f);
	V_CTM_Project = glm::translate(V_CTM_Project, glm::vec3(-1.0, -1.0, 0.0));
	V_CTM_Project = glm::scale(V_CTM_Project, glm::vec3(2.0 / V_Screen_Size[0], 2.0 / V_Screen_Size[1], 1));
	V_CTM_View = glm::mat4(1.0f);
	M_RenderUI();


	V_CurrentDrawing = true;

	glm::vec3 v(0.0f);
	glm::vec3 up(0.0, 0.0, 1.0);

	V_CTM_Project = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
	V_CTM_View = glm::lookAt(V_Camera_Pos, V_Camera_Look, up);

	//world
	M_RenderFractal();

	glutSwapBuffers();
}
int CGraphics::M_Initialize(CEngine * P)
{
	CTexture t;
	t.M_LoadTexture("resource/OBJ files/wall/normal.bmp");

	V_PEngine = P;
	V_Screen_Size = T2Double(1080, 1080);
	V_UserInput = CUserInput::getInstance();

	V_UserInput->M_SetWinSize(V_Screen_Size);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1080, 1080);

	int id = glutCreateWindow("Graphics Assn3");

	glClearColor(1, 1, 1, 1); //background white
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glutSetCursor(GLUT_CURSOR_NONE);


	return id;

}

void CGraphics::M_Initialize2(void)
{
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//glEnable(GL_CULL_FACE);

	V_SM = CShaderManager::getInstance();

	V_ViewMode = false;
	V_CurrentDrawing = false;

	//V_Camera_Pos = Vec3d(-3, -3, 2.5);
	V_UserInput->M_MouseSet(T2Int(0, 0));

	M_SetupHieraModels();

	V_FDepth = 0;

}

void CGraphics::M_ListenMessages(void)
{
	auto iq = SIQueue::M_GetSingletone(0);
	auto rq = SIQueue::M_GetSingletone(1);
	while (!iq->M_Empty())
	{
		auto m = iq->M_Pop();
		if (m.type == "down")
		{
			if (!M_Event_KeyPress(m.key, m.special)) rq->M_Push(m); //not mine!
		}
	}
	while (!rq->M_Empty())
	{
		iq->M_Push(rq->M_Pop());
	}
}

bool CGraphics::M_Event_KeyPress(int key, bool special)
{
	if (key == 'v' && special == false)
	{
		V_UserInput->V_MousePrison = !V_UserInput->V_MousePrison;

		V_ViewMode = !V_ViewMode;
		return true;
	}
	return false;
}

glm::mat4 CGraphics::M_GetBillboardMat(void)
{
	auto view = V_CTM_View;
	view = glm::transpose(view);
	for (int i = 0; i < 3; i++)
		view[i][3] = 0;
	view = glm::transpose(view); //inverse

	return glm::transpose(view);
}



void CGraphics::M_CallbackReshape(int w, int h)
{
	V_Screen_Size[0] = w;
	V_Screen_Size[1] = h;
	glViewport(0, 0, w, h);
}

void CGraphics::M_CallbackIdle()
{
	
	glutPostRedisplay();
}

void CGraphics::M_DrawLine(Vec3d p1, Vec3d p2, T4Int rgba)
{
	SRenderInfo r;

	glm::mat4 m = V_CTM_View;
	m = V_CTM_View;
	m = glm::translate(m, glm::vec3(p1));
	m = glm::scale(m, glm::vec3(p2 - p1));

	r.modelview = m;
	r.projection = V_CTM_Project;
	
	for (int i = 0; i < 4; i++) r.color[i] = rgba[i] / 255.0;
	V_Models["line"]->M_Draw(r);
}

void CGraphics::M_DrawModel(Vec3d p, string name, double r, double rotate, T4Int rgba)
{
	SRenderInfo ri;

	glm::mat4 m = V_CTM_View;
	m = glm::translate(m, glm::vec3(p));
	m = glm::rotate(m, float(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(r, r, r));
	m = m * V_CTM_Temp; //billboard

	ri.modelview = m;
	ri.projection = V_CTM_Project;

	ri.keeplight = false;
	ri.amb = Vec4d(0.1, 0.1, 0.1, 1.0);
	ri.dif = Vec4d(0.5, 0.5, 0.5, 1.0);
	ri.spc = Vec4d(0.0, 0.0, 0.0, 1.0);
	ri.light1 = V_CTM_View * Vec4d(V_Light1[0], V_Light1[1], V_Light1[2], 1);
	ri.normtrans = m;
	

	for (int i = 0; i < 4; i++) ri.color[i] = rgba[i] / 255.0;
	V_Models[name]->M_Draw(ri);
}
void CGraphics::M_DrawFont(Vec2d p, string str, T4Int rgba)
{
}

void CGraphics::M_DrawFontBig(Vec2d p, string str, double scale, T4Int rgba)
{
}

void CGraphics::M_DrawNumber(Vec3d p, double r, int num, T4Int rgba)
{
	string str = to_string(num);
	Vec3d i = Vec3d(0, 0, 0);
	for (auto c : str)
	{
		int k = c - '0';

		if (k == 0 || k == 2 || k == 3 || k == 5 || k == 6 || k == 7 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "A", r, 0.0, rgba);
		}

		if (k == 0 || k == 1 || k == 2 || k == 3 || k == 4 || k == 7 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "B", r, 0.0, rgba);
		}

		if (k == 0 || k == 1 || k == 3 || k == 4 || k == 5 || k == 6 || k == 7 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "C", r, 0.0, rgba);
		}

		if (k == 0 || k == 2 || k == 3 || k == 5 || k == 6 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "D", r, 0.0, rgba);
		}

		if (k == 0 || k == 2 || k == 6 || k == 8)
		{
			M_DrawModel(p+i, "E", r, 0.0, rgba);
		}

		if (k == 0 || k == 4 || k == 5 || k == 6 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "F", r, 0.0, rgba);
		}
		if (k == 2 || k == 3 || k == 4 || k == 5 || k == 6 || k == 8 || k == 9)
		{
			M_DrawModel(p+i, "G", r, 0.0, rgba);
		}

		i += Vec3d(r*1.5, 0, 0);
	}
}