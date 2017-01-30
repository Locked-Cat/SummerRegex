#pragma once

#include <memory>

#include "Common.h"
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
	private:
		AST ParseAlt();
		AST ParseAltBegin();
		AST ParseAltEnd();
		AST ParseConcat();
		AST ParseConcatBegin();
		AST ParseConcatEnd();
		AST ParseBase();

		void InsertEscape(Token& token, std::vector<char_t>& charRange);

		Tokenizer tokenizer;
		std::vector<Token> mTokens;
		std::vector<Token>::iterator mPos;

		Token GetToken();
		void MoveForward();
	};
}
