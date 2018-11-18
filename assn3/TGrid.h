#pragma once
#include "MainHeader.h"


using namespace std;

template<class T, int n>

class TGrid
{
	vector<T> arr;
	TVector<int, n> off;
	

public:
	TVector<int, n> size;

	void resize(TVector<int, n> s, T v = T())
	{
		int ff = 1;
		for (int i = 0; i < n; i++) ff *= s[i];
		arr.resize(ff, v);
		size = s;

		// making offset table
		int k = 1;
		for (int i = 0; i < n; i++)
		{
			off[i] = k;
			k *= size[i];
		}
	}
	TGrid(void)
	{
	}
	TGrid(const TGrid& x)
	{
		arr = x.arr;
		size = x.size;
		off = x.off;
	}
	void operator=(const TGrid& x)
	{
		arr = x.arr;
		size = x.size;
		off = x.off;
	}

	T& operator[](TVector<int, n> p)
	{
		int f = 0;
		for (int i = 0; i < n; i++)
		{
			f += off[i] * p[i];
		}
		return arr[f];
	}
	const T& operator[](TVector<int, n> p) const
	{
		int f = 0;
		for (int i = 0; i < n; i++)
		{
			f += off[i] * p[i];
		}
		return arr[f];
	}

	TVector<int, n> dir(int k) const
	{
		//only for 2d
		TVector<int, n> x(0, 0);
		if (k == 0) x[0] = -1;
		if (k == 1) x[1] = -1;
		if (k == 2) x[0] = 1;
		if (k == 3) x[1] = 1;

		return x;

		TVector<int, n> r(0);
		r[int(k / 2)] = (k % 2) * 2 - 1;
		return r;
	}




};