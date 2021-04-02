#include "def.h"


symbolNode:: symbolNode(int soffset)
{
	offset = soffset;
}

symbolNode:: symbolNode()
{
	offset = 0;
}

symbolTable:: symbolTable()
{
	stackIdx = -8;
	mp.clear();
}

bool symbolTable:: insertNewSymbol(string symbolName, int sz)
{
	if (mp.find(symbolName) != mp.end())
		return false;

	mp[symbolName] = symbolNode(stackIdx);
	stackIdx -= sz;
	return true;
}

int symbolTable:: getVal(string symbolName)
{
	if (mp.find(symbolName) == mp.end())
		return INT_MIN;

	return mp[symbolName].offset;
}

symbolTable symTab;
