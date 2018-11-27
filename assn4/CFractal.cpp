#include "CFractal.h"



CFractal::CFractal()
{
}




void CFractal::M_DrawRec(glm::mat4 CTM, int depth)
{
	if (depth == V_MaxDepth) return;

	SRenderInfo tri = V_RenderInfo;
	tri.modelview = temp;
	tri.normtrans = tri.modelview;

	for (auto m : V_Grow)
	{
		M_DrawRec(CTM * m, depth + 1);
	}
}