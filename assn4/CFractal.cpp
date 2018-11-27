#include "CFractal.h"




void CFractal::M_Draw_Rec(glm::mat4 CTM, int depth)
{
	if (depth == V_MaxDepth) return;

	SRenderInfo tri = V_RenderInfo;
	
	auto temp = CTM * V_Info.V_Trans_S;
	tri.modelview = temp;
	tri.normtrans = tri.modelview;

	for (auto m : V_Info.V_Grow)
	{
		M_Draw_Rec(CTM * m, depth + 1);
	}
}

void CFractal::M_Draw(const SRenderInfo& r)
{
	V_RenderInfo = r;
	M_Draw_Rec(r.modelview, 0); // 0 is root
}