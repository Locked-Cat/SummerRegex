#pragma once

#include <memory>
#include <utility>
#include <set>
#include "Types.h"
#include "Tokenizer.h"
#include "AST.h"

namespace summer
{
	using AST = std::unique_ptr<ASTNode>;

	class Parser
	{
	public:
		Parser() {};
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;

		AST Parse(const string_t& str);

		static std::set<char_t> sLegalCharSet;
	private:
		AST ParseAlt();
		AST ParseConcat();
		AST ParseBase();
		AST ParseFactor();
		std::pair<int, int> ParseLoop();
		AST ParseCharSet();
		void ParseCharRange(std::set<char_t>& charSet);
		void ParseCharBase(std::set<char_t>& charSet);
		void ParseX(std::set<char_t>& charSet);
		char_t ParseChar();
		int ParseNumber();

		std::shared_ptr<Token> GetToken();

		Tokenizer tokenizer;
		std::vector<std::shared_ptr<Token>> mTokens;
		std::vector<std::shared_ptr<Token>>::iterator mPos;
	};
}
