#pragma once

#include "token.hpp"
#include "ast.hpp"

#include <string>
#include <vector>

class Parser {
public:
	Parser(const std::vector<Token>&);
	ASTNode *parse();
private:
	std::vector<Token> tokens;
	std::size_t index = 0;

	ASTNode *parse_sum();
	ASTNode *parse_mul();
	ASTNode *parse_pow();
	ASTNode *parse_unary();
	ASTNode *parse_primary();
	ASTNode *parse_group();
	ASTNode *parse_func();

	Token current() const noexcept;
	Token prev() const noexcept;

	void advance() noexcept;

	bool check(TokenType) const noexcept;
	bool match(TokenType) noexcept;

	void consume(TokenType, const std::string&);

	[[noreturn]] void report(const std::string&) const;
};