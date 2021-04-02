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
%token  IDTOKEN
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
%token  ASSIGNMENT
%token  MAIN
%token  VOID

%%
program 		: function {$$ = createNewNode(ROOT_NODE);addDescendantNode($$,$1);setASTroot($$);}

function 		: INT MAIN LPARENTHESIS RPARENTHESIS LCURLY statementList RCURLY {printf("function hai\n");$$ = createNewNode(FUNCTION_NODE);addDescendantNode($$,$6);}

statementList   : statement statementList {addDescendantNode($1,$2);$$ = $1;};
				| statement {$$ = $1;};

statement 		: RETURN exp SEMICOLON {$$ = createNewNode(RETURN_STATEMENT_NODE);addDescendantNode($$,$2);}
				| exp SEMICOLON {$$ = createNewNode(EXP_STATEMENT_NODE);addDescendantNode($$,$1);}
				| INT ID SEMICOLON {$$ = createNewNode(DECLARE_STATEMENT_NODE);addDescendantNode($$,$2);}
				| INT ID ASSIGNMENT exp SEMICOLON {$$ = createNewNode(DECLARE_ASSIGN_STATEMENT_NODE);addDescendantNode($$,$2);addDescendantNode($$,$4);};

exp             : ID ASSIGNMENT exp {$$ = createNewNode(ASSIGNMENTOP_NODE);addDescendantNode($$,$1);addDescendantNode($$,$3);}
				| logicalorexp {$$ = $1;};

logicalorexp 	: logicalorexp binary_op_or logicalandExp {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);} 
	   			| logicalandExp {$$ = $1;};

logicalandExp   : logicalandExp binary_op_and equalityExp {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
				| equalityExp {$$ = $1;};

equalityExp 	: equalityExp binary_op_eq relationalExp {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
			    | relationalExp {$$ = $1;};

relationalExp   : relationalExp binary_op_rel additiveExp {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
				| additiveExp {$$ = $1;};

additiveExp 	: additiveExp binary_op_add term {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
	   			| term {$$ = $1;};

term            : term binary_op_mul factor {$$ = createNewNode(BINARYOP_NODE,$2);addDescendantNode($$,$1);addDescendantNode($$,$3);}
				| factor {$$ = $1;};

factor          : LPARENTHESIS exp RPARENTHESIS {$$ = $2;}
				| unary_op factor {$$ = createNewNode(UNARYOP_NODE,$1);addDescendantNode($$,$2);}
				| num {$$ = createNewNode(CONSTEXP_NODE,0,yytext);}
				| ID {$$ = $1;};

ID 				: IDTOKEN {$$ = createNewNode(IDENTIFIER_NODE,0,yytext);};

binary_op_or  	: OR {$$ = OR;};

binary_op_and 	: AND {$$ = AND;};

binary_op_eq  	: NOTEQUAL {$$ = NOTEQUAL;} | EQUAL {$$ = EQUAL;};

binary_op_rel 	: LESSTHAN {$$ = LESSTHAN;} | LESSTHANEQ {$$ = LESSTHANEQ;} | GREATERTHAN {$$ = GREATERTHAN;} | GREATERTHANEQ {$$ =GREATERTHANEQ;};

binary_op_add 	: ADDITION {$$ = ADDITION;} | MINUS {$$ = MINUS;};

binary_op_mul 	: MULTIPLICATION {$$ = MULTIPLICATION;} | DIVISION {$$ = DIVISION;};

unary_op       	: LOGICAL_NEGATION {$$ =LOGICAL_NEGATION;} | COMPLEMENT {$$ = COMPLEMENT;} | MINUS {$$= MINUS;};
%%

