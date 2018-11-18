#pragma once
#include "CHandler.h"
#include "CShaderManager.h"
#include <stack>
#include "CDrawing.h"

struct SHierModelNode
{
	shared_ptr<CDrawing> draw;
	glm::mat4 trans;
	glm::mat4 trans_s; // no inheritance

	int port;

	int left_child;
	int right_sibling;

	vector<int> homos; // nodes that will be drawed in same time

	SHierModelNode() {
		trans = glm::mat4(1.0); trans_s = glm::mat4(1.0);  port = -1; left_child = -1; right_sibling = -1;}
};
class CHierModel :
	public CHandler
{
protected:
	GLuint V_Program;

	map<int, glm::mat4> V_Trans2; //transforms applied after hiera transform, according to port. (for animation)
	map<int, CHierModel*> V_Concat;
	
	stack<glm::mat4> V_MatrixStack;
	vector<SHierModelNode> V_Tree;
	void M_Release(void);

	void M_Draw_Rec(int index, glm::mat4 CTM);

	T4Double V_NewColor;

public:
	void M_RegisterTrans2(int port, glm::mat4 t);
	void M_ClearTrans2(void) { V_Trans2.clear(); }
	void M_Draw(glm::mat4 CTM, T4Double color = T4Double(1.0, 1.0, 1.0, 1.0));

	void M_ConcatHierModel(int index, CHierModel* c); // concat two hiermodel so that can be drawn together

	CHierModel(vector<SHierModelNode>& t) { V_Tree = t; }
	CHierModel(SHierModelNode& t) { V_Tree.push_back(t); }

	virtual ~CHierModel();
};

