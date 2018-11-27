#pragma once

#include "TVector.h"
#include "MainHeader.h"

class T2Double : public TVector<double, 2>
{
public:
	T2Double(void) {}
	T2Double(double a, double b) { (*this)[0] = a, (*this)[1] = b; }
	T2Double(const T2Double& x) { TVector<double, 2>::operator=(x); }
	T2Double(const TVector<double, 2>& x) { TVector<double, 2>::operator=(x); }
	T2Double(const Vec2d& x) { (*this)[0] = x[0], (*this)[1] = x[1]; }
	void operator=(const T2Double& x) { TVector<double, 2>::operator=(x); }
	void set(double a, double b) { (*this)[0] = a, (*this)[1] = b; }
	Vec3d convert_gl(void) { return Vec3d((*this)[0], (*this)[1], 0.0); }
};