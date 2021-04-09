#include "def.h"
#include "y.tab.h"

node :: node (int Ntype, int NsubType, string Nvalue )
{
	type = Ntype;
	subType = NsubType;
	value = Nvalue;
	descendants = vector<node *>();
	elseNode = NULL;
}

void node :: addDescendant(node * descendant)
{
	//cout<<type<<" "<<descendant<<"\n";
	descendants.push_back(descendant);
	return;
}

void node :: addBlockDescendant(node *descendant)
{
	blockDescendantList.push_back(descendant);
	return;
}

uintptr_t createNewNode(int Ntype, int NsubType, string Nvalue )
{
	node* newNode = new node(Ntype, NsubType, Nvalue);
	return (uintptr_t)newNode;
}

void addDescendantNode(uintptr_t parent, uintptr_t descendant)
{
	//cout<<parent<<" "<<descendant<<"\n";
	((node *)parent)->addDescendant((node *)descendant);
	return ;
}

void addBlockDescendantNode(uintptr_t blockNode, uintptr_t descendant)
{
	((node *)blockNode)->addBlockDescendant((node *)descendant);
	return;
}

void setElseNode(uintptr_t parent, uintptr_t descendant)
{
	((node *)parent)->elseNode = (node *)descendant;
}

AST::AST()
{
	depth = 0;
	sectionsCnt = 0;
	ifstmCnt = 0;
	whileCnt = 0;
	forCnt = 0;
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

void setFuncOuterBlock(uintptr_t curNode)
{
	((node *)curNode)->isFuncOuterBlock = true;
}

void AST:: traverse(ofstream &fout, node* curNode)
{
	if (curNode == NULL)
		return;

	//cout << curNode->type << " " << (uintptr_t)curNode << " &&&&&&&& ";
	//for (auto it : curNode->descendants)
	//cout << (uintptr_t)it << " ";
	//cout << "\n";
	//visitDescendants(fout,curNode);
	//return ;
	//cout << curNode->type << " " << curNode->value << "\n";
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
	case BLOCK_NODE:
	{
		cout << "block open\n";
		symTab.incrNestLevel();
		for (auto descendant : curNode->blockDescendantList)
		{
			cout << "*" << descendant << "\n";
			traverse(fout, descendant);
		}
		int g = symTab.decrNestLevel();
		if (g && !curNode->isFuncOuterBlock)
			fout << "add $" << g << ", %rsp\n";
		cout << "block closed " << g << "\n";
		break;
	}
	case WHILE_NODE:
	{
		int curWhileCnt = whileCnt++;
		int g = 0;
		symTab.incrNestLevel();
		fout << "_while" << curWhileCnt << ":\n";
		traverse(fout, curNode->descendants[0]);
		fout << "cmp $1, %rax\n";
		fout << "jne _postwhile" << curWhileCnt << "\n";
		traverse(fout, curNode->descendants[1]);
		fout << "jmp _while" << curWhileCnt << "\n";

		fout << "_postwhile" << curWhileCnt << ":\n";

		g = symTab.decrNestLevel();
		if (g)
			fout << "add $" << g << ", %rsp\n";

		break;
	}
	case FOR_NODE:
	{
		int curForCnt = forCnt++;
		int g = 0;
		symTab.incrNestLevel();
		traverse(fout, curNode->descendants[0]);
		fout << "_for" << curForCnt << ":\n";
		traverse(fout, curNode->descendants[1]);
		fout << "cmp $1, %rax\n";
		fout << "jne _postfor" << curForCnt << "\n";
		//symTab.incrNestLevel();
		traverse(fout, curNode->descendants[3]);
		//int g = symTab.decrNestLevel();
		//if (g)
		//fout << "add $" << g << ", %rsp\n";
		traverse(fout, curNode->descendants[2]);
		fout << "jmp _for" << curForCnt << "\n";

		fout << "_postfor" << curForCnt << ":\n";
		g = symTab.decrNestLevel();
		if (g)
			fout << "add $" << g << ", %rsp\n";
		break;
	}
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
		if (symTab.insertNewSymbol(symbolName, INT_SYMBOL) == false)
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
		if (symTab.insertNewSymbol(symbolName, INT_SYMBOL) == false)
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
	case IF_STATEMENT_NODE:
	{
		int cnt = (++ifstmCnt);
		traverse(fout, curNode->descendants[0]);
		fout << "cmp $0, %rax\n";
		fout << "je _notif" << cnt << "\n";
		traverse(fout, curNode->descendants[1]);
		if (curNode->elseNode != NULL)
		{
			fout << "jmp _postif" << cnt << "\n";
		}
		fout << "_notif" << cnt << ":\n";
		if (curNode->elseNode != NULL)
		{
			traverse(fout, curNode->elseNode);
			fout << "_postif" << cnt << ":\n";
		}
		traverse(fout, curNode->descendants[2]);
		break;
	}
	case ASSIGNMENTOP_NODE:
	{
		auto symEntry = symTab.getVal((curNode->descendants[0])->value);
		int offset = symEntry.first;
		//cout << curNode->descendants[0]->value << " " << offset << "\n";
		if (offset == INT_MIN)
		{
			cout << "ERROR symbol not defined \n";
			exit(1);
		}
		traverse(fout, curNode->descendants[1]);
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
		auto symEntry = symTab.getVal(curNode->value);
		int offset  = symEntry.first;
		if (offset == INT_MIN)
		{
			cout << "ERROR symbol not defined \n";
			exit(1);
		}
		fout << "mov " << offset << "(%rbp), %rax\n";
		break;
	}
	case NULLEXP_NODE:
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
