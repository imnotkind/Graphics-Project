#pragma once
#include "MainHeader.h"

class StringHelper
{
public:
	static vector<string> M_split(const string& s, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(s);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	static string M_trim(string& s)
	{
		const char* delim = " \t\n\r\f\v";
		string rtrim =  s.erase(0, s.find_first_not_of(delim));
		string ltrim = s.erase(s.find_last_not_of(delim) + 1);
		return ltrim;
	}

};