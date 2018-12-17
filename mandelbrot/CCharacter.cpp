#include "CCharacter.h"



CCharacter::CCharacter(T2Double p, int i, T4Int c, double r) : CSomething(p, i, c, r, 0.0) 
{
	V_OldMP.set(0.5, 0.5);
	V_Power = 0; V_InvTime = 0; V_SuperTime= 0;
	V_LookAngle.set(0.0, 0.0);
}
void CCharacter::M_GetInvincible(int t)
{
	V_InvTime = t;
	V_Color.set(255, 255, 0, 100);
}
void CCharacter::M_Loop(double t)
{
	if (V_Power < 25) V_Power += 1;
	if (V_InvTime > 0)
	{
		V_InvTime--;
		if(V_InvTime == 0) V_Color.set(255, 255, 255, 255);
	}
	if (V_SuperTime > 0)
	{
		V_SuperTime--;
		if (V_SuperTime % 5 == 0)
		{
			auto mq = SMQueue::M_GetSingletone(0);
			SScriptMessage message;

			for (int i = 0; i <= 3; i++)
			{
				double theta = V_Rotate + DTR(90) * i;
				double speed = 1;

				message.type = "creation";
				message.content = (void*) new CBullet(V_Position, 2, T4Int(0, 0, 0, 255), 0.3, T2Double(cos(theta), sin(theta)) * speed);
				mq->M_Push(message);
			}
		}
	}

	M_CalculateLook();
	
}
void CCharacter::M_CalculateLook(void)
{
	V_Rotate = V_LookAngle[0];

}
void CCharacter::M_SuperFire(void)
{
	V_SuperTime = 35;
}
void CCharacter::M_MegaFire(void)
{
	auto mq = SMQueue::M_GetSingletone(0);
	SScriptMessage message;
	auto math = CMath::getInstance();
	for (int i = 0; i < 30; i++)
	{
		double theta = DTR(math->M_Num_dRandom(0, 360));
		double speed = math->M_Num_dRandom(0.5, 3);

		message.type = "creation";
		message.content = (void*) new CBullet(V_Position, 2, T4Int(255, 0, 0, 255), 0.3, T2Double(cos(theta), sin(theta)) * speed);
		mq->M_Push(message);
	}
	
}

void CCharacter::M_MoveFirst(T2Double v)
{
	T2Double p;
	double d = V_LookAngle[0];
	p[0] = v[0] * cos(d) - v[1] * sin(d);
	p[1] = v[0] * sin(d) + v[1] * cos(d);

	V_Position += p;
}

void CCharacter::M_Fire(void)
{
	if (V_Power < 25) return;
	auto mq = SMQueue::M_GetSingletone(0);
	SScriptMessage message;

	for (int i = -1; i <= 1; i++)
	{
		double theta = V_Rotate + DTR(10) * i;
		double speed = 1;

		message.type = "creation";
		message.content = (void*) new CBullet(V_Position, 2, T4Int(255, 0, 0, 255), 0.3, T2Double(cos(theta), sin(theta)) * speed);
		mq->M_Push(message);
	}
	V_Power = 0;
}
