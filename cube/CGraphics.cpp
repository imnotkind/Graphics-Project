#include "CGraphics.h"

void smoothMove(struct KeyboardValue& v, double t, double speed, double scale);
bool doubleSame(double a, double b);


CGraphics::CGraphics()
{
	Mouse_Status.mouse_moving = false;
	Mouse_Status.start_time = 0;
	Mouse_Status.stop_time = 0;

	for (int i = 0; i < 6; i++) {
		switch (i) {
		case 0: Key_Status[i].key = 'a'; break;
		case 1: Key_Status[i].key = 'd'; break;
		case 2: Key_Status[i].key = 's'; break;
		case 3: Key_Status[i].key = 'w'; break;
		case 4: Key_Status[i].key = ' '; break;
		case 5: Key_Status[i].key = GLUT_KEY_SHIFT_L; break;
		default: break;
		}
		Key_Status[i].pressed = false;
		Key_Status[i].moving = false;
		Key_Status[i].speed = 0;
	}
}

CGraphics::~CGraphics()
{
}

bool CGraphics::M_MoveRequest(Vec3d d)
{
	glm::mat4 trans = glm::inverse(V_Fractals["basic"]->M_Trace(V_Trace));

	auto np = V_Camera_Pos + d;
	glm::vec4 p = glm::vec4(np[0], np[1], np[2], 1);

	p = trans * p;

	V_Camera_Pos += d;

	T4Int C(0, 0, 0, 255);

	if (abs(p[0]) < V_FD && abs(p[1]) < V_FD 
		&& p[2] > 0 && p[2] < (V_FH + V_FD * 2) )
	{
		C[0] = 255;
		V_Camera_Pos = np;
		return true;
	}
	else
	{
		if (p[0] >= V_FD) V_Trace.push_back(0);
		else if (p[0] <= -V_FD) V_Trace.push_back(2);
		else if (p[1] >= V_FD) V_Trace.push_back(1);
		else if (p[1] <= -V_FD) V_Trace.push_back(3);
		else if (p[2] >= V_FH + V_FD * 2) V_Trace.push_back(4);
		else if (p[2] <= 0) V_Trace.pop_back();

		C[2] = 255;
		V_Count++;

		V_SpeedScale = 1.0 * pow(V_FRatio, V_Trace.size());
		return false;
	}
		

}


void CGraphics::M_RenderUI(void)
{

	for(int i = 0; i < V_Trace.size(); i++)
		M_DrawNumber(Vec3d(100, 100 + 35 * i, 0), 15, V_Trace[i], T4Int(255, 255, 255, 255));


}
void CGraphics::M_MoveCamera(void)
{
	static double t = 0.0;
	t += 0.00461;
	
	T2Double mouse = V_UserInput->M_MouseGet_Normalized();

	double d = 0;

	if (Mouse_Status.mouse_moving)
		d = 1 - exp(log(0.5) * 5 * (Mouse_Status.stop_time - Mouse_Status.start_time));
	else d = 1;


//	cout << "d: " << d << endl;

	mouse[0] = Mouse_Status.prev_x +  (Mouse_Status.prev_x - mouse[0]) * d;
	mouse[1] = Mouse_Status.prev_y +  (Mouse_Status.prev_y - mouse[1]) * d;

	mouse[1] = clamp(mouse[1], DTR(-80), DTR(80));

	double th = mouse[0]; double pi = mouse[1] == 0 ? 0 : mouse[1] / abs(mouse[1]) * std::min(abs(mouse[1]), DTR(80.0));

	if (Mouse_Status.prev_x == mouse[0] && Mouse_Status.prev_y == mouse[1]) {
		if (Mouse_Status.mouse_moving) {
			Mouse_Status.stop_time = t;
		}
		Mouse_Status.mouse_moving = false;

	}
	else {
		if (!Mouse_Status.mouse_moving) {
			Mouse_Status.start_time = t;
		}

		Mouse_Status.mouse_moving = true;
	}

	V_Camera_Look = glm::vec3(cos(-th) * cos(pi), sin(-th) *cos(pi), sin(-pi));
//	cout << "cameraLook  x: " << V_Camera_Look.x << "  y: " << V_Camera_Look.y << "  z: " << V_Camera_Look.z << endl;

	Vec3d hor = glm::vec3(-sin(-th) *cos(pi), cos(-th) * cos(pi), 0);
	Vec3d ver = glm::vec3(0, 0, 1);

	double speed = V_SpeedScale * 0.005;

	//pressed
	if (V_UserInput->M_IfPressed('a', false)) {
		Key_Status[0].pressed = true;
		Key_Status[0].moving = true;
		
		if (!M_MoveRequest(hor * (float)speed)) {
			Key_Status[0].moving = false;
		}
	} 
	if (V_UserInput->M_IfPressed('d', false)) {
		Key_Status[1].pressed = true;
		Key_Status[1].moving = true;

		if (!M_MoveRequest(-hor * (float)speed)) {
			Key_Status[1].moving = false;
		}
	} 
	if (V_UserInput->M_IfPressed('s', false)) {
		Key_Status[2].pressed = true;
		Key_Status[2].moving = true;

		if (!M_MoveRequest(-V_Camera_Look * (float)speed)) {
			Key_Status[2].moving = false;
		}
	} 
	if (V_UserInput->M_IfPressed('w', false)) {
		Key_Status[3].pressed = true;
		Key_Status[3].moving = true;

		if (!M_MoveRequest(V_Camera_Look * (float)speed)) {
			Key_Status[3].moving = false;
		}
	}
	if (V_UserInput->M_IfPressed(' ', false)) {
		Key_Status[4].pressed = true;
		Key_Status[4].moving = true;

		if (!M_MoveRequest(ver * (float)speed)) {
			Key_Status[4].moving = false;
		}
	}
	if (V_UserInput->M_IfPressed(GLUT_KEY_SHIFT_L, true)) {
		Key_Status[5].pressed = true;
		Key_Status[5].moving = true;

		if (!M_MoveRequest(-ver * (float)speed)) {
			Key_Status[5].moving = false;
		}
	}

	//stopped
	if (!V_UserInput->M_IfPressed('a', false)) {
		smoothMove(Key_Status[0], t, speed, V_SpeedScale);
		if (!M_MoveRequest(hor * (float)Key_Status[0].speed)) {
			Key_Status[0].moving = false;
			Key_Status[0].speed = 0;
		}
	}
	if (!V_UserInput->M_IfPressed('d', false)) {
		smoothMove(Key_Status[1], t, speed, V_SpeedScale);
		if(!M_MoveRequest(-hor * (float)Key_Status[1].speed)) {
			Key_Status[1].moving = false;
			Key_Status[1].speed = 0;
		}
	}
	if (!V_UserInput->M_IfPressed('s', false)) {
		smoothMove(Key_Status[2], t, speed, V_SpeedScale);
		if(!M_MoveRequest(-V_Camera_Look * (float)Key_Status[2].speed)) {
			Key_Status[2].moving = false;
			Key_Status[2].speed = 0;
		}
	}
	if (!V_UserInput->M_IfPressed('w', false)) {
		smoothMove(Key_Status[3], t, speed, V_SpeedScale);
		if(!M_MoveRequest(V_Camera_Look * (float)Key_Status[3].speed)) {
			Key_Status[3].moving = false;
			Key_Status[3].speed = 0;
		}
	}
	if (!V_UserInput->M_IfPressed(' ', false)) {
		smoothMove(Key_Status[4], t, speed, V_SpeedScale);
		if(!M_MoveRequest(ver * (float)Key_Status[4].speed)) {
			Key_Status[4].moving = false;
			Key_Status[4].speed = 0;
		}
	}
	if (!V_UserInput->M_IfPressed(GLUT_KEY_SHIFT_L, true)) {
		smoothMove(Key_Status[5], t, speed, V_SpeedScale);
		if (!M_MoveRequest(-ver * (float)Key_Status[5].speed)) {
			Key_Status[5].moving = false;
			Key_Status[5].speed = 0;
		}
	}


	Mouse_Status.prev_x = mouse[0];
	Mouse_Status.prev_y = mouse[1];

	V_Camera_Look += V_Camera_Pos;

	return;

	V_Camera_Look = vec3(0, 0, 2);
	V_Camera_Pos = vec3(cos(t) * 6, sin(t) * 6,4);

	return;


}

void CGraphics::M_RenderFractal(void)
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, V_SM->V_Textures["pallete"]->textureID);
	auto k = glGetUniformLocation(V_SM->M_GetProgram(), "pallete");
	glUniform1i(k, 0);

	for (int i = -50; i <= 50; i++)
	{
		float x = i * 0.1;
		M_DrawLine(Vec3d(x, -5.0, 0), Vec3d(x, 5.0, 0), T4Int(8, 255, 255, 255));
		M_DrawLine(Vec3d(-5.0, x, 0), Vec3d(5.0, x, 0), T4Int(8, 255, 255, 255));
	}

	

	static double t = 0.0;
	t += 0.01;

	V_Lights[0].pos = Vec4d(5 * cos(t), 3 * sin(t), 5.0, 1.0);
	V_Lights[0].dif = Vec4d(0.2, 0.2, 0.2, 1);
	V_Lights[0].spc = Vec4d(0.2, 0.2, 0.2, 1);

	V_Lights[1].pos = Vec4d(4.4 * cos(t*0.9), 0, 3.8 * sin(t*0.9), 1.0);
	V_Lights[1].dif = Vec4d(0.03, 0.03, 0.03, 1);
	V_Lights[1].spc = Vec4d(0.03, 0.03, 0.03, 1);

	V_Lights[2].pos = Vec4d(cos(t*0.3), sin(t*0.3), 1.0, 0.0);
	V_Lights[2].dif = Vec4d(0.2, 0.2, 0.2, 1);
	V_Lights[2].spc = Vec4d(0.2, 0.2, 0.2, 1);

	V_KeepLight = false;
	//M_DrawModel(Vec3d(-50, -50, 50), "star", 0.001, 1, T4Int(255, 255, 255, 255));
	

	for (auto l : V_Lights)
	{
		M_DrawModel(l.pos, "sphere", 0.1, 0.0, T4Int(220, 220, 20, 255));
	}
	V_KeepLight = false;


	T4Int rgba(255, 255, 0, 255);

	SRenderInfo ri;

	double rotate = 0.0;
	glm::vec3 p(0.0);
	double scale = 1.0;

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, p);
	m = glm::rotate(m, float(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(scale, scale, scale));
	
	ri.view = V_CTM_View;
	ri.model = m;
	ri.projection = V_CTM_Project;

	ri.keeplight = false;
	ri.normtrans = V_CTM_View * m;

	ri.amb = Vec4d(0.1, 0.1, 0.1, 1.0);

	auto vt = V_CTM_View;
	ri.lights.resize(V_Lights.size());
	transform(V_Lights.begin(), V_Lights.end(), ri.lights.begin(),
		[vt](SLight x)->SLight {x.pos = vt * x.pos; return x; });

	V_SM->M_UseProgram("prg4");
	auto l = V_SM->M_GetUniformLoc("t");
	glUniform1f(l, (float)t);

	for (int i = 0; i < 4; i++) ri.color[i] = rgba[i] / 255.0;

	V_DistortScale = sqrt(pow(V_DistortScale, 1.999)  * pow(V_SpeedScale, 0.001));
	l = V_SM->M_GetUniformLoc("fractal1");
	glUniform1f(l, V_DistortScale);
	
	V_Fractals["basic"]->M_Draw(ri, V_Trace, V_Trace.size(), V_Trace.size() + 2);

}
void CGraphics::M_CallbackDisplay()
{
	M_ListenMessages();

	V_CTM_Temp = glm::mat4(1.0);

	static double count = 0;
	count += 0.005;
	
	

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

	if (V_ViewMode)
	{
		M_MoveCamera();
		V_CTM_View = glm::lookAt(V_Camera_Pos, V_Camera_Look, up);
	}
	else
	{
		auto look = vec3(0, 0, 2);
		auto pos = vec3(cos(count) * 6, sin(count) * 6, 4);
		V_CTM_View = glm::lookAt(pos, look, up);
	}

	V_CTM_Project = glm::perspective(glm::radians(45.0f), 1.0f, 0.05f * float(V_SpeedScale), 1000.0f);
	

	//world
	M_RenderFractal();

	glutSwapBuffers();
}
int CGraphics::M_Initialize(CEngine * P)
{

	V_PEngine = P;
	V_Screen_Size = T2Double(1080, 1080);
	V_UserInput = CUserInput::getInstance();

	V_UserInput->M_SetWinSize(V_Screen_Size);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1080, 1080);

	int id = glutCreateWindow("Graphics Project");

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

	V_ViewMode = true;
	V_CurrentDrawing = false;

	//V_Camera_Pos = Vec3d(-3, -3, 2.5);
	V_UserInput->M_MouseSet(T2Int(0, 0));

	

	V_FDepth = 0;
	V_KeepLight = false;
	V_Lights.resize(3);

	V_DistortScale = 1.0;
	V_FRatio = 35.0 / 50.0;
	V_FH = 40;
	V_FD = 82.5;

	M_SetupHieraModels();

	V_Camera_Pos[2] = 0.1;
	V_Count = 0;
	V_SpeedScale = 1;

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

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, glm::vec3(p1));
	m = glm::scale(m, glm::vec3(p2 - p1));

	r.view = V_CTM_View;
	r.model = m;
	r.projection = V_CTM_Project;
	
	for (int i = 0; i < 4; i++) r.color[i] = rgba[i] / 255.0;
	V_Models["line"]->M_Draw(r);
}

void CGraphics::M_DrawModel(Vec3d p, string name, double r, double rotate, T4Int rgba)
{
	SRenderInfo ri;

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, glm::vec3(p));
	m = glm::rotate(m, float(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(r, r, r));
	m = m * V_CTM_Temp; //billboard

	ri.view = V_CTM_View;
	ri.model = m;
	ri.projection = V_CTM_Project;

	ri.keeplight = V_KeepLight;
	if (!V_KeepLight) V_KeepLight = true;
	ri.amb = Vec4d(0.0, 0.0, 0.0, 1.0);

	auto vt = V_CTM_View;
	ri.lights.resize(V_Lights.size());
	transform(V_Lights.begin(), V_Lights.end(), ri.lights.begin(),
		[vt](SLight x)->SLight {x.pos = vt * x.pos; return x; });

	ri.normtrans = V_CTM_View * m;

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

void smoothMove(struct KeyboardValue& v, double t, double speed, double scale) {
	if (v.pressed) {
		v.start = t;
		v.speed = speed;
	}

	v.pressed = false;

	if (v.moving) {
		double accel = scale * 0.015 * (t - v.start);

		if (accel < speed) {
				v.speed = speed - accel;
		}
		else if (accel >= speed) {
			v.moving = false;
			v.speed = 0;
		}
	}
}


bool doubleSame(double a, double b) {
	double offset = 0.000001;

	if (abs(a - b) < offset)
		return true;
	else
		return false;
}