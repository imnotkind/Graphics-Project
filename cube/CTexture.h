#pragma once
#include "CHandler.h"
#include "readbmp.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();
	void M_LoadTexture(string path);
};

