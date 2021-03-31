#ifndef DEF
#define DEF 
#include <string>
#include "ast.h"
using namespace std;

enum nodeTypes{
	ROOT_NODE,
	FUNCTION_NODE,
	STATEMENT_NODE,
	UNARYOP_NODE,
	BINARYOP_NODE,
	CONSTEXP_NODE
};

void addDescendantNode(uintptr_t parent, uintptr_t descendant);
uintptr_t createNewNode(int Ntype, int NsubType =0, string Nvalue ="");
void setASTroot(uintptr_t root);
extern AST ast;
#endif
