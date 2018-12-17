#pragma once
#include "MainHeader.h"

class CGeneral
{
	CGeneral() { Instance = NULL; }
	~CGeneral() { freeInstance(); }
	vector<T4Int> V_Pallete;
	static CGeneral* Instance;
public:
	static CGeneral* getInstance(void)
	{
		if (Instance == NULL) Instance = new CGeneral;
		return Instance;
	}
	static void freeInstance(void)
	{
		if (Instance) delete Instance;
	}
	T4Int M_Pallete(int p);
	
};

