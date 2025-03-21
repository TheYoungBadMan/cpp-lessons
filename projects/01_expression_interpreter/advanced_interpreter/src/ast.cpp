#include "ast.hpp"
#include "visitor.hpp"

void BinaryNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void UnaryNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void GroupNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void FuncNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void VarNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void NumNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}
