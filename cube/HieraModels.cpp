#include "CGraphics.h"


SDrawingInfo temptemp(string s, T4Double c)
{
	SDrawingInfo d;
	d.DrawMode = 4;
	d.Global_Color = c;
	d.PolygonName = s;
	d.Program = "prg4";
	d.Line_Color.set(1.0, 0.0, 0.0, 1.0);
	d.light = true;

	return d;
}
void CGraphics::M_SimplePolyFractal(void)
{
	float s = 35.0/50.0;
	float h = 40;
	float r = 82.5;

	//tapering
	auto tm = glm::mat4(1.0);
	tm = glm::translate(tm, glm::vec3(0, 0, -h*0.5));
	tm[2][3] = 1/h*0.9;

	auto tm2 = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, h*0.5));
	tm = tm2 * tm;

	tm = glm::mat4(1.0); tm[2][2] = -1;
	

	vector<SHierModelNode> tree;
	SHierModelNode hroot;
	hroot.draw.reset(new CDrawing(temptemp("wormhole_0", T4Double(1.0, 1.0, 1.0, 1.0))));
	hroot.trans = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, h*0.5));
	hroot.trans_s = tm;
	hroot.left_child = 1;
	tree.push_back(hroot);

	hroot.draw.reset(new CDrawing(temptemp("sphere_hole_0", T4Double(1.0, 1.0, 1.0, 1.0))));
	hroot.trans = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, h*0.5 + r));
	hroot.left_child = -1;
	hroot.trans_s = glm::mat4(1.0);
	tree.push_back(hroot);



	SFractal node;

	node.V_Draw.reset(new CHierModel(tree));
	node.V_Trans_S = glm::mat4(1.0);
	node.V_Trans_S = glm::scale(node.V_Trans_S, glm::vec3(1, 1, 1));

	float d = r*0.5;
	
	
	Vec3d v(1, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 t = glm::mat4(1.0);
	
		t = glm::translate(t, glm::vec3(0, 0, h + r));
		t = glm::rotate(t, float(DTR(i * 90)), glm::vec3(0, 0, 1));
		t = glm::rotate(t, float(-DTR(90)), glm::vec3(0, -1, 0));
		t = glm::translate(t, glm::vec3(0, 0, r));
		t = glm::scale(t, glm::vec3(s, s, s));
		node.V_Grow.push_back(t);
	}

	glm::mat4 t = glm::mat4(1.0);
	t = glm::translate(t, glm::vec3(0, 0, 2*r + h));
	t = glm::scale(t, glm::vec3(s, s, s));
	node.V_Grow.push_back(t);


	node.V_Initial.push_back(glm::scale(node.V_Trans_S, glm::vec3(1/ r, 1/ r, 1/ r)));

	V_Fractals["basic"] = shared_ptr<CFractal>(new CFractal(node));
}
void CGraphics::M_SetupHieraModels(void)
{
	M_SimplePolyFractal();

	auto p = CShaderManager::getInstance();
	V_Models.clear();
	for (auto k : p->M_GetPolygonList())
	{
		
		SDrawingInfo di;
		di.Global_Color = T4Double(1.0, 1.0, 1.0, 1.0);
		di.PolygonName = k.first;
		di.Program = di.PolygonName == "star" ?  "prg4" : "prg1";
		di.Line_Color = T4Double(0.0, 0.0, 0.0, 1.0);
		di.light = di.PolygonName == "star" ? true : false;

		auto& m = p->V_Polygon_suggested_mode;
		if (m.find(di.PolygonName) != m.end())
			di.DrawMode = m[di.PolygonName];
		else
			di.DrawMode = 2;

		SHierModelNode node;
		node.draw.reset(new CDrawing(di));
		V_Models[k.first] = shared_ptr<CHierModel>(new CHierModel(node));
	}

	set<string> meshes = p->V_Meshes;

	for (auto k : meshes)
	{
		CMesh M(k);
		V_Models[M.M_GetName()] = M.M_GetHierModel();
	}


	V_Models["sphere"]->M_RegisterTrans2(0, glm::scale(glm::mat4(1.0), glm::vec3(0.05, 0.05, 0.05)));


	return;
	//some manual modifications
	V_Models["gun"]->M_RegisterTrans2(0, glm::scale(glm::mat4(1.0), glm::vec3(4.0, 4.0, 4.0)));

	glm::mat4 trans;
	trans = glm::scale(glm::mat4(1.0), glm::vec3(20.0, 20.0, 20.0));
	trans = glm::translate(trans, glm::vec3(0, 0, -15));
	V_Models["skeleton"]->M_RegisterTrans2(0, trans);


}