#pragma once
#include "CSomething.h"
class CBullet :
	public CSomething
{
	T2Double V_Velocity;
public:

	virtual void M_Loop(double t);

	CBullet(T2Double p, int i, T4Int c, double r, T2Double v) : CSomething(p,i,c,r, 0.0) { V_Velocity = v; }
	virtual ~CBullet();
};

