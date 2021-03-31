#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "y.tab.h"

int main()
{
	FILE *fp = fopen("test.c", "r");
	yyin = fp;
	yyparse();
	return 0;
}
