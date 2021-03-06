%{ 
   #include <stdio.h> 
   #include <ctype.h>
   #include "lex.h"
   #include "def.h"
   #define YYSTYPE uintptr_t
	/* yacc error handler */
   void yyerror(char const * s)
   {   
		printf ("YYERROR:%s %s\n", s,yytext);
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
%token  IF
%token  ELSE
%token  COLON
%token  QUESTION_MARK
%token  FOR
%token  WHILE
%token  DO
%token  BREAK
%token  CONTINUE
%token  COMMA

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
program 		: function {$$ = createNewNode(ROOT_NODE);addDescendantNode($$,$1);setASTroot($$);}

function 		: INT MAIN LPARENTHESIS RPARENTHESIS LCURLY block RCURLY {printf("function hai\n");setFuncOuterBlock($6);$$ = createNewNode(FUNCTION_NODE);addDescendantNode($$,$6);}

block   		: block  blockItem {addBlockDescendantNode($1,$2);$$ = $1;}
				| blockItem {$$ = createNewNode(BLOCK_NODE);addBlockDescendantNode($$,$1);};

blockItem 		: statement {$$ = $1;}
				| declaration {$$ = $1;};

expORdecl       : exp SEMICOLON {$$ = $1;}
				| SEMICOLON {$$ = createNewNode(NULLEXP_NODE);}
				| declaration {$$ = $1;};

expSEMICOLON    : exp SEMICOLON {$$ =$1;}
				| SEMICOLON {$$ = createNewNode(NULLEXP_NODE);};

expRPARENTHESIS : exp RPARENTHESIS {$$ = $1;}
				| RPARENTHESIS {$$ = createNewNode(NULLEXP_NODE);};

declaration 	: INT ID SEMICOLON {$$ = createNewNode(DECLARE_STATEMENT_NODE);addDescendantNode($$,$2);}
				| INT ID ASSIGNMENT exp SEMICOLON {$$ = createNewNode(DECLARE_ASSIGN_STATEMENT_NODE);addDescendantNode($$,$2);addDescendantNode($$,$4);};
			 	
statement 		: LCURLY block RCURLY {$$ = $2;}
			    | LCURLY RCURLY {$$ = createNewNode(NULLEXP_NODE);}
				| RETURN exp SEMICOLON {$$ = createNewNode(RETURN_STATEMENT_NODE);addDescendantNode($$,$2);}
				| expSEMICOLON {$$ = createNewNode(EXP_STATEMENT_NODE);addDescendantNode($$,$1);}
				| IF LPARENTHESIS exp RPARENTHESIS statement %prec LOWER_THAN_ELSE {$$ = createNewNode(IF_STATEMENT_NODE);addDescendantNode($$,$3);addDescendantNode($$,$5);}
				| IF LPARENTHESIS exp RPARENTHESIS statement ELSE statement {$$ = createNewNode(IF_STATEMENT_NODE);addDescendantNode($$,$3);addDescendantNode($$,$5);setElseNode($$,$7);}
                | WHILE LPARENTHESIS exp RPARENTHESIS statement {$$ = createNewNode(WHILE_NODE);addDescendantNode($$,$3);addDescendantNode($$,$5);}
				| FOR LPARENTHESIS expORdecl exp SEMICOLON expRPARENTHESIS statement {$$ = createNewNode(FOR_NODE);addDescendantNode($$,$3);addDescendantNode($$,$4);addDescendantNode($$,$6);addDescendantNode($$,$7);}
				| FOR LPARENTHESIS expORdecl SEMICOLON expRPARENTHESIS statement {$$ = createNewNode(FOR_NODE);addDescendantNode($$,$3);addDescendantNode($$,createNewNode(CONSTEXP_NODE,0,"1"));addDescendantNode($$,$5);addDescendantNode($$,$6);};

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

