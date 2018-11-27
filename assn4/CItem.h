#pragma once
#include "CSomething.h"
class CItem :
	public CSomething
{
	int V_Type;
public:
	int M_GetType(void) { return V_Type; }
	virtual void M_Loop(double t);
	CItem(T2Double p, int i, T4Int c, double r, int t) : CSomething(p, i, c, r, 0.0), V_Type(t) {}

	virtual ~CItem() {}
};

