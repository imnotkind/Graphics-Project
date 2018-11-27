#pragma once

#include "TGrid.h"
#include "CHandler.h"
#include "MessageQueue.h"

#include "CUserInput.h"
#include <fstream>

#include <set>
#include <list>

class CGraphics;

class CEngine : public CHandler
{

	void M_ListenMessages(void);
	bool M_Event_KeyPress(int key, bool special); //false : not mine


public:

	
	void M_Loop(void);
	void M_CheckKeyPress();
	void M_Initialize(void);
	

	CEngine();
	~CEngine();
};

