#include "CEngine.h"



CEngine::CEngine()
{
}


CEngine::~CEngine()
{
}
void CEngine::M_ReadMap(string path)
{
	ifstream fs;
	fs.open(path);

	int w, h;
	fs >> w >> h >> V_Max_Items >> V_Max_Enemies;
	string s;
	V_Map.resize(T2Int(w, h), -1);
	for (int i = 0; i < h; i++)
	{
		fs >> s;
		if (s.size() != w) CError("Invliad map data format", true);
		for (int j = 0; j < w; j++)
		{
			V_Map[T2Int(j, i)] = s[j] - '0';
		}
	}
}
void CEngine::M_ListenMessages(void)
{
	//Script Messages
	auto mq = SMQueue::M_GetSingletone(0);
	while (!mq->M_Empty()) 
	{
		auto m = mq->M_Pop();
		if (m.type == "creation") // object creation request
		{
			V_Objects.insert(shared_ptr<CSomething>((CSomething*)m.content));
		}
	}

	//Input Messages;
	auto iq = SIQueue::M_GetSingletone(0);
	auto rq = SIQueue::M_GetSingletone(1);

	while (!iq->M_Empty())
	{
		auto m = iq->M_Pop();
		if (m.type == "down")
		{
			if (!M_Event_KeyPress(m.key, m.special)) rq->M_Push(m); //not mine!
		}
	}
	while (!rq->M_Empty())
	{
		iq->M_Push(rq->M_Pop());
	}

}
bool CEngine::M_CheckWallCollision(shared_ptr<CSomething> p)
{
	for (int x = 0; x < V_Map.size[0]; x++)
	{
		for (int y = 0; y < V_Map.size[1]; y++)
		{
			if (V_Map[T2Int(x, y)] == 1) // wall
			{
				T2Double cen = T2Double(x, y)*V_Grid_Size;
				T2Double wh = T2Double(V_Grid_Size, V_Grid_Size);

				if (V_Math->M_CircleRectCollisionTest(p->M_GetPosition(), p->M_GetRadius(), cen, wh)) 
				{
					return true;
				}
			}
		}
	}
	return false;
}
void CEngine::M_ObjectIndexing(void)
{
	V_PBullets.clear();
	V_PEnemies.clear();
	V_PItems.clear();

	for (auto p = V_Objects.begin(); p != V_Objects.end(); p++)
	{
		if (dynamic_cast<CBullet*> (p->get()) != NULL)
		{
			V_PBullets.emplace_back(p);
			continue;
		}
		if (dynamic_cast<CEnemy*> (p->get()) != NULL)
		{
			V_PEnemies.emplace_back(p);
			continue;
		}
		if (dynamic_cast<CItem*> (p->get()) != NULL)
		{
			V_PItems.emplace_back(p);
			continue;
		}
	}
}
void CEngine::M_CollisionTest(void)
{
	set<int> remove_list_bullet;
	set<int> remove_list_enemy;
	set<int> remove_list_item;

	//bullet - wall
	for (int i = 0; i < V_PBullets.size(); i++)
	{
		auto b = V_PBullets[i];
		if (M_CheckWallCollision(*b))
		{
			remove_list_bullet.insert(i);
		}
	}
	//bullet - enemy
	for (int i = 0; i < V_PBullets.size(); i++)
	{
		auto b = V_PBullets[i];
		for (int j = 0; j < V_PEnemies.size(); j++)
		{
			auto e = V_PEnemies[j];
			auto c1 = (*b)->M_GetPosition();
			auto c2 = (*e)->M_GetPosition();
			auto r1 = (*b)->M_GetRadius();
			auto r2 = (*e)->M_GetRadius();

			if (V_Math->M_2CirclesCollsionTest(c1, r1, c2, r2))
			{
				remove_list_enemy.insert(j);
				remove_list_bullet.insert(i);
			}
		}	
	}

	//item - character
	for (int i = 0; i < V_PItems.size(); i++)
	{
		auto t = V_PItems[i];
		auto c1 = V_Player->M_GetPosition();
		auto c2 = (*t)->M_GetPosition();
		auto r1 = V_Player->M_GetRadius();
		auto r2 = (*t)->M_GetRadius();

		if (V_Math->M_2CirclesCollsionTest(c1, r1, c2, r2))
		{
			V_Player->M_GetItemList().push_back((dynamic_cast<CItem*>(t->get()))->M_GetType());
			remove_list_item.insert(i);
		}
	}

	//enemy - character
	for (auto e : V_PEnemies)
	{
		if (V_IS_Invincible > 0) break; //yeah!

		auto c1 = V_Player->M_GetPosition();
		auto c2 = (*e)->M_GetPosition();
		auto r1 = V_Player->M_GetRadius();
		auto r2 = (*e)->M_GetRadius();

		if (V_Math->M_2CirclesCollsionTest(c1, r1, c2, r2))
		{
			V_Life--;
			V_IS_Invincible = 200;
			V_Player->M_GetInvincible(V_IS_Invincible);
			if (V_Life == 0)
			{
				M_Defeat();
				return;
			}
			
			
		}
	}

	if (!remove_list_enemy.empty()) //get angry
	{
		double factor = (double)V_PEnemies.size() / (double)V_Max_Enemies;
		if (V_PEnemies.size() < 10) factor -= 0.3;
		for (auto e : V_PEnemies)
		{
			dynamic_cast<CEnemy*>(e->get())->M_SetSpeed(V_Math->M_Num_dRandom(0.4, 0.6) + (1 - factor) * 0.5);
		}
	}

	for (auto i : remove_list_bullet) V_Objects.erase(V_PBullets[i]);
	for (auto i : remove_list_enemy) V_Objects.erase(V_PEnemies[i]);
	for (auto i : remove_list_item) V_Objects.erase(V_PItems[i]);

	

}
void CEngine::M_ItemState(void)
{
	if (V_IS_Camera > 0) V_IS_Camera -= 1.0;
	if (V_IS_Invincible > 0) V_IS_Invincible -= 1.0;
	if (V_IS_Speed > 0) V_IS_Speed -= 1.0;

}
void CEngine::M_Defeat(void)
{
	V_GameEnd = 1;

}
void CEngine::M_MoveRequest(T2Double d)
{
	V_Player->M_MoveFirst(d);

	if(M_CheckWallCollision(V_Player))
		V_Player->M_MoveFirst(d *(-1));
}
void CEngine::M_Loop(void)
{
	if (V_GameEnd)
	{
		auto iq = SIQueue::M_GetSingletone(0);
		if (!iq->M_Empty())
		{
			auto x = iq->M_Pop();
			if(x.type == "down" && x.key == 'r' && x.special == false)M_Initialize();
		}
		return;
	}

	V_CurrTick = GetTickCount();
	V_LeftTime = V_MaxTime - (V_CurrTick - V_StartTick) / 1000;

	if (V_LeftTime <= 0)
	{
		V_GameEnd = 1; return;
	}

	M_ObjectIndexing();
	M_ItemState();
	M_EnemyNavigation();

	if(V_Animation_Temp >= 0.0) V_Animation_Temp -= 1;
	double t = 1.0;

	for (auto p : V_Objects)
	{
		p->M_Loop(t);
	}
	V_Player->M_Loop(t);
	M_CollisionTest();
	
	M_ListenMessages();
	M_CheckKeyPress();

}
void CEngine::M_CheckKeyPress()
{
	auto iq = CUserInput::getInstance();
	double speed = 0.1;

	if (V_IS_Speed > 0.0) speed += sin(V_IS_Speed / 240.0 * PI) *0.15;

	if (iq->M_IfPressed('d', false))
	{
		V_Player->M_Rotate(-0.03);
	}
	if (iq->M_IfPressed('a', false))
	{
		V_Player->M_Rotate(0.03);
	}
	if (iq->M_IfPressed('s', false))
	{
		//M_MoveRequest(T2Double(-V_Grid_Size * speed, 0));
	}
	if (iq->M_IfPressed('w', false))
	{
		M_MoveRequest(T2Double(V_Grid_Size * speed, 0));
	}
}
void CEngine::M_ItemUse(list<int>& x)
{
	if (x.empty()) return;

	int z = x.front();
	x.pop_front();

	if (z == 0) // Mega fire
	{
		V_Player->M_MegaFire();
	}
	if (z == 1) // Camera up
	{
		V_IS_Camera = 600;
	}
	if (z == 2) // Invincible
	{
		V_IS_Invincible = 200;
		V_Player->M_GetInvincible(200);
	}
	if (z == 3) // Sppeed up
	{
		V_IS_Speed = 60 * 4;
	}
	if (z == 4) // SuperFire
	{
		V_Player->M_SuperFire();
	}
}
bool CEngine::M_Event_KeyPress(int key, bool special)
{

	if (key == KEY_LEFT_CLICK && special)
	{
		V_Animation_Temp = 30;
		V_Player->M_Fire(); // Space bar
		return true;
	}
	if (key == 32 && !special)
	{
		M_ItemUse(V_Player->M_GetItemList());
		return true;
	}
	return false;
}
T2Int CEngine::M_GetEmptyPlace(void)
{
	while (true) // random try
	{
		int x = V_Math->M_Num_iRandom(10, V_Map.size[0] - 1);
		int y = V_Math->M_Num_iRandom(10, V_Map.size[1] - 1);

		if (V_Map[T2Int(x, y)] == 0)
		{
			return(T2Int(x, y));
		}
	}
}



void CEngine::M_Initialize(void)
{
	auto mq = SMQueue::M_GetSingletone(0);
	while (!mq->M_Empty()) { mq->M_Pop(); }

	V_GameEnd = 0;
	V_Objects.clear();

	V_CrazyMod = false;
	V_Life = 3;
	V_Animation_Temp = 0.0;

	V_StartTick = GetTickCount();
	V_CurrTick = V_StartTick;
	V_MaxTime = 500;

	V_IS_Camera = 0.0;
	V_IS_Invincible = -1.0;
	V_IS_Speed = -1.0;

	V_Grid_Size = 6.0;
	
	M_ReadMap("resource/map.txt");

	int n_enm = V_Max_Enemies;
	int n_itm = V_Max_Items;

	V_Player = shared_ptr<CCharacter>(new CCharacter(T2Double(1, 1) * V_Grid_Size, 0, T4Int(255, 255, 255, 255), V_Grid_Size * 0.3));

	//place items
	for (int i = 0; i < n_itm; i++)
	{
		auto p = M_GetEmptyPlace();
		V_Map[p] = 2;
		vector<int> pr; pr.push_back(3); pr.push_back(1); pr.push_back(2); pr.push_back(2); pr.push_back(1);
		int type = V_Math->M_SelectOne(pr);
		auto q = V_Objects.insert(shared_ptr<CItem>(new CItem(T2Double(p[0], p[1]) * V_Grid_Size, 3+type, T4Int(255, 255, 255, 255), V_Grid_Size * 0.3, type)));
	}


	//place enemies
	for (int i = 0; i < n_enm; i++)
	{
		auto p = M_GetEmptyPlace();
		vector<int> pr; pr.push_back(5); pr.push_back(3);
		int pick = V_Math->M_SelectOne(pr);
		int type;
		if (pick == 0)
			type = 1;
		else
			type = 8;
		auto q = V_Objects.insert(shared_ptr<CEnemy>(new CEnemy(T2Double(p[0], p[1]) * V_Grid_Size, type, T4Int(255, 255, 255, 255), V_Grid_Size * 0.3, V_Math->M_Num_dRandom(0.4, 0.6))));
		//p.first->get()->~~ : some initialization
	}

}

T2Int CEngine::M_DiscretePos(T2Double p)
{
	p *= (1.0 / V_Grid_Size);
	return T2Int(int(p[0] + 0.5), int(p[1] + 0.5));
}
typedef pair<int, T2Int> mp;
bool mycomp(const mp& a, const mp& b)
{
	return a.first > b.first;
}

void CEngine::M_EnemyNavigation(void)
{
	static int count = 0;
	count++;
	//if (count < 1) return;
	count = 0;

	int off = 0;

	T2Int q = M_DiscretePos(V_Player->M_GetPosition());

	double factor = (double)V_PEnemies.size() / (double)V_Max_Enemies; //fewer enemy leads to more aggresiveness

	if (V_PEnemies.size() == 0)
	{
		V_GameEnd = 2; return; //Clear!
	}
	for (auto e : V_PEnemies) 
	{
		if (V_Math->M_St_Frequency(0.9)) continue;

		off = V_Math->M_Num_iRandom(0, 3);
		auto enemy = dynamic_cast<CEnemy*>(e->get());
		

		TGrid<int, 2> dis, path, check;
		dis.resize(V_Map.size, 1000000);
		path.resize(V_Map.size, -1);
		check.resize(V_Map.size, 0);

		T2Int p = M_DiscretePos(enemy->M_GetPosition());

		if (p == q) continue;

		T2Int diff = p - q;

		double detection = 5 + log(2 - factor) * 60;
		if (diff[0] * diff[0] + diff[1] * diff[1] > detection * detection ) //too far to detect player
		{
			if (enemy->M_MoveEmpty())
			{
				for (int d = off; d < 4 + off; d++) //for variety of path, order of visiting is random
				{
					T2Int np = p + dis.dir(d % 4);
					if (V_Map[np] != 1)
					{
						T2Double temp(np[0], np[1]);
						temp *= V_Grid_Size;
						enemy->M_PushMove(temp);
						break;
					}
				}
			}			
		}
		else
		{
			if (V_Map[p] == 1 || V_Map[q] == 1) continue;

			enemy->M_ClearMove();
			

			priority_queue<mp, vector<mp>, function<bool(mp, mp)>> pq(mycomp);
			pq.push(mp(0, q));

			while (true)//DIJKSTRA!
			{
				if (pq.empty()) break;
				auto x = pq.top(); pq.pop();

				if (x.second == p) break;
				if (check[x.second]) continue;

				dis[x.second] = x.first;
				check[x.second] = 1;

				for (int d = off; d < 4 + off; d++) //for variety of path, order of visiting is random
				{
					int dd = d % 4;
					T2Int np = x.second + dis.dir(dd);
					if (V_Map[np] != 1)
					{
						if (dis[np] > dis[x.second] + 1)
						{
							pq.push(mp(dis[x.second] + 1, np));
							path[np] = (dd + 2) % 4;
							dis[np] = dis[x.second] + 1;
						}
					}
				}
			}

			T2Int Cur = p;

			while (true) //recover path
			{
				Cur = Cur + dis.dir(path[Cur]);

				T2Double temp(Cur[0], Cur[1]);
				temp *= V_Grid_Size;
				enemy->M_PushMove(temp);

				if (Cur == q) break;
			}
		}
	}
}