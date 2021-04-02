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
	sectionsCnt = 0;
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
		//LOGICAL_NEGATION
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
	case MULTIPLICATION:
		fout << "imul %rcx, %rax\n";
		break;
	case DIVISION:
		fout << "cqto\n";
		fout << "idivq %rcx\n";
		break;
	case EQUAL:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "sete %al\n";
		break;
	case NOTEQUAL:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "setne %al\n";
		break;
	case LESSTHAN:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "setl %al\n";
		break;
	case LESSTHANEQ:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "setle %al\n";
		break;
	case GREATERTHAN:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "setg %al\n";
		break;
	case GREATERTHANEQ:
		fout << "cmp %rcx, %rax\n";
		fout << "mov $0, %rax\n";
		fout << "setge %al\n";
		break;
	default:
		cout << "ERROR\n";
		exit(1);
	}
	return;
}

void handleDECLARE_ASSIGN_STATEMENT_NODE(ofstream &fout)
{
	fout << "push %rax \n";
	return;
}


void handleASSIGNMENTOP_NODE(ofstream &fout, int offset)
{
	fout << "mov %rax, " << offset << "(%rbp)\n";
	return;
}

void AST:: traverse(ofstream &fout, node* curNode)
{
	if (curNode == NULL)
		return;

	cout << curNode->type << " " << curNode->value << "\n";
	switch (curNode->type)
	{
	case ROOT_NODE:
		fout << ".global _start\n";
		fout << "_start:\n";
		fout << "call _main\n";
		fout << "mov %rax, %rdi\n";
		fout << "mov $60, %rax\n";
		fout << "syscall\n";
		visitDescendants(fout, curNode);
		break;
	case FUNCTION_NODE:
		fout << ".global _main\n";
		fout << "_main" << ":\n";
		fout << "push %rbp\n";
		fout << "mov %rsp, %rbp\n";
		visitDescendants(fout, curNode);
		break;
	case RETURN_STATEMENT_NODE:
		traverse(fout, curNode->descendants[0]);
		fout << "mov %rbp, %rsp\n";
		fout << "pop %rbp\n";
		fout << "ret\n";
		if ((curNode->descendants).size() > 1)
			traverse(fout, curNode->descendants[1]);
		break;
	case EXP_STATEMENT_NODE:
		visitDescendants(fout, curNode);
		break;
	case DECLARE_STATEMENT_NODE:
	{
		string symbolName = (curNode->descendants[0])->value;
		if (symTab.insertNewSymbol(symbolName, 8) == false)
		{
			cout << "ERROR symbol defined already \n";
			exit(1);
		}
		handleDECLARE_ASSIGN_STATEMENT_NODE(fout);
		if ((curNode->descendants).size() > 1)
			traverse(fout, curNode->descendants[1]);
		break;
	}
	case DECLARE_ASSIGN_STATEMENT_NODE:
	{
		string symbolName = (curNode->descendants[0])->value;
		if (symTab.insertNewSymbol(symbolName, 8) == false)
		{
			cout << "ERROR symbol defined already \n";
			exit(1);
		}
		traverse(fout, curNode->descendants[1]);
		handleDECLARE_ASSIGN_STATEMENT_NODE(fout);
		if ((curNode->descendants).size() > 2)
			traverse(fout, curNode->descendants[2]);
		break;
	}
	case ASSIGNMENTOP_NODE:
	{
		int offset = symTab.getVal((curNode->descendants[0])->value);
		cout << curNode->descendants[0]->value << " " << offset << "\n";
		if (offset == INT_MIN)
		{
			cout << "ERROR symbol not defined \n";
			exit(1);
		}
		cout << "B hai\n";
		traverse(fout, curNode->descendants[1]);
		cout << "B wapis\n";
		handleASSIGNMENTOP_NODE(fout, offset);
		break;
	}
	case UNARYOP_NODE:
		visitDescendants(fout, curNode);
		handleUNARYOP_NODE(fout, curNode->subType);
		break;
	case BINARYOP_NODE:
		if (curNode->subType == AND)
		{
			int sc = (++sectionsCnt);
			traverse(fout, curNode->descendants[0]);
			fout << "cmp $0, %rax\n";
			fout << "jne _expclause" << sc << "\n";
			fout << "jmp _end" << sc << "\n";
			fout << "_expclause" << sc << ":\n";
			traverse(fout, curNode->descendants[1]);
			fout << "cmp $0, %rax\n";
			fout << "mov $0, %rax\n";
			fout << "setne %al\n";
			fout << "_end" << sc << ":\n";

		}
		else if (curNode->subType == OR)
		{
			int sc = (++sectionsCnt);
			traverse(fout, curNode->descendants[0]);
			fout << "cmp $0, %rax\n";
			fout << "je _expclause" << sc << "\n";
			fout << "mov $1, %rax\n";
			fout << "jmp _end" << sc << "\n";
			fout << "_expclause" << sc << ":\n";
			traverse(fout, curNode->descendants[1]);
			fout << "cmp $0, %rax\n";
			fout << "mov $0, %rax\n";
			fout << "setne %al\n";
			fout << "_end" << sc << ":\n";
		}
		else
		{
			traverse(fout, curNode->descendants[1]);
			fout << "push %rax\n";
			traverse(fout, curNode->descendants[0]);
			fout << "pop %rcx\n";
			handleBINARYOP_NODE(fout, curNode->subType);
		}
		break;
	case CONSTEXP_NODE:
		fout << "mov $" << curNode->value << ", %rax\n";
		break;
	case IDENTIFIER_NODE:
	{
		int offset = symTab.getVal(curNode->value);
		if (offset == INT_MIN)
		{
			cout << "ERROR symbol not defined \n";
			exit(1);
		}
		fout << "mov " << offset << "(%rbp), %rax\n";
		break;
	}
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
