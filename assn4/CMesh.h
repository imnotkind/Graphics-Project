#pragma once
#include "CHandler.h"
#include "CHierModel.h"

struct SMeshGroup
{
	set<int> group_members;
	int group_parent;
	glm::vec3 trans_origin;
	pair<float, glm::vec3> rotate_origin;
	glm::vec3 trans_parent;
	pair<float, glm::vec3> rotate_parent;
	int port;
	
};
typedef pair<int, vector<int>> treenode;
class CMesh :
	public CHandler
{
	T4Double V_LineColor;
	T4Double V_SurfaceColor;

	shared_ptr <CHierModel> V_Model;
	vector<SMeshGroup> V_Groups;

	string V_Name;

	void M_ConstructHierModel(void);
	void M_Rec_Construct(map<int, SHierModelNode>& all, vector<treenode>& treenodes,
		int root, int sibling, int off);
public:
	string M_GetName(void) { return V_Name; }
	CMesh(string meta);
	shared_ptr<CHierModel> M_GetHierModel(void) { return V_Model; }
	virtual ~CMesh() {}

};

