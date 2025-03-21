#pragma once

#include "token.hpp"

#include <string_view>
#include <vector>
#include <unordered_map>

class Lexer {
public:
	Lexer(std::string_view input) noexcept : input(input) {}
	std::vector<Token> tokenize();
private:
	std::string_view input;
	std::size_t index = 0;

	Token extract();
	Token extract_id();
	Token extract_num();
	Token extract_op();

	char peek(std::size_t = 0) const noexcept;
	void advance(std::size_t = 1) noexcept;
	[[noreturn]] void report(std::string_view) const;
};