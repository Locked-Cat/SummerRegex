#pragma once

#include <vector>
#include <memory>
#include "Types.h"

namespace summer
{
	enum TokenType
	{
		CHAR,
		META,
		END
	};

	class Token
	{
	public:
		virtual TokenType GetType() const = 0;
		~Token() {}
	};

	template<typename T>
	typename T::value_type GetValue(const Token& token)
	{
		return dynamic_cast<const T&>(token).value;
	}


	class CharToken
		: public Token
	{
	public:
		using value_type = char_t;
		friend value_type GetValue<CharToken>(const Token& token);

		CharToken(char c)
			: value(c) {}

		virtual TokenType GetType() const override
		{
			return CHAR;
		}
	private:
		char_t value;
	};

	class MetaToken
		: public Token
	{
	public:
		using value_type = char_t;
		friend value_type GetValue<MetaToken>(const Token& token);

		MetaToken(char c)
			: value(c) {}

		virtual TokenType GetType() const override
		{
			return META;
		}
	private:
		char_t value;
	};

	class EndToken
		: public Token
	{
	public:
		EndToken() {}
		virtual TokenType GetType() const override
		{
			return END;
		}
	};

	class Tokenizer
	{
	public:
		Tokenizer() {}
		std::vector<std::shared_ptr<Token>> Tokenize(const string_t& str);
	};
}