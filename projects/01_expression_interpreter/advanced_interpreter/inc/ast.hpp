#pragma once

#include <string>
#include <vector>
#include <memory>

struct ASTNode {
	virtual ~ASTNode() noexcept = default;
	virtual void accept(class Visitor&) = 0;
};

struct BinaryNode : ASTNode {
	std::string op;
	std::unique_ptr<ASTNode> left, right;

	BinaryNode(std::string_view op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
		: op(op), left(std::move(left)), right(std::move(right)) {}
	void accept(class Visitor&) override;
};

struct UnaryNode : ASTNode {
	std::string op;
	std::unique_ptr<ASTNode> base;

	UnaryNode(std::string_view op, std::unique_ptr<ASTNode> base)
		: op(op), base(std::move(base)) {}
	void accept(class Visitor&) override;
};

struct GroupNode : ASTNode {
	std::unique_ptr<ASTNode> base;

	GroupNode(std::unique_ptr<ASTNode> base) : base(std::move(base)) {}
	void accept(class Visitor&) override;
};

struct FuncNode : ASTNode {
	std::string id;
	std::vector<std::unique_ptr<ASTNode>> args;

	FuncNode(std::string_view id, std::vector<std::unique_ptr<ASTNode>>&& args)
		: id(id), args(std::move(args)) {}
	void accept(class Visitor&) override;
};

struct VarNode : ASTNode {
	std::string id;

	VarNode(std::string_view id) : id(id) {}
	void accept(class Visitor&) override;
};

struct NumNode : ASTNode {
	double value;

	NumNode(std::string_view value) : value(std::stod(std::string(value))) {}
	void accept(class Visitor&) override;
};