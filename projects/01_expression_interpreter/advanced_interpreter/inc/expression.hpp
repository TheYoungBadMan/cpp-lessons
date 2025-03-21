#pragma once

#include "ast.hpp"

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

class Expression {
public:
	Expression(const std::string&);

	void print() const noexcept;
	std::string to_string() const noexcept;
	double eval(const std::unordered_map<std::string_view, double>&,
				const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>>&) const;
private:
	std::string input;
	std::unique_ptr<ASTNode> root;
};