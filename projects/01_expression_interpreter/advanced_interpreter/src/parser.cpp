#include "parser.hpp"

#include "token.hpp"
#include "ast.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>

std::unique_ptr<ASTNode> Parser::parse() {
	return parse_sum();
}

std::unique_ptr<ASTNode> Parser::parse_sum() {
	auto left = parse_mul();
	while (match(TokenType::PLUS, TokenType::MINUS)) {
		auto op = previous().value;
		auto right = parse_mul();
		left = std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<ASTNode> Parser::parse_mul() {
	auto left = parse_pow();
	while (match(TokenType::STAR, TokenType::SLASH)) {
		auto op = previous().value;
		auto right = parse_pow();
		left = std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<ASTNode> Parser::parse_pow() {
	auto left = parse_unary();
	if (match(TokenType::CARET)) {
		auto op = previous().value;
		auto right = parse_pow();
		left = std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<ASTNode> Parser::parse_unary() {
	if (match(TokenType::PLUS, TokenType::MINUS)) {
		auto op = previous().value;
		auto base = parse_unary();
		return std::make_unique<UnaryNode>(op, std::move(base));
	}
	return parse_primary();
}

std::unique_ptr<ASTNode> Parser::parse_primary() {
	if (match(TokenType::NUM)) {
		auto value = previous().value;
		return std::make_unique<NumNode>(value);
	}

	if (match(TokenType::LPAREN)) {
		return parse_group();
	}

	if (match(TokenType::ID)) {
		return parse_func();
	}

	report("Unexpected token: " + std::string(current().value));
}

std::unique_ptr<ASTNode> Parser::parse_group() {
	auto base = parse();
	consume(TokenType::RPAREN, "Expected ), got " + std::string(current().value));
	return std::make_unique<GroupNode>(std::move(base));
}

std::unique_ptr<ASTNode> Parser::parse_func() {
	auto id = previous().value;

	if (match(TokenType::LPAREN)) {
		std::vector<std::unique_ptr<ASTNode>> args;
		if (!match(TokenType::RPAREN)) {
			do {
				auto arg = parse();
				args.push_back(std::move(arg));
			} while (match(TokenType::COMMA));
			consume(TokenType::RPAREN, "Expected ), got " + std::string(current().value));
		}
		return std::make_unique<FuncNode>(id, std::move(args));
	}

	return std::make_unique<VarNode>(id);
}

inline Token Parser::current() const noexcept {
	return tokens[index];
}

inline Token Parser::previous() const noexcept {
	return tokens[index - 1];
}

inline void Parser::advance() noexcept {
	++index;
}

template <typename... Args>
inline bool Parser::match(Args... args) noexcept {
	if (((current().type == args) || ...)) {
		advance();
		return true;
	}
	return false;
}

inline void Parser::consume(TokenType type, std::string_view message) {
	if (!match(type)) {
		report(message);
	}
}

inline void Parser::report(std::string_view message) const {
	throw std::runtime_error(std::string(message));
}


