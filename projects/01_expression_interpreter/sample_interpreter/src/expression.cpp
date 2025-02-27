#include "expression.hpp"

#include "token.hpp"
#include "ast.hpp"

#include "lexer.hpp"
#include "parser.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

#include <cmath>
#include <numbers>

Expression::Expression(const std::string& input) : input(input) {
	Lexer lexer(input);
	const auto& tokens = lexer.tokenize();
	Parser parser(tokens);
	root = parser.parse();
}

std::string Expression::to_string() const noexcept {
	return to_string_helper(root);
}

double Expression::eval(const std::unordered_map<std::string, double>& vars,
						const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>>& funcs) const {
	return eval_helper(root, vars, funcs);
}

std::string Expression::to_string_helper(ASTNode *root) const noexcept {
	if (auto node = dynamic_cast<BinaryNode*>(root); node) {
		return to_string_helper(node->left) + node->op + to_string_helper(node->right);
	}

	if (auto node = dynamic_cast<UnaryNode*>(root); node) {
		return node->op + to_string_helper(node->base);
	}

	if (auto node = dynamic_cast<GroupNode*>(root); node) {
		return "(" + to_string_helper(node->base) + ")";
	}

	if (auto node = dynamic_cast<FuncNode*>(root); node) {
		auto func = node->id + "(";
		for (std::size_t i = 0; i < node->args.size(); ++i) {
			func += to_string_helper(node->args[i]);
			if (i < node->args.size() - 1) {
				func += ", ";
			}
		}
		return func + ")";
	}

	if (auto node = dynamic_cast<VarNode*>(root); node) {
		return node->id;
	}

	auto node = dynamic_cast<NumNode*>(root);
	return std::to_string(node->value);
}

double Expression::eval_helper(ASTNode *root, const std::unordered_map<std::string, double>& vars,
								const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>>& funcs) const {

	if (auto node = dynamic_cast<BinaryNode*>(root); node) {
		auto left = eval_helper(node->left, vars, funcs);
		auto right = eval_helper(node->right, vars, funcs);
		return binary_ops.at(node->op)(left, right);
	}

	if (auto node = dynamic_cast<UnaryNode*>(root); node) {
		auto base = eval_helper(node->base, vars, funcs);
		return unary_ops.at(node->op)(base);
	}

	if (auto node = dynamic_cast<GroupNode*>(root); node) {
		return eval_helper(node->base, vars, funcs);
	}

	if (auto node = dynamic_cast<FuncNode*>(root); node) {
		std::vector<double> args;
		for (auto arg : node->args) {
			args.push_back(eval_helper(arg, vars, funcs));
		}

		if (builtin_funcs.contains(node->id)) {
			return builtin_funcs.at(node->id)(args);
		}

		if (funcs.contains(node->id)) {
			return funcs.at(node->id)(args);
		}

		throw std::runtime_error("Undefined func id");
	}

	if (auto node = dynamic_cast<VarNode*>(root); node) {
		if (constants.contains(node->id)) {
			return constants.at(node->id);
		}

		if (vars.contains(node->id)) {
			return vars.at(node->id);
		}

		throw std::runtime_error("Undefined var id");
	}

	auto node = dynamic_cast<NumNode*>(root);
	return node->value;
}

const std::unordered_map<std::string, double> Expression::constants = {
	{"e", std::numbers::e},
	{"pi", std::numbers::pi}
};

const std::unordered_map<std::string, std::function<double(double)>> Expression::unary_ops  {
	{"+", [](double x) -> double {
		return x;
	}},
	{"-", [](double x) -> double {
		return -x;
	}}
};

const std::unordered_map<std::string, std::function<double(double, double)>> Expression::binary_ops = {
	{"+", [](double x, double y) -> double {
		return x + y;
	}},
	{"-", [](double x, double y) -> double {
		return x - y;
	}},
	{"*", [](double x, double y) -> double {
		return x * y;
	}},
	{"/", [](double x, double y) -> double {
		return x / y;
	}},
	{"^", [](double x, double y) -> double {
		return std::pow(x, y);
	}},
};

const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>> Expression::builtin_funcs = {
	{"sin", [](const std::vector<double>& args) -> double {
		if (args.size() != 1) {
			throw std::invalid_argument("Invalid number of arguments");
		}
		return std::sin(args[0]);
	}},
	{"cos", [](const std::vector<double>& args) -> double {
		if (args.size() != 1) {
			throw std::invalid_argument("Invalid number of arguments");
		}
		return std::cos(args[0]);
	}},
	{"log", [](const std::vector<double>& args) -> double {
		if (args.size() != 1) {
			throw std::invalid_argument("Invalid number of arguments");
		}
		return std::log(args[0]);
	}},
	{"exp", [](const std::vector<double>& args) -> double {
		if (args.size() != 1) {
			throw std::invalid_argument("Invalid number of arguments");
		}
		return std::exp(args[0]);
	}},
};