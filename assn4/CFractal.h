#pragma once
#include "CHandler.h"
#include "CDrawing.h"

struct SFractal
{
	vector<glm::mat4> V_Initial;
	vector<glm::mat4> V_Grow;
	glm::mat4 V_Trans_S; // no inheritance
	
	shared_ptr<CDrawing> V_Draw;

};

class CFractal :
	public CHandler
{
	SFractal V_Info;
	void M_Draw_Rec(glm::mat4 CTM, int depth);
	int V_MaxDepth;
	SRenderInfo V_RenderInfo;

public:
	CFractal(const SFractal& i) : V_Info(i) {}

	void M_Draw(const SRenderInfo& r, int max);
	virtual ~CFractal(){}
};

