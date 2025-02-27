#pragma once

#include "ast.hpp"

#include <string>
#include <unordered_map>
#include <functional>

class Expression {
public:
	Expression(const std::string&);

	std::string to_string() const noexcept;
	double eval(const std::unordered_map<std::string, double>&,
				const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>>&) const;
private:
	std::string input;
	ASTNode *root;

	std::string to_string_helper(ASTNode *) const noexcept;
	double eval_helper(ASTNode *, const std::unordered_map<std::string, double>&,
				const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>>&) const;

	static const std::unordered_map<std::string, double> constants;
	static const std::unordered_map<std::string, std::function<double(double)>> unary_ops;
	static const std::unordered_map<std::string, std::function<double(double, double)>> binary_ops;
	static const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>> builtin_funcs;
};