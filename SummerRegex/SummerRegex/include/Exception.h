#pragma once

#include <exception>
#include <cstdio>

class IllegalNumber
	: public std::exception
{
public:
	IllegalNumber(int pos)
		: std::exception("Illegal number!")
		, mPos(pos)
	{}

	const char* what() const override
	{
		char message[64];
		sprintf_s(message, "Illegal number at position: %d.", mPos);
		return message;
	}
private:
	int mPos;
};
