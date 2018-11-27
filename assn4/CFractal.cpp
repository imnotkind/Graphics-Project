#include "CFractal.h"




void CFractal::M_Draw_Rec(glm::mat4 CTM, int depth)
{
	if (depth == V_MaxDepth) return;

	SRenderInfo tri = V_RenderInfo;
	
	auto temp = CTM * V_Info.V_Trans_S;
	tri.modelview = temp;
	tri.normtrans = tri.modelview;

	V_Info.V_Draw->M_Draw(tri);

	for (auto m : V_Info.V_Grow)
	{
		M_Draw_Rec(CTM * m, depth + 1);

	}
}

void CFractal::M_Draw(const SRenderInfo& r, int max)
{
	V_MaxDepth = max;
	V_RenderInfo = r;

	for (auto m : V_Info.V_Initial)
	{
		M_Draw_Rec(r.modelview * m, 0);
	}
}