#include "lexer.hpp"

#include "token.hpp"

#include <vector>
#include <unordered_map>

#include <stdexcept>

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;

	while (index < input.length()) {
		auto token = extract();
		tokens.push_back(token);
	}

	return tokens;
}

Token Lexer::extract() {
	while (std::isblank(peek())) advance();

	if (index >= input.length()) {
		return Token{TokenType::END};
	}

	if (std::isalpha(peek()) || peek() == '_' ) {
		return extract_id();
	}

	if (std::isdigit(peek())) {
		return extract_num();
	}

	return extract_op();
}

Token Lexer::extract_id() {
	auto start = index;
	while (std::isalnum(peek()) || peek() == '_') advance();
	auto value = input.substr(start, index - start);
	return {TokenType::ID, value};
}

Token Lexer::extract_num() {
	auto start = index;
	while (std::isdigit(peek())) advance();
	if (peek() == '.') {
		advance();
		while (std::isdigit(peek())) advance();
	}
	auto value = input.substr(start, index - start);
	return {TokenType::NUM, value};
}

Token Lexer::extract_op() {
	static const std::unordered_map<std::string_view, TokenType> ops = {
		{"+", TokenType::PLUS},
		{"-", TokenType::MINUS},
		{"*", TokenType::STAR},
		{"/", TokenType::SLASH},
		{"^", TokenType::CARET},
		{",", TokenType::COMMA},
		{"(", TokenType::LPAREN},
		{")", TokenType::RPAREN},
	};

	if (auto it = ops.find(input.substr(index, 1)); it != ops.end()) {
		advance();
		return {it->second, it->first};
	}

	report("Invalid character" + peek());
}

char Lexer::peek(std::size_t offset) const noexcept {
	return input[index + offset];
}

void Lexer::advance(std::size_t offset) noexcept {
	index += offset;
}

void Lexer::report(std::string_view message) const {
	throw std::runtime_error(std::string(message));
}