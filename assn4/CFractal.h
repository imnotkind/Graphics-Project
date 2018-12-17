#pragma once
#include "CHandler.h"
#include "CDrawing.h"
#include "CHierModel.h"

struct SFractal
{
	vector<glm::mat4> V_Initial;
	vector<glm::mat4> V_Grow;
	glm::mat4 V_Trans_S; // no inheritance
	
	shared_ptr<CHierModel> V_Draw;

};

class CFractal :
	public CHandler
{
	SFractal V_Info;

	void M_Draw_Rec(glm::mat4 CTM, int depth);
	void M_Draw_Rec_t(glm::mat4 CTM, int depth);


	int V_MaxDepth; //for ordinary rendering
	vector<int> V_Trace; int V_Back; int V_Front; //for partial rendering

	SRenderInfo V_RenderInfo;

public:
	CFractal(const SFractal& i) : V_Info(i) {}

	void M_Draw(const SRenderInfo& r, int max);
	void M_Draw(const SRenderInfo& r, vector<int> trace, int back, int front);

	glm::mat4 M_Trace(vector<int> trace);
	virtual ~CFractal(){}
};

