#include "CGraphics.h"


SDrawingInfo temptemp(string s, T4Double c)
{
	SDrawingInfo d;
	d.DrawMode = 3;
	d.Global_Color = c;
	d.PolygonName = s;
	d.Program = "prg1";
	d.Line_Color.set(1.0, 0.0, 0.0, 1.0);
	d.light = false;

	return d;
}
void CGraphics::M_SimplePolyFractal(void)
{
	V_Fractals;
	SFractal node;

	node.V_Draw.reset(new CDrawing(temptemp("cubobj", T4Double(1.0, 0.0, 0.0, 1.0))));
	
	for (int i = 0; i < 3; i++)
	{
		for (int y = -1; y <= 1; y += 2)
		{
			Vec3d off;
			off[i] = y;
			node.draw.reset(new CDrawing(temptemp("cubobj", T4Double(1.0, 0.0, 0.0, 1.0))));
			node.
		}
	}
}
void CGraphics::M_SetupHieraModels(void)
{
	auto p = CShaderManager::getInstance();
	V_Models.clear();
	for (auto k : p->M_GetPolygonList())
	{
		
		SDrawingInfo di;
		di.Global_Color = T4Double(1.0, 1.0, 1.0, 1.0);
		di.PolygonName = k.first;
		di.Program = di.PolygonName == "cube1" ?  "prg2" : "prg1";
		di.Line_Color = T4Double(0.0, 0.0, 0.0, 1.0);
		di.light = false;

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

	//some manual modifications
	V_Models["gun"]->M_RegisterTrans2(0, glm::scale(glm::mat4(1.0), glm::vec3(4.0, 4.0, 4.0)));
	
	V_Models["sphere"]->M_RegisterTrans2(0, glm::scale(glm::mat4(1.0), glm::vec3(0.05, 0.05, 0.05)));
	V_Models["man"]->M_ConcatHierModel(4, V_Models["gun"].get());

	glm::mat4 trans;
	trans = glm::scale(glm::mat4(1.0), glm::vec3(20.0, 20.0, 20.0));
	trans = glm::translate(trans, glm::vec3(0, 0, -15));
	V_Models["skeleton"]->M_RegisterTrans2(0, trans);


}