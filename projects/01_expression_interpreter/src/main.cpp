#include "expression.hpp"

#include <iostream>

int main() {
	std::string input = "2+3*4";
	Expression expr(input);
	std::cout << expr.to_string() << std::endl;
}