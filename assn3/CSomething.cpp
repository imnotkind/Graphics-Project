#include "CSomething.h"



CSomething::~CSomething()
{
}

void CSomething::M_Move(T2Double d)
{
	auto math = CMath::getInstance();
	V_Rotate = math->M_2TV_Angle(V_Position, V_Position + d)[0];
	V_Position += d;
	
}

SDrawData CSomething::M_GetDrawData(void)
{
	SDrawData S;
	S.pos = V_Position;
	S.color = V_Color;
	S.img = V_Image;
	S.rotate = V_Rotate;
	S.size = V_Radius * 1.5;
	return S;
}