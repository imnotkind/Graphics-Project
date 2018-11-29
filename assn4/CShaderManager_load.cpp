#include "CShaderManager.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader.h"

void CShaderManager::M_LoadShader(string path, string name, int type)
{
	GLuint id = glCreateShader(type);

	string code;
	ifstream is(path.c_str(), std::ios::in);
	if (is.is_open())
	{
		std::string Line = "";
		while (getline(is, Line)) code += "\n" + Line;
		is.close();
	}
	else CError("Shader code " + path + " not found.", true);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const * sp = code.c_str();
	glShaderSource(id, 1, &sp, NULL);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> error_msg(InfoLogLength + 1);
		glGetShaderInfoLog(id, InfoLogLength, NULL, &error_msg[0]);
		printf("%s\n", &error_msg[0]);
		CError("Shader code " + path + " can't be compoiled", true);
	}

	if (type == GL_VERTEX_SHADER)
		V_VerShaders[name] = id;
	else
		V_FragShaders[name] = id;

}
void CShaderManager::M_LoadMesh(string path, string name)
{
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string warn, err;

	vector<Vec4d> vertices;
	vector<Vec4d> uvs;
	vector<Vec3d> normals;

	bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

	if (loaded == false)
		CError("obj file not found :" + path, true);
	
	int index = 0;
	for (auto s : shapes)
	{
		vertices.clear();
		uvs.clear();
		normals.clear();
		for (auto idx : s.mesh.indices)
		{
			if(idx.vertex_index != -1)
				vertices.emplace_back(
					attrib.vertices[3 * idx.vertex_index + 0], 
					attrib.vertices[3 * idx.vertex_index + 1], 
					attrib.vertices[3 * idx.vertex_index + 2], 
					1);
			
			if(idx.texcoord_index != -1)
				uvs.emplace_back(
					attrib.texcoords[2 * idx.texcoord_index + 0], 
					attrib.texcoords[2 * idx.texcoord_index + 1], 
					1, 1);
			

			if (idx.normal_index != -1)
				normals.emplace_back(
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]);
			else
				normals.emplace_back(1, 0, 0);
		}

		int n = s.mesh.indices.size();
		float* arr = new float[n * 4];
		float* norm = new float[n * 3];
		float* tex = new float[n * 2];

		for (int i = 0; i < n; i++)
		{
			int k = i * 4;
			int q = i * 3;
			int f = i * 2;

			for (int j = 0; j < 4; j++)
				arr[k + j] = vertices[i][j];
			for (int j = 0; j < 3; j++)
				norm[q + j] = normals[i][j];
			for (int j = 0; j < 2; j++)
				tex[f + j] = uvs[i][j];
		}

		GLuint vbid;
		GLuint vaid;

		glGenVertexArrays(1, &vaid);
		glBindVertexArray(vaid);
		glGenBuffers(1, &vbid);
		glBindBuffer(GL_ARRAY_BUFFER, vbid); // attach to currently bound vertex array

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (n * 4 + n * 3 + n*2), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)* (n * 4), arr);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)* (n * 4), sizeof(float)* (n * 3), norm);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)* (n * 7), sizeof(float)* (n * 2), tex);

		delete[] arr;
		delete[] norm;
		delete[] tex;

		ostringstream os;
		os << name << "_" << index;

		SVerArray va; va.num = n; va.aindex = vaid;
		V_Polygons[os.str()] = va;
		V_Buffers[os.str()] = vbid;
		index++;
	}
	

}
void CShaderManager::M_LoadPolygon(string data, string name)
{
	vector<string> l = StringHelper::M_split(data, ',');

	vector<float> fa(l.size());
	transform(l.begin(), l.end(), fa.begin(), [](string s)->float {return atof(s.c_str()); });

	vector<Vec4d> temp_arr;
	for (int i = 0; i < fa.size() / 3; i++)
	{
		int k = i * 3;
		temp_arr.emplace_back(fa[k], fa[k + 1], fa[k + 2], 1);
	}

	int n = temp_arr.size();
	float* arr = new float[n*4];

	for (int i = 0; i < n; i++)
	{
		int k = i * 4;
		for (int j = 0; j < 4; j++)
			arr[k + j] = temp_arr[i][j];
	}

	GLuint vbid;
	GLuint vaid;


	float* tex = new float[n * 2];
	if (name == "cube1") //test for tex coordinate
	{
		for (int i = 0; i < 6; i++)
		{
			int k = i * 6;
			float test[3];

			for (int d = 0; d < 3; d++)
			{
				test[d] = temp_arr[k][d];
				for (int j = 0; j < 6; j++)
					if (test[d] != temp_arr[k + j][d]) test[d] = -5;
			}

			int exam[2]; int count = 0;
			for (int d = 0; d < 3; d++)
			{
				if (test[d] < -4) exam[count] = d, count++;
			}
			
			for (int j = 0; j < 6; j++)
			{
				tex[2 * (k + j) + 0] = temp_arr[k + j][exam[0]] * 0.5 + 0.5;
				tex[2 * (k + j) + 1] = temp_arr[k + j][exam[1]] * 0.5 + 0.5;
			}
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			int k = i * 2;
			tex[k + 0] = 0;
			tex[k + 1] = 0;
		}
	}

	glGenVertexArrays(1, &vaid);
	glBindVertexArray(vaid);
	glGenBuffers(1, &vbid);
	glBindBuffer(GL_ARRAY_BUFFER, vbid); // attach to currently bound vertex array
		
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (n * 4 + n * 2), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)* (n * 4), arr);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)* (n * 4), sizeof(float)* (n * 2), tex);
	delete[] tex;
	delete[] arr;



	
	

	SVerArray va; va.num = n; va.aindex = vaid;
	V_Polygons[name] = va;
	V_Buffers[name] = vbid;
}
void CShaderManager::M_LoadProgram(string name, string ver, string frag)
{
	GLuint id = glCreateProgram();

	glAttachShader(id, V_VerShaders[ver]);
	glAttachShader(id, V_FragShaders[frag]);

	glBindFragDataLocation(id, 0, "color");
	glLinkProgram(id);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(id, GL_LINK_STATUS, &Result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> error_msg(InfoLogLength + 1);
		glGetProgramInfoLog(id, InfoLogLength, NULL, &error_msg[0]);
		printf("%s\n", &error_msg[0]);
		CError("Program " + name + " can't be linked", true);
	}

	if (name == "prg1")
	{
		V_Programs[name] = id;
		auto vl = glGetAttribLocation(id, "position");
		auto tl = glGetAttribLocation(id, "texcoord");

		for (auto p : V_Polygons)
		{
			auto a = p.second;
			auto b = V_Buffers[p.first];

			glBindVertexArray(a.aindex);
			glBindBuffer(GL_ARRAY_BUFFER, b);

			glEnableVertexAttribArray(vl);
			glVertexAttribPointer(vl, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(tl);
			glVertexAttribPointer(tl, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * a.num));
		}
	}
	else if(name == "prg2")
	{
		V_Programs[name] = id;
		auto vl = glGetAttribLocation(id, "position");
		auto tl = glGetAttribLocation(id, "texcoord");

		for (auto p : V_Polygons)
		{
			auto a = p.second;
			auto b = V_Buffers[p.first];

			glBindVertexArray(a.aindex);
			glBindBuffer(GL_ARRAY_BUFFER, b);

			glEnableVertexAttribArray(vl);
			glVertexAttribPointer(vl, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(tl);
			glVertexAttribPointer(tl, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * a.num));
		}
	}
	else if (name == "prg3")
	{
		V_Programs[name] = id;
		auto vl = glGetAttribLocation(id, "position");
		auto nl = glGetAttribLocation(id, "normal");

		for (auto p : V_Polygons)
		{
			auto a = p.second;
			auto b = V_Buffers[p.first];

			glBindVertexArray(a.aindex);
			glBindBuffer(GL_ARRAY_BUFFER, b);

			glEnableVertexAttribArray(vl);
			glVertexAttribPointer(vl, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(nl);
			glVertexAttribPointer(nl, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * a.num));
		}
	}
	else if (name == "prg4")
	{
		V_Programs[name] = id;
		auto vl = glGetAttribLocation(id, "position");
		auto nl = glGetAttribLocation(id, "normal");
		auto tl = glGetAttribLocation(id, "texcoord");

		for (auto p : V_Polygons)
		{
			auto a = p.second;
			auto b = V_Buffers[p.first];

			glBindVertexArray(a.aindex);
			glBindBuffer(GL_ARRAY_BUFFER, b);

			glEnableVertexAttribArray(vl);
			glVertexAttribPointer(vl, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(nl);
			glVertexAttribPointer(nl, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * a.num));

			glEnableVertexAttribArray(tl);
			glVertexAttribPointer(tl, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 7 * a.num));
		}
	}
	

}


void CShaderManager::M_ParseData(string line, map<string, string>& t, int mode)
{
	if (mode == 0) //direct
	{
		vector<string> l = StringHelper::M_split(line, ':');
		if (l.size() != 2)
			CError("invalid data format", true);
		string name = StringHelper::M_trim(l[0]);
		string path = StringHelper::M_trim(l[1]);
		t[name] = path;
	}


	if (mode == 1 || mode == 2) //indirect // mode 1 : polygon, mode 2 : program
	{
		ifstream is(line.c_str(), std::ios::in);
		if (is.is_open())
		{
			string subLine = "";
			while (getline(is, subLine))
			{
				vector<string> l = StringHelper::M_split(subLine, ':');

				/*
				if(mode == 1 && l.size() != 3)
					CError("invalid data format", true);
				if (mode == 2 && l.size() != 2)
					CError("invalid data format", true);
				*/

				string name = StringHelper::M_trim(l[0]);
				string data = StringHelper::M_trim(l[1]);

				if (mode == 1)
				{
					V_Polygon_suggested_mode[name] = atoi(StringHelper::M_trim(l[2]).c_str());

					vector<string> ll = StringHelper::M_split(l[3] , ',');
					V_Polygon_aux[name] = T3Int(atoi(StringHelper::M_trim(ll[0]).c_str()), atoi(StringHelper::M_trim(ll[1]).c_str()), atoi(StringHelper::M_trim(ll[2]).c_str()));
				}
				

				t[name] = data;
			}
			is.close();
		}
		else CError("file " + line + " not found.", true);
	}
}


