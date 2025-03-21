#include "visitor.hpp"

#include "ast.hpp"

#include <iostream>

void Printer::print(ASTNode& node) noexcept {
	node.accept(*this);
	std::cout << std::endl;
}

void Printer::visit(BinaryNode& node) {
	node.left->accept(*this);
	std::cout << node.op;
	node.right->accept(*this);
}

void Printer::visit(UnaryNode& node) {
	std::cout << node.op;
	node.base->accept(*this);
}

void Printer::visit(GroupNode& node) {
	std::cout << "(";
	node.base->accept(*this);
	std::cout << ")";
}

void Printer::visit(FuncNode& node) {
	std::cout << node.id << "(";
	for (std::size_t i = 0; i < node.args.size(); ++i) {
		node.args[i]->accept(*this);
		if (i != node.args.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << ")";
}

void Printer::visit(VarNode& node) {
	std::cout << node.id;
}

void Printer::visit(NumNode& node) {
	std::cout << node.value;
}