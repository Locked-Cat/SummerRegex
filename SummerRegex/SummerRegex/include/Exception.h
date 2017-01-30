#pragma once

#include <exception>
#include <cstdio>

namespace summer
{
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
			static char message[64];
			sprintf_s(message, "Illegal number at position: %d.", mPos);
			return message;
		}
	private:
		int mPos;
	};

	class IllegalCharacter
		: public std::exception
	{
	public:
		IllegalCharacter(int pos)
			: std::exception("Illegal character!")
			, mPos(pos)
		{}

		const char* what() const override
		{
			static char message[64];
			sprintf_s(message, "Illegal character at position: %d.", mPos);
			return message;
		}
	private:
		int mPos;
	};

	class IllegalSyntax
		: public std::exception
	{
	public:
		IllegalSyntax(int pos)
			: std::exception("Illegal syntax!")
			, mPos(pos)
		{}

		const char* what() const override
		{
			static char message[64];
			sprintf_s(message, "Illegal syntax at position: %d.", mPos);
			return message;
		}
	private:
		int mPos;
	};

}
