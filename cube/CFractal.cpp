#include "CFractal.h"


glm::mat4 CFractal::M_Trace(vector<int> trace)
{
	auto m = V_Info.V_Initial[0];
	for (auto x : trace)
	{
		m = m * V_Info.V_Grow[x];
	}
	return m;
}

void CFractal::M_Draw_Rec(glm::mat4 CTM, int depth)
{
	if (depth == V_MaxDepth) return;

	SRenderInfo tri = V_RenderInfo;
	
	auto temp = CTM * V_Info.V_Trans_S;
	tri.modelview = temp;
	tri.normtrans = tri.modelview;

	
	V_Info.V_Draw->M_Draw(tri);
	if (!tri.keeplight) V_RenderInfo.keeplight = true;

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

void CFractal::M_Draw_Rec_t(glm::mat4 CTM, int depth)
{
	
	if (depth > V_Front) return;

	
	SRenderInfo tri = V_RenderInfo;

	auto temp = CTM * V_Info.V_Trans_S;
	tri.modelview = temp;
	tri.normtrans = tri.modelview;

	if(depth == V_Back) tri.color.set(0.0, 1.0, 0.0, 1.0);
	else  tri.color.set(1.0, 0.0, 0.0, 1.0);

	V_Info.V_Draw->M_Draw(tri);
	if (!tri.keeplight) V_RenderInfo.keeplight = true;
	
	if (depth < V_Back)
	{
		M_Draw_Rec_t(CTM * V_Info.V_Grow[V_Trace[depth]], depth + 1);
		return;
	}

	for (auto m : V_Info.V_Grow)
	{
		M_Draw_Rec_t(CTM * m, depth + 1);
	}
		
}
void CFractal::M_Draw(const SRenderInfo& r, vector<int> trace, int back, int front)
{

	V_Trace = trace;
	V_Back = back;
	V_Front = front;

	V_RenderInfo = r;

	for (auto m : V_Info.V_Initial)
	{
		M_Draw_Rec_t(r.modelview * m, 0);
	}
}