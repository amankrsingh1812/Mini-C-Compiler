%{
#include <stdio.h>
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
%}

delim       [ \t\n]
whitespace  {delim}+
digit		[0-9]
letter		[a-zA-Z]
num			{digit}+
id			{letter}({letter}|{digit})*

%%

{whitespace} {}
";"          {return SEMICOLON;}
"("			 {return LPARENTHESIS;}
")"			 {return RPARENTHESIS;}
"{"          {return LCURLY;}
"}"			 {return RCURLY;}
"+"          {return ADDITION;}
"-" 		 {return MINUS;}
"*"          {return MULTIPLICATION;}
"/"          {return DIVISION;}
"~"          {return COMPLEMENT;}
"!" 		 {return LOGICAL_NEGATION;}
"&&" 		 {return AND;}
"||" 		 {return OR;}
"==" 		 {return EQUAL;}
"!=" 		 {return NOTEQUAL;}
"<" 		 {return LESSTHAN;}
"<=" 		 {return LESSTHANEQ;}
">" 		 {return GREATERTHAN;}
">=" 		 {return GREATERTHANEQ;}
"=" 		 {return ASSIGNMENT;}
void		 {return VOID;}
int			 {return INT;}
main		 {return MAIN;}
return		 {return RETURN;}
if           {return IF;}
else 		 {return ELSE;}
for          {return FOR;}
while        {return WHILE;}
do           {return DO;}
break        {return BREAK;}
continue     {return CONTINUE;}
":" 		 {return COLON;}
"?" 		 {return QUESTION_MARK;}
{id}		 {return IDTOKEN;}
{num}		 {return num;}
%%

int yywrap()
{
	return 1;
}


