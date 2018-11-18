#pragma once
#include "CHandler.h"
#include "CShaderManager.h"

struct SDrawingInfo
{
	string PolygonName;
	string Program;
	T4Double Global_Color;
	int DrawMode;
	//optional
	T4Double Line_Color;
};

class CDrawing :
	public CHandler
{
public:
	CShaderManager* V_PSM;

	SVerArray V_Array;
	string V_Program; // what program should be used for this drawing
	T4Double V_Color; // global color
	int V_DrawMode; //0 : point, 1 : line, 2 : strip, 3 : strip + line, 4 : triangles, 5: triangles + lines
	T4Double V_LineColor;

	CDrawing(const SDrawingInfo& s);
	void M_Draw(const glm::mat4& mat, T4Double color);
	virtual ~CDrawing() {}
};

