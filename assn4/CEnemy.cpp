#include "CEnemy.h"



void CEnemy::M_ClearMove(void)
{

	while (!V_MoveQueue.empty())V_MoveQueue.pop();
}
void CEnemy::M_Loop(double t)
{
	if (V_MoveQueue.empty()) return;

	auto math = CMath::getInstance();
	auto des = V_MoveQueue.front();

	V_Osil1 += V_Osil2;
	double asp = std::min(V_Speed*0.7 + V_Speed*0.3*sin(V_Osil1), math->M_2TV_Angle(V_Position, des)[1]);

	if (math->M_2TV_Angle(V_Position, des)[1] < 0.1) return; //normalize will go nan
	M_Move(math->M_2TV_Normalize(des - V_Position) * asp); //move by speed
	if (math->M_2TV_Angle(V_Position, des)[1] < 0.1)
	{
		V_MoveQueue.pop();
	}
}