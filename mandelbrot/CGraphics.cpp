#include "CGraphics.h"



CGraphics::CGraphics()
{
}

CGraphics::~CGraphics()
{
}

void CGraphics::M_RenderWorld(void)
{
	static double anim = 0.0;
	anim += 0.05;

}

void CGraphics::M_RenderUI(void)
{

}


int CGraphics::M_Initialize(void)
{

	V_Screen_Size = T2Double(1080, 1080);
	auto ui = CUserInput::getInstance();
	ui->M_SetWinSize(V_Screen_Size);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1080, 1080);

	int id = glutCreateWindow("Graphics Assn4");
	cout << id << endl;

	glClearColor(1, 1, 1, 1); //background white
	glClearColor(0.8f, 0.8f, 0.8f, 0.5f);
	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	return id;
}

void CGraphics::M_Initialize2(void)
{
	glEnable(GL_DEPTH_TEST);
	V_SM = CShaderManager::getInstance();

	V_ViewMode = false;
	V_CurrentDrawing = false;
	M_SetupHieraModels();

	V_Camera_Pos = Vec3d(0, 0, 2.5);
	V_Camera_Look = Vec3d(0, 0, 0);

	V_Height = 1;
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
		V_ViewMode = !V_ViewMode;
		return true;
	}
	if (key == 'r' && special == false)
	{
		V_Height = 1;
		return true;
	}
	if (key == 'w' && special == false)
	{
		V_Camera_Pos[1] += 0.1* atan(V_Camera_Pos[2]);
		return true;
	}
	if (key == 's' && special == false)
	{
		V_Camera_Pos[1] -= 0.1* atan(V_Camera_Pos[2]);
		return true;
	}
	if (key == 'a' && special == false)
	{
		V_Camera_Pos[0] -= 0.1* atan(V_Camera_Pos[2]);
		return true;
	}
	if (key == 'd' && special == false)
	{
		V_Camera_Pos[0] += 0.1* atan(V_Camera_Pos[2]);
		return true;
	}
	if (key == 'o' && special == false)
	{
		V_Height -= 0.3;
		return true;
	}
	if (key == 'p' && special == false)
	{
		V_Height += 0.3;
		return true;
	}
	if (key == 't' && special == false)
	{
		cout << "=========================" << endl;
		cout << "(" << V_Camera_Pos[0] << ", " << V_Camera_Pos[1] << ", " << V_Camera_Pos[2] << ") : " << V_Height << endl;
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
void CGraphics::M_MoveCamera(void)
{
	return;

}
void CGraphics::M_CallbackDisplay()
{
	M_ListenMessages();

	V_CTM_Temp = glm::mat4(1.0);

	


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	V_SM->M_UseProgram("prg1");
	
	V_CTM_Project = glm::mat4(1.0f);
	V_CTM_Project = glm::translate(V_CTM_Project, glm::vec3(-1.0, -1.0, 0.0));
	V_CTM_Project = glm::scale(V_CTM_Project, glm::vec3(2.0 / V_Screen_Size[0], 2.0 / V_Screen_Size[1], 1));
	V_CTM_View = glm::mat4(1.0f);
	M_RenderUI();

	V_CTM_Project = glm::mat4(1.0f);
	V_CTM_View = glm::mat4(1.0f);
	M_RenderFractal();



	glm::vec3 up(0, 0, 1);
	M_MoveCamera();
	V_CTM_Project = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
	V_CTM_View = glm::lookAt(V_Camera_Pos, V_Camera_Look, up);
	M_RenderWorld();


	glutSwapBuffers();
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

void coutmat(glm::mat4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}
void CGraphics::M_RenderFractal(void)
{

	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, V_SM->V_Textures["pallete"]->textureID);
	auto p = glGetUniformLocation(V_SM->M_GetProgram(), "pallete");
	glUniform1i(p, 0);

	auto ui = CUserInput::getInstance();
	

	V_Height -= 0.005;
	V_Camera_Pos[0] = -0.989733;
	V_Camera_Pos[1] = 1.79135e-06;
	V_Camera_Pos[2] = exp(V_Height);
	if (V_Height < 0.9)
	{
		//V_Camera_Pos[0] += 0.01*(ui->M_MouseGet_Normalized()[0] - 0.5) * atan(V_Camera_Pos[2]);
		//V_Camera_Pos[1] -= 0.01*(ui->M_MouseGet_Normalized()[1] - 0.5) * atan(V_Camera_Pos[2]);
	}
	
	

	Vec3d realcamera = V_Camera_Pos;

	glm::vec3 up(0, 1, 0);
	V_CTM_Project = glm::perspective(glm::radians(45.0f), 1.0f, float(V_Camera_Pos[2]*0.5), 1000.0f);
	V_CTM_View = glm::lookAt(realcamera, realcamera - Vec3d(0, 0, 10), up);

	M_Mandelbrot();
}
void CGraphics::M_Mandelbrot()
{
	static double t = 0.0;
	t += 0.001;

	GLuint p = glGetUniformLocation(V_SM->M_GetProgram(), "scale");
	GLuint q = glGetUniformLocation(V_SM->M_GetProgram(), "t");
	glUniform1f(p, 1); 
	glUniform1f(q, t); //this shouldn't be called before program use
	M_DrawModel(Vec3d(0.0, 0.0, 0.0), "square", 1, 0, T4Int(255, 255, 255, 255));
}



void CGraphics::M_DrawLine(Vec3d p1, Vec3d p2, T4Int rgba)
{
	glm::mat4 m;
	m = V_CTM_Project * V_CTM_View;
	m = glm::translate(m, glm::vec3(p1));
	m = glm::scale(m, glm::vec3(p2 - p1));
	T4Double c;
	for (int i = 0; i < 4; i++) c[i] = rgba[i] / 255.0;
	V_Models["line"]->M_Draw(m, c);
}

void CGraphics::M_DrawModel(Vec3d p, string name, double r, double rotate, T4Int rgba)
{

	glm::mat4 m;
	m = V_CTM_Project * V_CTM_View;
	m = glm::translate(m, glm::vec3(p));

	m = m * V_CTM_Temp; //billboard

	m = glm::rotate(m, float(rotate), glm::vec3(0.0f,0.0f,1.0f));
	m = glm::scale(m, glm::vec3(r, r, r));

	T4Double c;
	for (int i = 0; i < 4; i++) c[i] = rgba[i] / 255.0;
	V_Models[name]->M_Draw(m, c);
}
void CGraphics::M_DrawFont(Vec3d p, Vec2d size, string str, T4Int rgba)
{
}

