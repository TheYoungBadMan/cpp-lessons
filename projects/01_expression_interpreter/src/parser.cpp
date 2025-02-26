#include "parser.hpp"

#include "token.hpp"
#include "ast.hpp"

#include <string>
#include <vector>

#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

ASTNode *Parser::parse() {
	return parse_sum();
}

ASTNode *Parser::parse_sum() {
	auto left = parse_mul();
	while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
		const auto& op = prev().value;
		auto right = parse_mul();
		left = new BinaryNode(op, left, right);
	}
	return left;
}

ASTNode *Parser::parse_mul() {
	auto left = parse_pow();
	while (match(TokenType::STAR) || match(TokenType::SLASH)) {
		const auto& op = prev().value;
		auto right = parse_pow();
		left = new BinaryNode(op, left, right);
	}
	return left;
}

ASTNode *Parser::parse_pow() {
	auto left = parse_unary();
	if (match(TokenType::CARET)) {
		const auto& op = prev().value;
		auto right = parse_pow();
		left = new BinaryNode(op, left, right);
	}
	return left;
}

ASTNode *Parser::parse_unary() {
	if (match(TokenType::PLUS) || match(TokenType::MINUS)) {
		const auto& op = prev().value;
		auto base = parse_unary();
		return new UnaryNode(op, base);
	}
	return parse_primary();
}

ASTNode *Parser::parse_primary() {
	if (match(TokenType::NUM)) {
		const auto& num = prev().value;
		return new NumNode(num);
	}

	if (match(TokenType::LPAREN)) {
		return parse_group();
	}

	if (match(TokenType::ID)) {
		return parse_func();
	}

	report("Unexpected token: " + current().value);
}

ASTNode *Parser::parse_group() {
	auto base = parse();
	consume(TokenType::RPAREN, "Expected ), got " + current().value);
	return new GroupNode(base);
}

ASTNode *Parser::parse_func() {
	const auto& id = prev().value;

	if (match(TokenType::LPAREN)) {
		std::vector<ASTNode*> args;
		if (!check(TokenType::RPAREN)) {
			do {
				auto arg = parse();
				args.push_back(arg);
			} while (!match(TokenType::COMMA));
		}
		consume(TokenType::RPAREN, "Expected ), got " + current().value);
		return new FuncNode(id, args);
	}

	return new VarNode(id);
}

inline Token Parser::current() const noexcept {
	return tokens[index];
}

inline Token Parser::prev() const noexcept {
	return tokens[index - 1];
}

inline void Parser::advance() noexcept {
	++index;
}

inline bool Parser::check(TokenType type) const noexcept {
	return current() == type;
}

inline bool Parser::match(TokenType type) noexcept {
	if (check(type)) {
		advance();
		return true;
	}
	return false;
}

inline void Parser::consume(TokenType type, const std::string& message) {
	if (!match(type)) {
		report(message);
	}
}

inline void Parser::report(const std::string& message) const {
	throw std::runtime_error(message);
}


