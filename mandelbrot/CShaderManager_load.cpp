#include "CShaderManager.h"


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
		//CError("Shader code " + path + " can't be compoiled", true);
	}

	if (type == GL_VERTEX_SHADER)
		V_VerShaders[name] = id;
	else
		V_FragShaders[name] = id;

}
void CShaderManager::M_LoadMesh(string path, string name)
{
	objl::Loader loader;
	loader.LoadFile(path);
	auto mesh = loader.LoadedMeshes;

	int index = 0;
	for (auto m : mesh)
	{
		int n = m.Indices.size() * 4;
		int k = 0;
		float* arr = new float[n];
		for (auto i : m.Indices)
		{
			arr[k + 0] = m.Vertices[i].Position.X;
			arr[k + 1] = m.Vertices[i].Position.Y;
			arr[k + 2] = m.Vertices[i].Position.Z;
			arr[k + 3] = 1;
			k += 4;
		}

		GLuint vbid;
		GLuint vaid;

		glGenVertexArrays(1, &vaid);
		glBindVertexArray(vaid);
		glGenBuffers(1, &vbid);
		glBindBuffer(GL_ARRAY_BUFFER, vbid); // attach to currently bound vertex array
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), &arr[0], GL_STATIC_DRAW);
		delete[] arr;

		ostringstream os;
		os << name << "_" << index;

		SVerArray va; va.num = n/4; va.aindex = vaid;
		V_Polygons[os.str()] = va;
		V_Buffers[os.str()] = vbid;
		index++;
	}
}
void CShaderManager::M_LoadPolygon(string data, string name)
{

	vector<string> l = StringHelper::M_split(data, ',');
	float* arr = new float[l.size() / 3 * 4];

	int k = 0;
	for (int i = 1; i <= l.size(); i++)
	{
		arr[i - 1 + k] = atof(l[i-1].c_str());
		if (i % 3 == 0)
		{
			arr[i + k] = 1.0;
			k++;
		}
	}

	GLuint vbid;
	GLuint vaid;

	glGenVertexArrays(1, &vaid);
	glBindVertexArray(vaid);
	glGenBuffers(1, &vbid);
	glBindBuffer(GL_ARRAY_BUFFER, vbid); // attach to currently bound vertex array
	glBufferData(GL_ARRAY_BUFFER, l.size() / 3 * 4 * sizeof(float), &arr[0], GL_STATIC_DRAW);
	delete[] arr;
	

	SVerArray va; va.num = l.size() / 3; va.aindex = vaid;
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
		//CError("Program " + name + " can't be linked", true);
	}

	V_Programs[name] = id;
	auto vl = glGetAttribLocation(id, "position");
	for (auto p : V_Polygons)
	{
		auto a = p.second;
		auto b = V_Buffers[p.first];

		glBindVertexArray(a.aindex);
		glBindBuffer(GL_ARRAY_BUFFER, b);

		glEnableVertexAttribArray(vl);
		glVertexAttribPointer(vl, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
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


	if (mode == 1 || mode == 2) //indirect
	{
		ifstream is(line.c_str(), std::ios::in);
		if (is.is_open())
		{
			string subLine = "";
			while (getline(is, subLine))
			{
				vector<string> l = StringHelper::M_split(subLine, ':');

				if(mode == 1 && l.size() != 3)
					CError("invalid data format", true);
				if (mode == 2 && l.size() != 2)
					CError("invalid data format", true);

				string name = StringHelper::M_trim(l[0]);
				string data = StringHelper::M_trim(l[1]);

				if (mode == 1)
					V_Polygon_suggested_mode[name] = atoi(StringHelper::M_trim(l[2]).c_str());

				t[name] = data;
			}
			is.close();
		}
		else CError("file " + line + " not found.", true);
	}
}


