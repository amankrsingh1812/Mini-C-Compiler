CC = g++ -std=c++17 

compiler: compiler.cpp lex.o parser.o ast.o
		$(CC)compiler.cpp lex.o parser.o ast.o -o compiler.out

ast.o: ast.cpp ast.h
		$(CC)-c ast.cpp -o ast.o
lex.o: lex.yy.c
		$(CC)-c lex.yy.c -o lex.o

parser.o: y.tab.c lex.o
		$(CC)-c y.tab.c  -o parser.o

lex.yy.c: lex.l y.tab.c
		lex --header-file=lex.h lex.l  

y.tab.c: parser.y def.h ast.h
		yacc -d parser.y

clean: 
		rm *.o
		rm *.out
