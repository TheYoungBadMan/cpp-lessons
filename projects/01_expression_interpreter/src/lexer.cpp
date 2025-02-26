#include "lexer.hpp"

#include "token.hpp"

#include <string>
#include <vector>
#include <unordered_map>

#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input) {}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;

	while (1) {
		const auto& token = extract();
		tokens.push_back(token);
		if (token == TokenType::END) break;
	}

	return tokens;
}

Token Lexer::extract() {
	while (std::isblank(peek())) advance();

	if (index >= input.length()) {
		return {TokenType::END, ""};
	}

	if (std::isalpha(peek()) || peek() == '_' ) {
		return extract_id();
	}

	if (std::isdigit(peek())) {
		return extract_num();
	}

	if (metachars.contains(peek())) {
		return extract_op();
	}

	report("Unexpected char: " + peek());
}

Token Lexer::extract_id() {
	std::size_t size = 0;
	while (std::isalnum(peek(size)) || peek(size) == '_') ++size;
	const auto& value = std::string(input, index, size);
	advance(size);
	return {TokenType::ID, value};
}

Token Lexer::extract_num() {
	std::size_t size = 0;
	while (std::isdigit(peek(size))) ++size;
	if (peek(size) == '.') {
		++size;
		while (std::isdigit(peek(size))) ++size;
	}
	const auto& value = std::string(input, index, size);
	advance(size);
	return {TokenType::NUM, value};
}

Token Lexer::extract_op() {
	std::string op;
	std::size_t size = 0;

	while (metachars.contains(peek(size))) {
		op += peek(size);

		if (!operators.contains(op)) {
			op.pop_back();
			if (size == 0) {
				report("Invalid operator");
			}
			break;
		}

		++size;
	}
	advance(size);
	return {operators.at(op), op};
}

char Lexer::peek(std::size_t offset) const noexcept {
	return input[index + offset];
}

void Lexer::advance(std::size_t offset) noexcept {
	index += offset;
}

void Lexer::report(const std::string& message) const {
	throw std::runtime_error(message);
}

const std::string Lexer::metachars = "+-*/^,()";

const std::unordered_map<std::string, TokenType> Lexer::operators = {
	{"+", TokenType::PLUS}, {"-", TokenType::MINUS}, {"*", TokenType::STAR}, {"/", TokenType::SLASH}, {"^", TokenType::CARET},
	{",", TokenType::COMMA}, {"(", TokenType::LPAREN}, {")", TokenType::RPAREN}
};