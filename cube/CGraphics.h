#pragma once
#include "MainHeader.h"
#include "CGraphics.h"
#include "CEngine.h"
#include "CHandler.h"
#include "CShaderManager.h"
#include "CTexture.h"
#include "CMesh.h"
#include "CHierModel.h"

#include "CFractal.h"

#include <ctime>

struct KeyboardValue {
	char key;
	bool pressed;
	bool moving;
	double start;
	double speed;
};

struct MouseValue {
	bool mouse_moving;
	double prev_x;
	double prev_y;
	double start_time;
	double stop_time;
};

class CGraphics : public CHandler
{
	CEngine * V_PEngine;
	CShaderManager* V_SM;
	CUserInput* V_UserInput;

	T2Double V_Screen_Size;

	struct KeyboardValue Key_Status[6];
	struct MouseValue Mouse_Status;

	Vec3d V_Camera_Pos;
	Vec3d V_Camera_Look;
	Vec2d V_Camera_Look_Angle;

	Vec3d V_Camera_Speed;

	vector<SLight> V_Lights;
	bool V_KeepLight;


	int V_FDepth;


	double V_FRatio;
	double V_FD;
	double V_FH;

	int V_Count;

	vector<int> V_Trace;
	double V_SpeedScale;
	double V_DistortScale;
	bool M_MoveRequest(Vec3d d);



	glm::mat4 V_CTM_View;
	glm::mat4 V_CTM_Project;
	glm::mat4 V_CTM_Temp;
	
	bool V_CurrentDrawing;
	bool V_ViewMode; // false : 1p, true: 3p
	double fps = 0;

	LARGE_INTEGER old_count;
	LARGE_INTEGER new_count;
	LARGE_INTEGER freq;

	void M_RenderUI(void);
	void M_MoveCamera(void);
	void M_RenderFractal(void);

	glm::mat4 M_GetBillboardMat(void);

	void M_DrawLine(Vec3d p1, Vec3d p2, T4Int rgba);
	void M_DrawModel(Vec3d p, string name, double r, double rotate, T4Int rgba); //lying on xy
	void M_DrawFont(Vec2d p, string str, T4Int rgba);
	void M_DrawFontBig(Vec2d p, string str, double scale, T4Int rgba);
	void M_DrawNumber(Vec3d p, double r, int num, T4Int rgba);

	void M_ListenMessages(void);
	bool M_Event_KeyPress(int key, bool special);



	void M_SetupHieraModels(void);
	void M_SimplePolyFractal(void);

	map<string, shared_ptr<CHierModel>> V_Models;

	map<string, shared_ptr<CFractal>> V_Fractals;

public:

	void M_CallbackDisplay();
	void M_CallbackReshape(int w, int h);
	void M_CallbackIdle();

	int M_Initialize(CEngine * P);
	void M_Initialize2(void);

	CGraphics();
	~CGraphics();

	

	
};

