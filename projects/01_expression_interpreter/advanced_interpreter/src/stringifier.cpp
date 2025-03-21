#include "visitor.hpp"

#include "ast.hpp"

#include <string>

std::string Stringifier::stringify(ASTNode& root) {
	root.accept(*this);
	return str;
}

void Stringifier::visit(BinaryNode& node) {
	node.left->accept(*this);
	auto left = str;
	node.right->accept(*this);

	str = left + node.op + str;
}

void Stringifier::visit(UnaryNode& node) {
	node.base->accept(*this);
	str = node.op + str;
}

void Stringifier::visit(GroupNode& node) {
	node.base->accept(*this);
	str = "(" + str + ")";
}

void Stringifier::visit(FuncNode& node) {
	auto func = node.id + "(";
	for (std::size_t i = 0; i < node.args.size(); ++i) {
		node.args[i]->accept(*this);
		func += str;
		if (i != node.args.size() - 1) {
			func += ", ";
		}
	}
	str = func + ")";
}

void Stringifier::visit(VarNode& node) {
	str = node.id;
}

void Stringifier::visit(NumNode& node) {
	str = std::to_string(node.value);
}

