#pragma once
#include "MainHeader.h"
#include <cmath>
#include <random>
#include "TVector_speical.h"
#define C_E (2.7182818284)

class CMath
{
	mt19937 M_Random;
	CMath() { Instance = NULL; M_Random.seed((unsigned int)time(NULL)); }
	~CMath() { freeInstance(); }
	static CMath* Instance;

public:
	static CMath* getInstance(void)
	{
		if (Instance == NULL)Instance = new CMath;
		return Instance;
	}
	static void freeInstance(void)
	{
		if (Instance) delete Instance;
	}
	//
	double M_Num_dRandom(double min, double max);
	double M_Num_Gaussian(double mean, double sigma);
	int M_Num_iRandom(int min, int max);
	int M_Num_Discretization(double src, double s, double e, int d);

	int M_SelectOne(vector<int> weights);
	//
	double M_Func_Sigmoid(double r);
	//
	bool M_St_Frequency(double f); //decide 0 or 1 randomly


	T2Double M_2TV_Normalize(T2Double P);
	T2Double M_2TV_Angle(T2Double Start, T2Double End); ///angle and distance

	bool M_CircleRectCollisionTest(T2Double CirCen, double CirRad, T2Double RCen, T2Double RWH );
	bool M_2CirclesCollsionTest(T2Double c1, double r1, T2Double c2, double r2);

	vector<int> M_Permutation(int n);
};



