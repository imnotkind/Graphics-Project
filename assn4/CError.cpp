#include "CError.h"



CError::CError(string message, bool critical)
{
	cout << "ERROR! : " << message << endl;
	if (critical)
	{
		Sleep(1000000);
	}
}