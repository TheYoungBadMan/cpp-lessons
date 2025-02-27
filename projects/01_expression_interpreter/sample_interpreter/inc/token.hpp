#pragma once

#include <string>

enum class TokenType {
	ID, NUM,
	PLUS, MINUS, STAR, SLASH, CARET,
	COMMA, LPAREN, RPAREN,
	END
};

struct Token {
	TokenType type;
	std::string value;

	bool operator==(TokenType type) const noexcept {
		return this->type == type;
	}
};