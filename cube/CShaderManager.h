#pragma once
#include "CHandler.h"
#include <fstream>
#include <sstream>
#include "CTexture.h"

struct SVerArray
{
	int aindex;
	int num;
};
class CShaderManager :
	public CHandler
{

	map<string, GLuint> V_Programs;
	map<string, GLuint> V_VerShaders;
	map<string, GLuint> V_FragShaders;

	map<string, SVerArray> V_Polygons;
	map<string, GLuint> V_Buffers;

	map<string, T3Int> V_Polygon_aux;
	

	void M_LoadShader(string path, string name, int type);
	void M_LoadPolygon(string data, string name);
	void M_LoadProgram(string name, string ver, string frag);

	void M_ParseData(string line, map<string, string>& t, int mode);

	void M_LoadMesh(string path, string name);


	GLuint V_CurrentProgram;

	static CShaderManager* Instance;
	CShaderManager(string config_path);
	~CShaderManager();

	map<pair<string, GLuint>, GLuint> V_LocCache;
public:
	map<string, shared_ptr<CTexture>> V_Textures;
	set<string> V_Meshes; // meta path
	map<string, int> V_Polygon_suggested_mode;
	
	GLuint M_GetUniformLoc(string s);


	

	SVerArray M_GetPolygon(string s) { return V_Polygons[s]; }
	const map<string, SVerArray>& M_GetPolygonList(void) { return V_Polygons; }

	void M_UseProgram(string name);
	GLuint M_GetProgram(void) { return V_CurrentProgram; }

	static CShaderManager* getInstance(void)
	{
		if (Instance == NULL)Instance = new CShaderManager("resource/config.txt");
		return Instance;
	}
	static void freeInstance(void)
	{
		if (Instance) delete Instance;
	}
};

