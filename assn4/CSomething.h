#pragma once
#include "MainHeader.h"
#include "CMath.h"
#include "MessageQueue.h"

struct SScriptMessage
{
	int from;
	string type;
	void* content;
};
typedef CMessageQueue<SScriptMessage> SMQueue;

struct SDrawData
{
	T2Double pos;
	int img; //0:player, 1:enemy, 2:fire, 3:item_megafire, 4:item_cameraup, 5:item_invincible, 6:item_speedup
	T4Int color;
	double rotate;
	double size;
};


class CSomething
{
protected:
	T2Double V_Position;
	int V_Image;
	T4Int V_Color;

	double V_Rotate;
	double V_Radius;

public:
	virtual void M_Loop(double t) {}

	void M_Move(T2Double d);

	T2Double M_GetPosition(void) { return V_Position; }
	double M_GetRadius(void) { return V_Radius; }

	SDrawData M_GetDrawData(void);

	CSomething(T2Double p, int i, T4Int c, double r, double rot) : V_Position(p), V_Image(i), V_Color(c), V_Radius(r), V_Rotate(rot) { }

	~CSomething();
};

