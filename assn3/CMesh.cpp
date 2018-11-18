#include "CMesh.h"


//path of obj file and metadata file
CMesh::CMesh(string meta)
{
	

	int group_num;
	string obj_name;
	string obj_file_path;
	vector<int> group_port;
	vector<set<int>> group_info;
	vector<glm::vec3> group_translation;
	vector<pair<float, glm::vec3>> group_rotation;

	vector<int> parent_info;
	vector<pair<float, glm::vec3>> parent_rotation;
	vector<glm::vec3> parent_translation;

	T4Double line_color;
	T4Double surface_color;


	//metadata load
	string Line = "";

	ifstream is((meta).c_str(), std::ios::in);
	if (is.is_open())
	{	
		
		while (getline(is, Line))
		{
			if (Line == "%group_number")
			{
				getline(is, Line);
				group_num = atoi(Line.c_str());
				//if (V_group_num != V_LoadedMeshes.size())
				//CError("Incorrect data format", true);
			}

			if (Line == "%obj_file_path")
			{
				getline(is, Line);
				obj_file_path = Line;
			}

			if (Line == "%obj_name")
			{
				getline(is, Line);
				obj_name = Line;
				V_Name = obj_name;
			}

			if (Line == "%group_port")
			{
				getline(is, Line);
				vector<string> l = StringHelper::M_split(Line,',');
				for (auto s : l)
				{
					group_port.push_back(atoi(s.c_str()));
				}
			}

			if (Line == "%group_info")
			{
				for (int i = 0; i < group_num; i++)
				{
					getline(is, Line);
					vector<string> l = StringHelper::M_split(Line, ',');
					set<int> il;
					for (auto s : l)
					{
						il.insert(atoi(s.c_str()));
					}
					group_info.push_back(il);
				}
			}

			if (Line == "%group_translation")
			{
				for (int i = 0; i < group_num; i++)
				{
					getline(is, Line);
					vector<string> l = StringHelper::M_split(Line, ',');
					if (l.size() != 3)
						CError("Incorrect data format", true);

					glm::vec3 v(
						atof(l[0].c_str()), 
						atof(l[1].c_str()), 
						atof(l[2].c_str())
					);

					group_translation.push_back(v);
				}
			}

			if (Line == "%group_rotation")
			{
				for (int i = 0; i < group_num; i++)
				{
					getline(is, Line);
					vector<string> l = StringHelper::M_split(Line, ',');
					if (l.size() != 4)
						CError("Incorrect data format", true);

					glm::vec3 v(
						atof(l[1].c_str()),
						atof(l[2].c_str()),
						atof(l[3].c_str())
					);

					group_rotation.push_back(pair<float,glm::vec3>((float)DTR(atof(l[0].c_str())),v));
				}
			}

			if (Line == "%parent_info")
			{
				getline(is, Line);
				vector<string> l = StringHelper::M_split(Line, ',');
				if (l.size() != group_num)
					CError("Incorrect data format", true);

				for (string s : l)
				{
					parent_info.push_back(atoi(s.c_str()));
				}
			}

			if (Line == "%parent_rotation")
			{
				for (int i = 0; i < group_num; i++)
				{
					getline(is, Line);
					vector<string> l = StringHelper::M_split(Line, ',');
					if (l.size() != 4)
						CError("Incorrect data format", true);

					glm::vec3 v(
						atof(l[1].c_str()),
						atof(l[2].c_str()),
						atof(l[3].c_str())
					);

					parent_rotation.push_back(pair<float, glm::vec3>((float)DTR(atof(l[0].c_str())), v));
				}
			}

			if (Line == "%parent_translation")
			{
				for (int i = 0; i < group_num; i++)
				{
					getline(is, Line);
					vector<string> l = StringHelper::M_split(Line, ',');
					if (l.size() != 3)
						CError("Incorrect data format", true);

					glm::vec3 v(
						atof(l[0].c_str()),
						atof(l[1].c_str()),
						atof(l[2].c_str())
					);

					parent_translation.push_back(v);
				}
			}

			if (Line == "%line_color")
			{

				getline(is, Line);
				vector<string> l = StringHelper::M_split(Line, ',');
				if (l.size() != 4)
					CError("Incorrect data format", true);

				line_color.set(
					atoi(l[0].c_str()) / 255.0,
					atoi(l[1].c_str()) / 255.0,
					atoi(l[2].c_str()) / 255.0,
					atoi(l[3].c_str()) / 255.0
				);
			}

			if (Line == "%surface_color")
			{

				getline(is, Line);
				vector<string> l = StringHelper::M_split(Line, ',');
				if (l.size() != 4)
					CError("Incorrect data format", true);

				surface_color.set(
					atoi(l[0].c_str()) / 255.0,
					atoi(l[1].c_str()) / 255.0,
					atoi(l[2].c_str()) / 255.0,
					atoi(l[3].c_str()) / 255.0
				);


			}
		}
		is.close();
	}
	else CError("Config file " + meta + " not found.", true);
	for (int i = 0; i < group_num; i++)
	{
		SMeshGroup G;
		G.group_members = group_info[i];
		G.group_parent = parent_info[i];
		G.trans_origin = group_translation[i];
		G.rotate_origin = group_rotation[i];
		G.trans_parent = parent_translation[i];
		G.rotate_parent = parent_rotation[i];
		G.port = group_port[i];

		V_Groups.emplace_back(G);
	}

	V_LineColor = line_color;
	V_SurfaceColor = surface_color;
	M_ConstructHierModel();
}



void CMesh::M_Rec_Construct(map<int, SHierModelNode>& all, vector<treenode>& treenodes, int root, int sibling, int off)
{
	SHierModelNode N;
	SDrawingInfo D;
	D.DrawMode = 0;
	D.Global_Color.set(1.0, 0.0, 0.0, 1.0);
	D.Line_Color.set(1.0, 1.0, 0.0, 1.0);
	D.PolygonName = "NULL";
	D.Program = "prg1";


	N.draw.reset(new CDrawing(D));
	N.port = V_Groups[root].port; 

	N.trans = glm::translate(glm::mat4(1.0), V_Groups[root].trans_parent);
	N.trans = glm::rotate(N.trans,V_Groups[root].rotate_parent.first, V_Groups[root].rotate_parent.second);

	N.trans_s = glm::rotate(glm::mat4(1.0), V_Groups[root].rotate_origin.first, V_Groups[root].rotate_origin.second);
	N.trans_s = glm::translate(N.trans_s, V_Groups[root].trans_origin);

	for (auto f : V_Groups[root].group_members) N.homos.push_back(f + off);

	treenode& t = treenodes[root];
	N.left_child = t.second.empty() ? -1 : t.second[0];
	N.right_sibling = sibling;

	all[root] = N;
	
	for (int i = 0; i < t.second.size(); i++)
	{
		int next = i == t.second.size() - 1 ? -1 : t.second[i + 1];
		M_Rec_Construct(all, treenodes, t.second[i], next, off);
	}
}

void CMesh::M_ConstructHierModel(void)
{
	int N = V_Groups.size();
	vector<treenode> treenodes;
	treenodes.resize(N);

	for (int i = 0; i < N; i++) //find root and swap with 0th
	{
		if (V_Groups[i].group_parent == -1)
		{
			swap(V_Groups[i], V_Groups[0]);
			continue;
		}
	}
	for (int i = 0; i < N; i++)
	{
		treenodes[i].first = V_Groups[i].group_parent;
		if (i == 0) continue;
		treenodes[V_Groups[i].group_parent].second.push_back(i);
	}

	int max = 29;
	for (int i = 0; i < N; i++) //most among sub-meshes that are member of some group
		for (auto j : V_Groups[i].group_members)
			if (j > max) max = j;

	vector<SHierModelNode> result;
	map<int, SHierModelNode> all;

	M_Rec_Construct(all, treenodes, 0, -1, N);
	

	for (int i = 0; i < N; i++)
		result.push_back(all[i]);

	
	for (int i = 0; i < max; i++)
	{
		ostringstream os;
		os << V_Name << "_" << i;

		SHierModelNode N;
		SDrawingInfo D;

		
		D.Global_Color = V_SurfaceColor;
		D.Line_Color = V_LineColor;
		D.PolygonName = os.str();
		D.Program = "prg1";

		auto p = CShaderManager::getInstance();
		auto m = p->V_Polygon_suggested_mode;

		if (m.find(D.PolygonName) != m.end())
			D.DrawMode = m[D.PolygonName];
		else
			D.DrawMode = 5;
		
		N.port = -1;
		N.draw.reset(new CDrawing(D));
		N.left_child = -1;
		N.right_sibling = -1;

		result.emplace_back(N);
	}


	V_Model.reset(new CHierModel(result));
}