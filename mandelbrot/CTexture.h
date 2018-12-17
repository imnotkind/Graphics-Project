#pragma once
#include "CHandler.h"

class CTexture
{
public:
	GLuint textureID;
	CTexture(string imagepath);
	virtual ~CTexture();
};

