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

	GLuint texture;  // -1 : no tex
	bool light;
};

struct SLight
{
	glm::vec4 dif;
	glm::vec4 spc;
	glm::vec4 pos;
};

struct SRenderInfo
{
	glm::mat4 modelview;
	glm::mat4 projection;

	glm::mat4 normtrans;

	vector<SLight> lights;

	glm::vec4 amb;

	T4Double color;
	bool keeplight;
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

	GLuint V_Texture;
	bool V_Light;

	CDrawing(const SDrawingInfo& s);
	void M_Draw(const SRenderInfo& r);
	virtual ~CDrawing() {}
};

