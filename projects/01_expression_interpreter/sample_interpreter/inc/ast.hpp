#pragma once

#include <string>
#include <vector>

struct ASTNode {
	virtual ~ASTNode() noexcept = default;
};

struct BinaryNode : ASTNode {
	std::string op;
	ASTNode *left, *right;

	BinaryNode(const std::string& op, ASTNode *left, ASTNode *right) : op(op), left(left), right(right) {}
};

struct UnaryNode : ASTNode {
	std::string op;
	ASTNode *base;

	UnaryNode(const std::string& op, ASTNode *base) : op(op), base(base) {}
};

struct GroupNode : ASTNode {
	ASTNode *base;

	GroupNode(ASTNode *base) : base(base) {}
};

struct FuncNode : ASTNode {
	std::string id;
	std::vector<ASTNode*> args;

	FuncNode(const std::string& id, const std::vector<ASTNode*>& args) : id(id), args(args) {}
};

struct VarNode : ASTNode {
	std::string id;

	VarNode(const std::string& id) : id(id) {}
};

struct NumNode : ASTNode {
	double value;

	NumNode(const std::string& value) : value(std::stod(value)) {}
};