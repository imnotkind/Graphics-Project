#include "CGraphics.h"



void CGraphics::M_SetupHieraModels(void)
{
	auto p = CShaderManager::getInstance();

	for (auto k : p->M_GetPolygonList())
	{
		
		SDrawingInfo di;

		di.PolygonName = k.first;
		di.Program = "prg1";

		auto& m = p->V_Polygon_suggested_mode;
		if (m.find(di.PolygonName) != m.end())
			di.DrawMode = m[di.PolygonName];
		else
			di.DrawMode = 2;

		SHierModelNode node;
		node.draw.reset(new CDrawing(di));
		V_Models[k.first] = shared_ptr<CHierModel>(new CHierModel(node));
	}
}