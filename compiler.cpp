#include <iostream>
#include <cstdio>
#include <cstdint>
#include "lex.h"
//#include "def.h"
#include "y.tab.h"
#include "def.h"
#include <fstream>

using namespace std;

int main()
{
	FILE *fp = fopen("test.c", "r");
	yyin = fp;
	yyparse();

	ofstream fout("outputFiles/a.s",ios::out);
	//cout<<(uintptr_t)ast.rootNodeAST<<"\n";
	ast.traverse(fout,ast.rootNodeAST);
	return 0;
}
