#include "def.h"


symbolNode:: symbolNode(int soffset, int stype, int sdepth, string ssymbolName)
{
	offset = soffset;
	type = stype;
	depth = sdepth;
	symbolName = ssymbolName;
}

symbolNode:: symbolNode()
{
	offset = 0;
}

symbolNode:: ~symbolNode()
{
	cout << "Deleting symbol " << symbolName << " blockdepth " << depth << "\n";
}


symbolTable:: symbolTable()
{
	stackIdx = -8;
	depth = 0;
	hashTable.clear();
	scopeDisplay.clear();
}

void symbolTable:: incrNestLevel()
{
	depth++;
	scopeDisplay[depth] = NULL;
}

int symbolTable:: decrNestLevel()
{
	int sizeDel = 0;
	shared_ptr<symbolNode> temp = scopeDisplay[depth];
	scopeDisplay[depth] = NULL;

	while (temp != NULL)
	{
		hashTable[temp->symbolName] = temp->var;
		temp = temp->level;
		sizeDel += 8;
	}

	stackIdx += sizeDel;
	depth--;

	return sizeDel;
}

bool symbolTable:: insertNewSymbol(string symbolName, int symbolType)
{
	cout<<"Symbol insertion "<<symbolName<<"\n";
	shared_ptr<symbolNode> oldNode = hashTable[symbolName];
	if (oldNode!=NULL && oldNode->depth == depth)
		return false;

	shared_ptr<symbolNode> temp = shared_ptr<symbolNode>(new symbolNode(stackIdx, symbolType, depth,symbolName));
	temp->level = scopeDisplay[depth];
	scopeDisplay[depth] = temp;

	temp->var = oldNode;
	hashTable[symbolName] = temp;
	stackIdx -= 8;

	return true;
}

pair<int, int> symbolTable:: getVal(string symbolName)
{
	if (hashTable.find(symbolName) == hashTable.end())
		return {INT_MIN,0};

	return {hashTable[symbolName]->offset, hashTable[symbolName]->type};
}

symbolTable symTab;
