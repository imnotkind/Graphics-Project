#include "CShaderManager.h"


CShaderManager::CShaderManager(string config_path)
{
	//parse well and
	map<string, string> VerShaderPaths;
	map<string, string> FragShaderPaths;
	map<string, string> PolygonData;
	map<string, string> ProgramData;
	
	set<string> ObjData;

	string Line = "";

	ifstream is(config_path.c_str(), std::ios::in);
	if (is.is_open())
	{
		while (getline(is, Line))
		{
			if(Line == "%vs_start")
			{
				getline(is, Line);
				while (Line != "%vs_end")
				{
					M_ParseData(Line, VerShaderPaths, 0);
					getline(is, Line);
				}
			}

			if (Line == "%fs_start")
			{
				getline(is, Line);
				while (Line != "%fs_end")
				{
					M_ParseData(Line, FragShaderPaths, 0);
					getline(is, Line);
				}
			}

			if (Line == "%polygon_start")
			{
				getline(is, Line);
				while (Line != "%polygon_end")
				{
					
					M_ParseData(Line, PolygonData, 1);
					getline(is, Line);
				}
			}

			if (Line == "%program_start")
			{
				getline(is, Line);
				while (Line != "%program_end")
				{
					
					M_ParseData(Line, ProgramData, 2);
					getline(is, Line);
				}
			}
		}
		is.close();
	}
	else CError("Config file " + config_path + " not found.", true);

	/*
	ObjData.insert("resource/OBJ files/dummy_obj_meta.txt");
	ObjData.insert("resource/OBJ files/dummy_obj_red_meta.txt");
	ObjData.insert("resource/OBJ files/M1911_meta.txt");
	ObjData.insert("resource/OBJ files/Skeleton_meta.txt");
	*/

	ObjData.insert("resource/OBJ files/sphere_meta.txt");
	ObjData.insert("resource/OBJ files/cube_meta.txt");
	ObjData.insert("resource/OBJ files/cube2_meta.txt");

	ObjData.insert("resource/OBJ files/sphere_hole_meta.txt");
	ObjData.insert("resource/OBJ files/wormhole_meta.txt");

	for (auto p : VerShaderPaths)
	{
		M_LoadShader(p.second, p.first, GL_VERTEX_SHADER);
	}
	for (auto p : FragShaderPaths)
	{
		M_LoadShader(p.second, p.first, GL_FRAGMENT_SHADER);
	}
	for(auto p : PolygonData)
	{
		M_LoadPolygon(p.second, p.first);
	}


	for (auto p : ObjData)
	{
		string mname, mpath;
		ifstream is(p.c_str(), std::ios::in);
		if (is.is_open())
		{
			while (getline(is, Line))
			{
				if (Line == "%obj_name")
				{
					getline(is, mname);
				}
				if (Line == "%obj_file_path")
				{
					getline(is, mpath);
				}
			}
		}
		else CError("Mesh meta data " + p + " not found.", true);
		M_LoadMesh(mpath, mname);
		V_Meshes.insert(p);
	}


	for (auto p : ProgramData)
	{
		vector<string> v = StringHelper::M_split(p.second, ',');
		M_LoadProgram(p.first, StringHelper::M_trim(v[0]), StringHelper::M_trim(v[1]));
	}
	SVerArray temp; temp.aindex = -1; temp.num = 0;
	V_Polygons["NULL"] = temp;

	V_Textures["pallete"] = shared_ptr<CTexture>(new CTexture("resource/pallete.bmp"));
}

CShaderManager::~CShaderManager()
{
	for (auto x : V_Polygons)
	{
		GLuint u = x.second.aindex;
		glDeleteVertexArrays(1, &u);
	}
	for (auto x : V_Buffers)
	{
		GLuint u = x.second;
		glDeleteBuffers(1, &u);
	}
	for (auto x : V_Programs) glDeleteProgram(x.second);
	for (auto x : V_VerShaders) glDeleteShader(x.second);
	for (auto x : V_FragShaders) glDeleteShader(x.second);

	freeInstance();

}


void CShaderManager::M_UseProgram(string name)
{ 
	if (V_CurrentProgram == V_Programs[name]) return; // no redundant binding
	V_CurrentProgram = V_Programs[name];
	glUseProgram(V_CurrentProgram); 
}

GLuint CShaderManager::M_GetUniformLoc(string s)
{
	auto k = pair<string, GLuint>(s, M_GetProgram());
	auto i = V_LocCache.find(k);
	if (i == V_LocCache.end())
	{
		auto x = glGetUniformLocation(M_GetProgram(), s.c_str());
		V_LocCache[k] = x;
		return x;
	}
	return i->second;
}