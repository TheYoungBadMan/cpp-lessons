#include "visitor.hpp"

#include "ast.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cmath>
#include <numbers>

double Evaluator::evaluate(ASTNode& node) {
	node.accept(*this);
	return result;
}

void Evaluator::visit(BinaryNode& node) {

	node.left->accept(*this);
	double left = result;
	node.right->accept(*this);
	double right = result;

	result = binary_ops.at(node.op)(left, right);
}

void Evaluator::visit(UnaryNode& node) {

	node.base->accept(*this);
	result = unary_ops.at(node.op)(result);
}

void Evaluator::visit(GroupNode& node) {
	node.base->accept(*this);
}

void Evaluator::visit(FuncNode& node) {
	
	std::vector<double> args;
	for (std::size_t i = 0; i < node.args.size(); ++i) {
		node.args[i]->accept(*this);
		args.push_back(result);
	}

	if (auto it = builtin_funcs.find(node.id); it != builtin_funcs.end()) {
		result = it->second(args);
	} else if (auto it = funcs.find(node.id); it != funcs.end()) {
		result = it->second(args);
	} else {
		throw std::runtime_error("Function not found");
	}
}

void Evaluator::visit(VarNode& node) {

	if (auto it = constants.find(node.id); it != constants.end()) {
		result = it->second;
	} else if (auto it = vars.find(node.id); it != vars.end()) {
		result = it->second;
	} else {
		throw std::runtime_error("Variable not found");
	}
}

void Evaluator::visit(NumNode& node) {
	result = node.value;
}

const std::function<void(const std::vector<double>& args, std::size_t n)> Evaluator::check_args = [](const std::vector<double>& args, std::size_t n) {
	if (args.size() != n) {
		throw std::runtime_error("Invalid number of arguments");
	}
};

const std::unordered_map<std::string_view, std::function<double(double, double)>> Evaluator::binary_ops = {
	{"+", std::plus<double>()},
	{"-", std::minus<double>()},
	{"*", std::multiplies<double>()},
	{"/", std::divides<double>()},
	{"^", [](double a, double b) -> double { return std::pow(a, b); }},
};

const std::unordered_map<std::string_view, std::function<double(double)>> Evaluator::unary_ops = {
	{"-", std::negate<double>()},
	{"+", std::identity()}
};

const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>> Evaluator::builtin_funcs = {
	{"sin", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::sin(args[0]); }},
	{"cos", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::cos(args[0]); }},
	{"tan", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::tan(args[0]); }},
	{"asin", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::asin(args[0]); }},
	{"acos", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::acos(args[0]); }},
	{"atan", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::atan(args[0]); }},
	{"log", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::log(args[0]); }},
	{"sqrt", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::sqrt(args[0]); }},
	{"exp", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::exp(args[0]); }},
	{"pow", [](const std::vector<double>& args) -> double { check_args(args, 2); return std::pow(args[0], args[1]); }},
	{"sgn", [](const std::vector<double>& args) -> double { check_args(args, 1); return args[0] < 0 ? -1 : (args[0] > 0 ? 1 : 0); }},
	{"abs", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::abs(args[0]); }},
	{"ceil", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::ceil(args[0]); }},
	{"floor", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::floor(args[0]); }},
	{"round", [](const std::vector<double>& args) -> double { check_args(args, 1); return std::round(args[0]); }},
};

const std::unordered_map<std::string_view, double> Evaluator::constants = {
	{"pi", std::numbers::pi},
	{"e", std::numbers::e}
};
