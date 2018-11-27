#include "CBullet.h"


CBullet::~CBullet()
{
}

void CBullet::M_Loop(double t)
{
	auto p = V_Velocity;
	p *= t;
	V_Position += p;
}