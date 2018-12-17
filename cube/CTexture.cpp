#include "CTexture.h"



CTexture::CTexture()
{
}


CTexture::~CTexture()
{
}

void CTexture::M_LoadTexture(string path)
{
	Image im;
	int res = ImageLoad(path.c_str(), &im);

	if (res == 0)
		CError("texture load fail", true);


}