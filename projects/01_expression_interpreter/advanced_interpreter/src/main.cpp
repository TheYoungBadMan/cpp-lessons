#include "expression.hpp"

#include <iostream>

int main() {
	Expression expr("x+2 * f(x, y)");
	expr.print();
	
	std::cout << expr.to_string() << std::endl;

	std::unordered_map<std::string_view, double> values = {
		{"x", 3},
		{"y", 4}
	};

	std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>> functions = {
		{"f", [](const std::vector<double>& args) { return args[0] + args[1]; }}
	};

	std::cout << expr.eval(values, functions) << std::endl;

	return 0;
}