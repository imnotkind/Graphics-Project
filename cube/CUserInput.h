#pragma once
#include "CHandler.h"
#include "MessageQueue.h"

#define KEY_LEFT_CLICK (-1)

struct SInputMessage
{
	string type;
	int key;
	bool special;
};
typedef CMessageQueue<SInputMessage> SIQueue;

class CUserInput : public CHandler
{
	static CUserInput* Instance;
	set<pair<int, bool>> V_pressingkeys;
	CUserInput();

	T2Int V_MousePos;
	T2Double V_WinSize;

	
public:
	bool V_MousePrison;

	static CUserInput* getInstance(void)
	{
		if (Instance == NULL)Instance = new CUserInput;
		return Instance;
	}
	static void freeInstance(void)
	{
		if (Instance) delete Instance;
	}

	void M_PressDown(int key, bool special);
	void M_PressUp(int key, bool special);
	
	void M_SetWinSize(T2Double s) { V_WinSize = s; }
	void M_MouseSet(T2Int p) { V_MousePos = p; }
	T2Int M_MouseGet(void) { return V_MousePos; }
	T2Double M_MouseGet_Normalized(void) { return T2Double(V_MousePos[0] / V_WinSize[0], V_MousePos[1] / V_WinSize[1]); }

	bool M_IfPressed(int key, bool special);

	
	
	~CUserInput();
};

