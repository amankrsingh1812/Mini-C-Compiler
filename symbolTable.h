#include <unordered_map>
#include <string>

class symbolNode
{
public:
	symbolNode(int soffset);
	symbolNode();
	int offset;
};

class symbolTable
{
public:
	int stackIdx;
	unordered_map <string, symbolNode> mp;
	bool insertNewSymbol(string symbolName, int sz);
	int getVal(string symbolName);
	symbolTable();

};
