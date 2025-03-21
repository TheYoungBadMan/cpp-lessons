#pragma once

#include "token.hpp"
#include "ast.hpp"

#include <string>
#include <vector>
#include <memory>

class Parser {

public:
	Parser(std::vector<Token>&& tokens) noexcept : tokens(std::move(tokens)) {}
	std::unique_ptr<ASTNode> parse();
private:
	std::vector<Token> tokens;
	std::size_t index = 0;

	std::unique_ptr<ASTNode> parse_sum();
	std::unique_ptr<ASTNode> parse_mul();
	std::unique_ptr<ASTNode> parse_pow();
	std::unique_ptr<ASTNode> parse_unary();
	std::unique_ptr<ASTNode> parse_primary();
	std::unique_ptr<ASTNode> parse_group();
	std::unique_ptr<ASTNode> parse_func();

	Token current() const noexcept;
	Token previous() const noexcept;

	void advance() noexcept;
	
	template <typename... Args>
	bool match(Args... args) noexcept;

	void consume(TokenType, std::string_view);

	[[noreturn]] void report(std::string_view) const;
};