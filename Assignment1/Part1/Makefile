compiler: lex.yy.c 
		gcc -o compiler lex.yy.c main.c -ll
lex.yy.c:
		flex minijava.flex
clean:
		rm -f lex.yy.c compiler