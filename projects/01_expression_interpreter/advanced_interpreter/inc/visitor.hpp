#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <functional>

class Visitor {
public:
	virtual ~Visitor() noexcept = default;

	virtual void visit(class BinaryNode&) = 0;
	virtual void visit(class UnaryNode&) = 0;
	virtual void visit(class GroupNode&) = 0;
	virtual void visit(class FuncNode&) = 0;
	virtual void visit(class VarNode&) = 0;
	virtual void visit(class NumNode&) = 0;
};

class Stringifier : public Visitor {
public:
	std::string stringify(class ASTNode&);

	void visit(class BinaryNode&) override;
	void visit(class UnaryNode&) override;
	void visit(class GroupNode&) override;
	void visit(class FuncNode&) override;
	void visit(class VarNode&) override;
	void visit(class NumNode&) override;
private:
	std::string str = "";
};

class Printer : public Visitor {
public:
	void print(class ASTNode&) noexcept;

	void visit(class BinaryNode&) override;
	void visit(class UnaryNode&) override;
	void visit(class GroupNode&) override;
	void visit(class FuncNode&) override;
	void visit(class VarNode&) override;
	void visit(class NumNode&) override;
};

class Evaluator : public Visitor {
public:

	Evaluator(const std::unordered_map<std::string_view, double>& vars,
		const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>>& funcs) noexcept
		: vars(vars), funcs(funcs) {}

	double evaluate(class ASTNode&);

	void visit(class BinaryNode&) override;
	void visit(class UnaryNode&) override;
	void visit(class GroupNode&) override;
	void visit(class FuncNode&) override;
	void visit(class VarNode&) override;
	void visit(class NumNode&) override;
private:
	double result = 0.;

	const std::unordered_map<std::string_view, double>& vars;
	const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>>& funcs;

	static const std::function<void(const std::vector<double>&, std::size_t)> check_args;

	static const std::unordered_map<std::string_view, std::function<double(double, double)>> binary_ops;
	static const std::unordered_map<std::string_view, std::function<double(double)>> unary_ops;
	static const std::unordered_map<std::string_view, std::function<double(const std::vector<double>&)>> builtin_funcs;
	static const std::unordered_map<std::string_view, double> constants;
};