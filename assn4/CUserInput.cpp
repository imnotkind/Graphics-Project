#include "CUserInput.h"



CUserInput::CUserInput()
{
}


CUserInput::~CUserInput()
{
}

void CUserInput::M_PressDown(int key, bool special)
{
	if (M_IfPressed(key, special))
	{
		return;
	}

	auto iq = SIQueue::M_GetSingletone(0);
	SInputMessage s;
	s.key = key;
	s.special = special;
	s.type = "down";
	iq->M_Push(s);

	V_pressingkeys.insert(make_pair(key,special));

}
void CUserInput::M_PressUp(int key, bool special)
{
	auto iq = SIQueue::M_GetSingletone(0);
	SInputMessage s;
	s.key = key;
	s.special = special;
	s.type = "up";
	iq->M_Push(s);

	V_pressingkeys.erase(make_pair(key, special));
}
bool CUserInput::M_IfPressed(int key, bool special)
{
	return V_pressingkeys.find(make_pair(key, special)) != V_pressingkeys.end();
}