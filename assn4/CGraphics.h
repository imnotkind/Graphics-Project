#pragma once
#include "MainHeader.h"
#include "CGraphics.h"
#include "CEngine.h"
#include "CHandler.h"
#include "CShaderManager.h"
#include "CTexture.h"
#include "CMesh.h"
#include "CHierModel.h"

#include <ctime>

class CGraphics : public CHandler
{
	CEngine * V_PEngine;
	CShaderManager* V_SM;

	T2Double V_Screen_Size;



	Vec3d V_Camera_Pos;
	Vec3d V_Camera_Look;
	Vec2d V_Camera_Look_Angle;

	Vec3d V_Camera_Speed;

	Vec3d V_Light1;


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

public:

	void M_CallbackDisplay();
	void M_CallbackReshape(int w, int h);
	void M_CallbackIdle();

	int M_Initialize(CEngine * P);
	void M_Initialize2(void);

	CGraphics();
	~CGraphics();

	

	
};

