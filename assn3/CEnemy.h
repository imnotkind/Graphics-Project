#pragma once
#include "CSomething.h"
class CEnemy :
	public CSomething
{
	queue<T2Double> V_MoveQueue;
	double V_Speed;

	double V_Osil1;
	double V_Osil2; //parameters for dynamic speed


public:
	virtual void M_Loop(double t);

	void M_ClearMove(void);
	void M_PushMove(T2Double k) { V_MoveQueue.push(k); }
	bool M_MoveEmpty(void) { return V_MoveQueue.empty(); }

	void M_SetSpeed(double s) { V_Speed = s; }

	CEnemy(T2Double p, int i, T4Int c, double r, double s) : CSomething(p, i, c, r, 0.0), V_Speed(s) 
	{
		auto math = CMath::getInstance();
		V_Osil1 = math->M_Num_dRandom(0, PI);
		V_Osil2 = math->M_Num_dRandom(0.02, 0.09);
	}
	virtual ~CEnemy(){}
};

