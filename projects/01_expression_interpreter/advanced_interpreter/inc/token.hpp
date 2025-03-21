#pragma once

#include <string_view>

enum class TokenType {
	ID, NUM,
	PLUS, MINUS, STAR, SLASH, CARET,
	COMMA, LPAREN, RPAREN,
	END
};

struct Token {
	TokenType type;
	std::string_view value;

	Token(TokenType type) noexcept
		: type(type), value("") {}

	Token(TokenType type, std::string_view value) noexcept
		: type(type), value(value) {}

	bool operator==(TokenType type) const noexcept {
		return this->type == type;
	}
};