#include "expression.hpp"

#include "token.hpp"
#include "ast.hpp"

#include "lexer.hpp"
#include "parser.hpp"

#include "visitor.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

Expression::Expression(const std::string& input) : input(input), root(nullptr) {
	Lexer lexer(input);
	auto&& tokens = lexer.tokenize();
	Parser parser(std::move(tokens));
	root = parser.parse();
}

void Expression::print() const noexcept {
	Printer printer;
	printer.print(*root);
}

std::string Expression::to_string() const noexcept {
	Stringifier stringifier;
	return stringifier.stringify(*root);
}

double Expression::eval(const std::unordered_map<std::string_view, double>& vars,
	const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>>& funcs) const {
	
	Evaluator evaluator(vars, funcs);

	return evaluator.evaluate(*root);
}