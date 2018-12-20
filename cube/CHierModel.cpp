#include "CHierModel.h"



CHierModel::~CHierModel()
{
	M_Release();
}

void CHierModel::M_Release(void)
{

}
void CHierModel::M_RegisterTrans2(int port, glm::mat4 t)
{
	V_Trans2[port] = t;
}
void CHierModel::M_ConcatHierModel(int index, CHierModel* c)
{
	V_Concat[index] = c;
}
void CHierModel::M_Draw_Rec(int index, glm::mat4 CTM)
{
	SRenderInfo tri = V_RenderInfo;

	V_MatrixStack.push(CTM);
	SHierModelNode& node = V_Tree[index];

	//not exist -> identity
	glm::mat4 trans2 = V_Trans2.find(node.port) == V_Trans2.end() ? glm::mat4(1.0f) : V_Trans2[node.port];
	CTM = CTM * node.trans * trans2;
	auto temp = CTM * node.trans_s;

	tri.model = temp;
	tri.normtrans = tri.model;

	node.draw->M_Draw(tri);
	for (auto h : node.homos)
		V_Tree[h].draw->M_Draw(tri);

	tri.model = CTM;
	tri.normtrans = tri.model;

	auto i = V_Concat.find(index); //draw concated model
	if (i != V_Concat.end() && i->second != NULL)
		i->second->M_Draw(tri);

	if (node.left_child != -1) M_Draw_Rec(node.left_child, CTM);
	CTM = V_MatrixStack.top();  V_MatrixStack.pop();

	tri.model = CTM;
	tri.normtrans = tri.model;

	if (node.right_sibling != -1) M_Draw_Rec(node.right_sibling, CTM);
}

void CHierModel::M_Draw(const SRenderInfo& r)
{
	auto s = CShaderManager::getInstance();
	V_Program = s->M_GetProgram();
	V_RenderInfo = r;

	M_Draw_Rec(0, r.model); // 0 is root
}

