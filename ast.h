#ifndef AST__
#define AST__
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

class node
{
public:
	int type;
	int subType;
	string value;

	vector<node*> descendants;
	vector<node*> blockDescendantList;
	node* elseNode;
	node(int Ntype, int NsubType = 0, string Nvalue = "");
	void addDescendant(node * descendant);
	void addBlockDescendant(node * descendant);
};




class AST
{
public :
	node * rootNodeAST;
	int sectionsCnt;
	int ifstmCnt;
	AST();

	void setRootNode(node* root);

	void traverse(ofstream &fout, node* curNode);
	void visitDescendants (ofstream &fout, node* curNode);
	
};

/*
node :: node (int Ntype, int Nrvalue, string Nname )
{
	type = Ntype;
	rvalue = Nrvalue;
	name = Nname;
	descendants = vector<node *>();
}

void node :: addDescendant(node * descendant)
{
	descendants.push_back(descendant);
}

int node:: getRval()
{
	return rvalue;
}


uintptr_t createNewNode(int Ntype, int Nrvalue, string Nname )
{
	node* newNode = new node(Ntype, Nrvalue, Nname);
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

int AST:: visitDescendants(ofstream &fout, node * curNode)
{
	if (curNode == NULL)
		return 0;

	int ans = 0;
	for (auto descendant : curNode->descendants)
		ans += traverse(fout, descendant);
	return ans;
}


int AST:: traverse(ofstream &fout, node* curNode)
{
	if (curNode == NULL)
		return 0;

	cout<<curNode->type<<"\n";
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
		curNode->rvalue = visitDescendants(fout, curNode);
		//fout << "mov    $" << curNode->rvalue << ", %eax\n";
		fout << "mov     $60, %rax\n";
		fout << "mov     $"<<curNode->rvalue<<", %rdi\n";
		fout << "syscall\n";
		break;
	case EXP_NODE:
		return curNode->rvalue;
		break;
	default:
		break;
	}
	return curNode->rvalue;
}


void setASTroot(uintptr_t root)
{
	cout<<((node *)root)->type<<"\n";
	ast.setRootNode((node *)root);
	rootNodeAST = ((node *)root);
	return ;
}
*/

#endif
