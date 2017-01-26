#pragma once

#include <string>
#include <memory>

#include "FA.h"

namespace summer
{
	class Parser
	{
	public:
		using string_t = std::string;
		using char_t = string_t::value_type;
		using pos_t = string_t::const_iterator;

		Parser() {};
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;

		FA Parse(const string_t str);
	private:
		bool HasNext() const;
		char_t PeekNext() const;
		void MoveForward();

		//See Grammar.txt
		void S();
		void E();
		void EQuote();
		void T();
		void TQuote();
		FA C();
		int Number();

		string_t mStr;
		pos_t mPos;
		std::unique_ptr<FA> mFA;
	};
}
