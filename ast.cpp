#include "def.h"
#include "y.tab.h"

node :: node (int Ntype, int NsubType, string Nvalue )
{
	type = Ntype;
	subType = NsubType;
	value = Nvalue;
	descendants = vector<node *>();
}

void node :: addDescendant(node * descendant)
{
	descendants.push_back(descendant);
}

uintptr_t createNewNode(int Ntype, int NsubType, string Nvalue )
{
	node* newNode = new node(Ntype, NsubType, Nvalue);
	return (uintptr_t)newNode;
}

void addDescendantNode(uintptr_t parent, uintptr_t descendant)
{
	((node *)parent)->addDescendant((node *)descendant);
	return ;
}

AST::AST()
{
	rootNodeAST = NULL;
}


void AST:: setRootNode(node* root)
{
	rootNodeAST = root;
}

void AST:: visitDescendants(ofstream &fout, node* curNode)
{
	for (auto descendant : curNode->descendants)
		traverse(fout, descendant);
}

void handleUNARYOP_NODE(ofstream &fout, int subType)
{
	if (subType == MINUS)
	{
		fout << "neg %rax\n";
	}
	else if (subType == COMPLEMENT)
	{
		fout << "not %rax\n";
	}
	else
	{
		fout << "cmp $0, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "sete %al\n";
	}
	return;
}

// 1st operand is in %eax and 2nd operand is in %ecx
void handleBINARYOP_NODE(ofstream &fout, int subType)
{
	switch (subType)
	{
	case ADDITION:
		fout << "add %rcx, %rax\n";
		break;
	case MINUS:
		fout << "sub %rcx, %rax\n";
		break;
	//fout << "mov %rcx, %rax\n";
	case MULTIPLICATION:
		fout << "imul %rcx, %rax\n";
		break;
	case DIVISION:
		fout << "cqto\n";
		fout << "idivq %rcx\n";
		break;
	default:
		cout << "ERROR\n";
		exit(1);
	}
}

void AST:: traverse(ofstream &fout, node* curNode)
{
	if (curNode == NULL)
		return;

	cout << curNode->type << " " << (curNode->subType==DIVISION)<<curNode->value << "\n";
	switch (curNode->type)
	{
	case ROOT_NODE:
		fout << ".global _start\n";
		visitDescendants(fout, curNode);
		break;
	case FUNCTION_NODE:
		fout << "_start" << ":\n";
		visitDescendants(fout, curNode);
		break;
	case STATEMENT_NODE:
		visitDescendants(fout, curNode);
		fout << "mov %rax, %rdi\n";
		fout << "mov $60, %rax\n";
		fout << "syscall\n";
		break;
	case UNARYOP_NODE:
		visitDescendants(fout, curNode);
		handleUNARYOP_NODE(fout, curNode->subType);
		break;
	case BINARYOP_NODE:
		traverse(fout, curNode->descendants[1]);
		fout << "push %rax\n";
		traverse(fout, curNode->descendants[0]);
		fout << "pop %rcx\n";
		handleBINARYOP_NODE(fout, curNode->subType);
		break;
	case CONSTEXP_NODE:
		fout << "mov $" << curNode->value << ", %rax\n";
		break;
	default:
		cout << "ERROR\n";
		exit(1);
	}
	return ;
}


void setASTroot(uintptr_t root)
{
	//cout << "SETAST root " << ((node *)root)->type << " " << root << "\n";
	ast.setRootNode((node *)root);
	return ;
}

AST ast;
