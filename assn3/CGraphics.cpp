#include "CGraphics.h"



CGraphics::CGraphics()
{
}

CGraphics::~CGraphics()
{
}

void CGraphics::M_RenderGame(void)
{
	static double anim = 0.0;
	anim += 0.05;


	auto am1 = glm::rotate(glm::mat4(1.0), (float)(cos(anim) * 0.2 * PI), glm::vec3(0.0, 0.0, 1.0));
	auto am2 = glm::rotate(glm::mat4(1.0), (float)(sin(anim) * 0.2 * PI), glm::vec3(0.0, 0.0, 1.0));
	auto am3 = glm::rotate(glm::mat4(1.0), (float)(sin(anim * 1.4) * 0.3 * PI), glm::vec3(0.0, 0.0, 1.0));
	auto am4 = glm::rotate(glm::mat4(1.0), (float)(sin(anim * 1.4) * 0.3 * PI), glm::vec3(0.0, 0.0, 1.0));

	V_Models["enemy"]->M_ClearTrans2();
	V_Models["enemy"]->M_RegisterTrans2(0, am1);
	V_Models["enemy"]->M_RegisterTrans2(1, am2);
	V_Models["enemy"]->M_RegisterTrans2(2, am3);


	//render objects
	for (auto x : V_PEngine->V_Objects)
	{
		auto d = x->M_GetDrawData();
		
		if (3 <= d.img && d.img <= 7 ) //item
		{
			auto old = V_CTM_Temp;
			V_CTM_Temp = M_GetBillboardMat();
			M_DrawItem(d.pos.convert_gl(), d.size, d.img - 3);
			V_CTM_Temp = old;
		}
		else if(d.img == 1)
		{

			M_DrawModel(d.pos.convert_gl() + glm::vec3(0.0, 0.0, 1.5), "enemy", d.size * 0.04,  d.rotate +DTR(90), d.color);
		}
		else if(d.img == 2)//bullet
		{
			M_DrawModel(d.pos.convert_gl() + glm::vec3(0.0, 0.0, 1.0), "sphere", d.size, d.rotate, d.color);
		}
		else if (d.img == 8) //skeleton
		{
			M_DrawModel(d.pos.convert_gl() + glm::vec3(0.0, 0.0, 2.5), "skeleton", d.size * 0.01, d.rotate + DTR(90), d.color);
		}
		
	}

	//render player
	auto d = V_PEngine->V_Player->M_GetDrawData();
	am1 = glm::rotate(glm::mat4(1.0), (float)glm::radians(70.0), glm::vec3(0.0, 0.0, 1.0));
	am1 = glm::rotate(am1, (float)(sin(anim) * 0.2 * PI), glm::vec3(1.0, 0.0, 0.0));
	am2 = glm::rotate(glm::mat4(1.0), (float)glm::radians(-70.0), glm::vec3(0.0, 0.0, 1.0));
	am2 = glm::rotate(am2, (float)(-sin(anim) * 0.2 * PI), glm::vec3(1.0, 0.0, 0.0));

	am3 = glm::rotate(glm::mat4(1.0), (float)(-sin(anim) * 0.2 * PI), glm::vec3(1.0, 0.0, 0.0));
	am4 = glm::rotate(glm::mat4(1.0), (float)(sin(anim) * 0.2 * PI), glm::vec3(1.0, 0.0, 0.0));

	if (V_PEngine->V_Animation_Temp > 0)
	{
		double t = -(V_PEngine->V_Animation_Temp - 30) / 30.0;
		t = sin(log(t * 20 + 1));
		am1 = glm::rotate(glm::mat4(1.0), (float)glm::radians(-100.0 * t), glm::vec3(1.0, 0.0, 0.0));
		am2 = glm::rotate(glm::mat4(1.0), (float)glm::radians(-100.0 * t), glm::vec3(1.0, 0.0, 0.0));
	}

	V_Models["man"]->M_ClearTrans2();
	V_Models["man"]->M_RegisterTrans2(2, am1);
	V_Models["man"]->M_RegisterTrans2(5, am2);
	V_Models["man"]->M_RegisterTrans2(9, am3);
	V_Models["man"]->M_RegisterTrans2(12, am4);
	
	
	M_DrawModel(d.pos.convert_gl(), "man", d.size * 0.02, d.rotate + DTR(90), d.color);

	//render map
	double gsize = V_PEngine->V_Grid_Size;
	auto s = V_PEngine->V_Map.size;
	for (int i = 0; i < s[0]; i++)
	{
		for (int j = 0; j < s[1]; j++)
		{
			int g = 10 * sin(i*j);
			int r = 20 * cos(i*j);
			if (V_PEngine->V_Map[T2Int(i, j)] == 1)
			{
				T2Double cen = T2Double(i, j)*gsize;
				auto p = cen.convert_gl();

				auto old = V_CTM_Temp;
				V_CTM_Temp = glm::scale(V_CTM_Temp, glm::vec3(1.0, 1.0, 3.0));

				if(V_PEngine->V_IS_Camera)
					M_DrawModel(p, "cube1", gsize / 2, 0, T4Int(125 + r, 30 + g, 255, MC_R(255 - 200 * sin(V_PEngine->V_IS_Camera / 600.0 * PI))));
				else
					M_DrawModel(p, "cube1", gsize / 2, 0, T4Int(125 + r, 30 + g, 255, 255));

				V_CTM_Temp = old;
			}
		}
	}

	auto old = V_CTM_Temp;
	auto midx = gsize * s[0] / 2.0;
	auto midy = gsize * s[1] / 2.0;
	V_CTM_Temp = glm::translate(V_CTM_Temp, glm::vec3(0, 0, -gsize*1.5));
	V_CTM_Temp = glm::scale(V_CTM_Temp, glm::vec3(1.0, 1.0, 0.01));
	
	M_DrawModel(glm::vec3(midx, midy, 0), "cube1", midx, 0, T4Int(150, 150, 100, 255));
	V_CTM_Temp = old;
}

void CGraphics::M_RenderUI(void)
{

	
	if (V_PEngine->V_GameEnd == 1)
	{
		M_DrawNumber(Vec3d(V_Screen_Size[0] / 2 - 150, V_Screen_Size[1] / 2, 0), 100, V_PEngine->V_PEnemies.size(), T4Int(255, 0, 0, 255));
		M_DrawModel(Vec3d(V_Screen_Size[0] / 2, V_Screen_Size[1] / 2, 0), "square", 250, 0, T4Int(130, 100, 100, 255));
	}
	if (V_PEngine->V_GameEnd == 2)
	{
		M_DrawNumber(Vec3d(V_Screen_Size[0] / 2 - 150, V_Screen_Size[1] / 2, 0), 100, V_PEngine->V_LeftTime, T4Int(125, 255, 0, 255));
		M_DrawModel(Vec3d(V_Screen_Size[0] / 2, V_Screen_Size[1] / 2, 0), "square", 250, 0, T4Int(100, 130, 100, 255));
	}
	

	M_DrawNumber(Vec3d(50, 100, 0), 10, V_PEngine->V_PEnemies.size(), T4Int(255,0,0,255));
	M_DrawNumber(Vec3d(50, 150, 0), 10, V_PEngine->V_LeftTime, T4Int(125,255,0,255));
	M_DrawNumber(Vec3d(50, 200, 0), 10, V_PEngine->V_Life, T4Int(255, 255, 0, 255));
	M_DrawModel(Vec3d(50, 150, 0), "square", 80, 0, T4Int(100, 100, 100, 200));
	
	
	auto l = V_PEngine->V_Player->M_GetItemList();

	auto n = std::min(4, (int)l.size());
	auto it = l.begin();

	switch (n)
	{
		case 4:
			M_DrawItem(Vec3d(300, V_Screen_Size[1] - 40, 0), 30, *next(it, 3));
		case 3:
			M_DrawItem(Vec3d(230, V_Screen_Size[1] - 40, 0), 30, *next(it, 2));
		case 2:
			M_DrawItem(Vec3d(160, V_Screen_Size[1] - 40, 0), 30, *next(it, 1));
		case 1:
			M_DrawItem(Vec3d(70, V_Screen_Size[1] - 60, 0), 50,*it);
		case 0:
			break;
	}

	M_DrawModel(Vec3d(70, V_Screen_Size[1] - 60, 0), "square", 50, 0, T4Int(100, 170, 170, 200));
	M_DrawModel(Vec3d(160, V_Screen_Size[1] - 40, 0), "square", 30, 0, T4Int(100, 170, 170, 200));
	M_DrawModel(Vec3d(230, V_Screen_Size[1] - 40, 0), "square", 30, 0, T4Int(100, 170, 170, 200));
	M_DrawModel(Vec3d(300, V_Screen_Size[1] - 40, 0), "square", 30, 0, T4Int(100, 170, 170, 200));



}


int CGraphics::M_Initialize(CEngine * P)
{

	V_PEngine = P;
	V_Screen_Size = T2Double(1080, 1080);
	auto ui = CUserInput::getInstance();
	ui->M_SetWinSize(V_Screen_Size);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1080, 1080);

	int id = glutCreateWindow("Graphics Assn3");
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
	double static olda = 0.0;
	V_Camera_Look_Angle = V_PEngine->V_Player->M_GetLook().convert_gl();
	olda = olda * 0.9 + V_Camera_Look_Angle[0]*0.1;

	
	if (V_ViewMode)
	{
		auto ui = CUserInput::getInstance();
		double d = 1-ui->M_MouseGet_Normalized()[1];
		d *= 2;
		auto p = V_PEngine->V_Player->M_GetPosition();
		V_Camera_Pos[0] = p[0] - cos(olda) * (15 + 15*d);
		V_Camera_Pos[1] = p[1] - sin(olda) * (15 + 15*d);
		V_Camera_Pos[2] = 20;

		V_Camera_Look = V_Camera_Pos;
		V_Camera_Look[0] += (20 + 15*d)*cos(V_Camera_Look_Angle[0]);
		V_Camera_Look[1] += (20 + 15 * d)*sin(V_Camera_Look_Angle[0]);
		V_Camera_Look[2] = 5;
	}
	else
	{
		auto p = V_PEngine->V_Player->M_GetPosition();
		V_Camera_Pos[0] = p[0] + 0.8*cos(V_Camera_Look_Angle[0]);
		V_Camera_Pos[1] = p[1] + 0.8*sin(V_Camera_Look_Angle[0]);
		V_Camera_Pos[2] = 3;

		V_Camera_Look = V_Camera_Pos;
		V_Camera_Look[0] += 5 * cos(V_Camera_Look_Angle[0]);
		V_Camera_Look[1] += 5 * sin(V_Camera_Look_Angle[0]);
		V_Camera_Look[2] += 2 * sin(V_Camera_Look_Angle[1]);
	}


	return;

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
	glm::vec3 up(0, 0, 1);

	V_CTM_Project = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
	V_CTM_View = glm::lookAt(V_Camera_Pos, V_Camera_Look, up);

	M_RenderGame();
	

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
void CGraphics::M_DrawFont(Vec2d p, string str, T4Int rgba)
{
}

void CGraphics::M_DrawFontBig(Vec2d p, string str, double scale, T4Int rgba)
{
}

void CGraphics::M_DrawItem(Vec3d p, double r, int z)
{
	if (z == 0) // Mega fire
	{
		for (int i = 0; i < 10; i++)
		{
			M_DrawModel(p, "diamond", r*0.5, (2 * PI / 10.0)*i, T4Int(255, 255, 0, 255));
		}
		for (int i = 0; i < 10; i++)
		{
			M_DrawModel(p, "diamond", r, (2 * PI / 10.0)*i, T4Int(255, 0, 0, 255));
		}
		
	}
	if (z == 1) // Camera up
	{
		M_DrawModel(p, "circle", r*0.4, 0, T4Int(255, 255, 255, 255));
		//M_DrawModel(p + Vec3d(r*0.7, r*0.5, 0), "circle", r*0.1, 0, T4Int(255, 255, 255, 255)); fix this
		M_DrawModel(p, "rectangle", r*0.9, 0, T4Int(90, 90, 90, 255));

	}
	if (z == 2) // Invincible
	{
		M_DrawModel(p, "star", r*0.8, 0, T4Int(255, 255, 0, 255));
		M_DrawModel(p, "star", r, 0, T4Int(255, 204, 0, 255));
		
	}
	if (z == 3) // Speed up
	{
		for (int i = 0; i < 3; i++)
		{
			M_DrawModel(p+Vec3d(r*0.1,0,0), "diamond", r, PI/12 + (PI/12)*i, T4Int(255, 255, 255, 255));
			M_DrawModel(p-Vec3d(r*0.1,0,0), "diamond", r, PI - (PI / 12 + (PI / 12)*i), T4Int(255, 255, 255, 255));
		}
	}
	if (z == 4) // SuperFire
	{
		for (int i = 0; i < 4; i++)
		{
			M_DrawModel(p, "diamond", r, (2*PI/4.0)*i, T4Int(30, 30, 30, 255));
		}
	}
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