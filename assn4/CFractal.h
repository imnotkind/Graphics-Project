#pragma once
#include "CHandler.h"
#include "CDrawing.h"

struct SFractal
{
	
	glm::mat4 trans;
	glm::mat4 trans_s; // no inheritance
};

class CFractal :
	public CHandler
{
	shared_ptr<CDrawing> V_Draw;


	vector<glm::mat4> V_Initial;
	vector<glm::mat4> V_Grow;

	void M_DrawRec(glm::mat4 CTM, int depth);

	int V_MaxDepth;
	SRenderInfo V_RenderInfo;
public:
	CFractal();
	virtual ~CFractal(){}
};

