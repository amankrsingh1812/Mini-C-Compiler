%{ 
   #include <stdio.h> 
   #include <ctype.h>
   #include "lex.h"
   #include "def.h"
   #define YYSTYPE uintptr_t
	/* yacc error handler */
   void yyerror(char const * s)
   {   
		printf ("YYERROR:%s\n", s);
   } 


%} 

%token  INT
%token  ID
%token  num
%token  LPARENTHESIS
%token  RPARENTHESIS
%token  RCURLY
%token  LCURLY
%token  RETURN
%token  SEMICOLON
%token  ADDITION
%token  MINUS
%token  MULTIPLICATION
%token  DIVISION
%token  COMPLEMENT
%token  LOGICAL_NEGATION
%token  AND
%token  OR
%token  EQUAL
%token  NOTEQUAL
%token  LESSTHAN
%token  LESSTHANEQ
%token  GREATERTHAN
%token  GREATERTHANEQ
%token  MAIN

%%
program 		: function {$$ = createNewNode(ROOT_NODE);addDescendantNode($$,$1);setASTroot($$);}

function 		: INT MAIN LPARENTHESIS RPARENTHESIS LCURLY statement RCURLY {$$ = createNewNode(FUNCTION_NODE);addDescendantNode($$,$6);}

statement 		: RETURN exp SEMICOLON {$$ = createNewNode(STATEMENT_NODE);addDescendantNode($$,$2);}

exp 			: exp binary_op_predl term {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
	   			| term {$$ = $1;};

term            : term binary_op_predh factor {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
				| factor {$$ = $1;};

factor          : LPARENTHESIS exp RPARENTHESIS {$$ = $2;}
				| unary_op factor {$$ = createNewNode(UNARYOP_NODE,$1);addDescendantNode($$,$2);}
				| num {$$ = createNewNode(CONSTEXP_NODE,0,yytext);};

binary_op_predl : ADDITION {$$ = ADDITION;} | MINUS {$$ = MINUS;};

binary_op_predh : MULTIPLICATION {$$ = MULTIPLICATION;} | DIVISION {$$ = DIVISION;};

unary_op        : LOGICAL_NEGATION {$$ =LOGICAL_NEGATION;} | COMPLEMENT {$$ = COMPLEMENT;} | MINUS {$$= MINUS;};
%%

