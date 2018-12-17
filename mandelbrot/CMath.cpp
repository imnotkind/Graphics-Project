#include "CMath.h"



double CMath::M_Num_dRandom(double min, double max)
{
	uniform_real_distribution<double> dis(min, max);
	return dis(M_Random);
}
int CMath::M_Num_iRandom(int min, int max)
{
	uniform_int_distribution<int> dis(min, max);
	return dis(M_Random);
}
double CMath::M_Num_Gaussian(double mean, double sigma)
{
	normal_distribution<double> distribution(mean, sigma);
	return distribution(M_Random);

}
int CMath::M_SelectOne(vector<int> weights)
{
	int d = 0;
	for (int i = 0; i < weights.size(); i++)
	{
		d += weights[i];
	}
	int t = M_Num_iRandom(0, d - 1);
	d = 0;
	for (int i = 0; i < weights.size(); i++)
	{
		if (t >= d && t < d + weights[i]) return i;
		d += weights[i];
	}
}
int CMath::M_Num_Discretization(double src, double s, double e, int d)
{
	double R = (src - s) / (e - s) * d;
	return MC_R(R);

}
double CMath::M_Func_Sigmoid(double r)
{
	return 1.0 / (1 + pow(C_E, -r));
}

bool CMath::M_St_Frequency(double f)
{
	return M_Num_dRandom(0, 1) <= f;
}

T2Double CMath::M_2TV_Normalize(T2Double P)
{
	double d = sqrt(P[0] * P[0] + P[1] * P[1]);
	return P * (1 / d);
}
T2Double CMath::M_2TV_Angle(T2Double Start, T2Double End)
{
	End -= Start;
	return T2Double(atan2(End[1], End[0]), sqrt(End[0] * End[0] + End[1] * End[1]));
}
bool CMath::M_CircleRectCollisionTest(T2Double CirCen, double CirRad, T2Double RCen, T2Double RWH)
{
	T2Double l1, l2;
	RWH *= 0.5;
	l1 = T2Double(RCen[0] - RWH[0], RCen[0] + RWH[0]);
	l2 = T2Double(CirCen[0] - CirRad, CirCen[0] + CirRad);
	if (l1[1] < l2[0] || l1[0] > l2[1])
	{
		return false;
	}
	l1 = T2Double(RCen[1] - RWH[1], RCen[1] + RWH[1]);
	l2 = T2Double(CirCen[1] - CirRad, CirCen[1] + CirRad);
	if (l1[1] < l2[0] || l1[0] > l2[1])
	{
		return false;
	}
	return true;
}

bool CMath::M_2CirclesCollsionTest(T2Double c1, double r1, T2Double c2, double r2)
{
	T2Double x = M_2TV_Angle(c1, c2);
	if (x[1] < r1 + r2) return true;
	return false;
}


vector<int> CMath::M_Permutation(int n)
{
	//NOT IMPLEMENTED YET
	vector<int> r;
	r.resize(n, -1);
	for (int i = 0; i < n; i++)
	{
		while (true)
		{
			int x = M_Num_iRandom(0, n - 1);
			break;
		}
	
	}
	return r;
}