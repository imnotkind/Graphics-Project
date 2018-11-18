#pragma once
#include "CCharacter.h"
#include "CItem.h"
#include "CEnemy.h"
#include "TGrid.h"
#include "CHandler.h"

#include "CUserInput.h"
#include <fstream>

#include <set>
#include <list>

class CGraphics;

class CEngine : public CHandler
{
	//Object Indexing cahces
	vector<set<shared_ptr<CSomething>>::iterator> V_PBullets;
	vector<set<shared_ptr<CSomething>>::iterator> V_PEnemies;
	vector<set<shared_ptr<CSomething>>::iterator> V_PItems;


	int V_GameEnd = 0; // 0 : playing, 1 : gameover, 2: clear


	double V_Grid_Size;
	friend CGraphics;

	shared_ptr<CCharacter> V_Player;
	set <shared_ptr<CSomething>> V_Objects;
	
	void M_Defeat(void);

	T2Int M_GetEmptyPlace(void);
	void M_MoveRequest(T2Double d); //request for move character

	void M_ReadMap(string path);

	//map parameter
	int V_Max_Enemies;
	int V_Max_Items;
	TGrid<int, 2> V_Map;

	bool M_CheckWallCollision(shared_ptr<CSomething> p);

	void M_CollisionTest(void);
	void M_ListenMessages(void);
	bool M_Event_KeyPress(int key, bool special); //false : not mine

	void M_ObjectIndexing(void);
	void M_ItemState(void);

	void M_ItemUse(list<int>& x);

	void M_EnemyNavigation(void);

	T2Int M_DiscretePos(T2Double p);

	//item state
	double V_IS_Camera;
	double V_IS_Invincible;
	double V_IS_Speed;

	DWORD V_StartTick;
	DWORD V_CurrTick;

	int V_MaxTime;
	int V_LeftTime;

	int V_Life;

	double V_Animation_Temp;

	bool V_CrazyMod;

public:

	bool M_GameEnd(void) {return V_GameEnd != 0; }
	
	void M_Loop(void);
	void M_CheckKeyPress();
	void M_Initialize(void);
	

	CEngine();
	~CEngine();
};

