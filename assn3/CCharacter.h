#pragma once
#include "CSomething.h"
#include "CBullet.h"
#include "CUserInput.h"

class CCharacter :
	public CSomething
{
	list<int> V_Items;
	int V_Power;//bullet power
	int V_InvTime;
	int V_SuperTime;

	T2Double V_OldMP;
	T2Double V_LookAngle;

	void M_CalculateLook(void);
public:
	virtual void M_Loop(double t);

	void M_SuperFire(void);
	void M_MegaFire(void);
	void M_Fire(void); //fire a bullet
	void M_GetInvincible(int t);

	T2Double M_GetLook(void) { return V_LookAngle; }
	void M_Rotate(double r) { V_LookAngle[0] += r; }
	void M_MoveFirst(T2Double v); //with looking direction

	list<int>& M_GetItemList(void) {return V_Items;}

	CCharacter(T2Double p, int i, T4Int c, double r);
	virtual ~CCharacter() {}
};

