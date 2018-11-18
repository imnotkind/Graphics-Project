#include "CGeneral.h"


T4Int CGeneral::M_Pallete(int p)
{
	if (p == 0) return T4Int(214, 70, 128, 255);
	if (p == 1) return T4Int(28, 144, 145, 255);
	if (p == 2) return T4Int(100, 150, 23, 255);
	if (p == 3) return T4Int(106, 23, 231, 255);
	if (p == 4) return T4Int(83, 179, 73, 255);
	if (p == 5) return T4Int(33, 38, 228, 255);
	if (p == 6) return T4Int(186, 20, 0, 255);
	if (p == 7) return T4Int(89, 0, 100, 255);
	if (p == 8) return T4Int(24, 19, 86, 255);
	return T4Int(255, 0, 255, 255);
}
