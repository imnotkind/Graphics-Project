#include "CEngine.h"



CEngine::CEngine()
{
}


CEngine::~CEngine()
{
}
void CEngine::M_ListenMessages(void)
{

	//Input Messages;
	auto iq = SIQueue::M_GetSingletone(0);
	auto rq = SIQueue::M_GetSingletone(1);

	while (!iq->M_Empty())
	{
		auto m = iq->M_Pop();
		if (m.type == "down")
		{
			if (!M_Event_KeyPress(m.key, m.special)) rq->M_Push(m); //not mine!
		}
	}
	while (!rq->M_Empty())
	{
		iq->M_Push(rq->M_Pop());
	}

}

void CEngine::M_Loop(void)
{
	
	M_ListenMessages();
	M_CheckKeyPress();

}
void CEngine::M_CheckKeyPress()
{
	auto iq = CUserInput::getInstance();
	
}
bool CEngine::M_Event_KeyPress(int key, bool special)
{

	return false;
}


void CEngine::M_Initialize(void)
{
	
}
