#ifndef SYMBOLTABLE__
#define SYMBOLTABLE__ 
#include <unordered_map>
#include <string>
#include <memory>

class symbolNode
{
public:
	symbolNode(int soffset,int type,int depth,string symbolName);
	symbolNode();
	~symbolNode();
	int offset;
	int type;
	int depth;
	string symbolName;
	shared_ptr<symbolNode> level;
	shared_ptr<symbolNode> var;
};

class symbolTable
{
public:
	int stackIdx;
	int depth;
	unordered_map <string, shared_ptr<symbolNode>> hashTable;
	unordered_map <int, shared_ptr<symbolNode>> scopeDisplay;

	bool insertNewSymbol(string symbolName, int symbolType);
	pair<int,int> getVal(string symbolName);
	void incrNestLevel();
	int decrNestLevel();
	symbolTable();

};
#endif
