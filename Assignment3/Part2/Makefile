compiler: lex.yy.c parser.tab.o main.cc
		g++ -g -w -ocompiler parser.tab.o lex.yy.c main.cc symbol_table.cc semantic.cc cfg.cc tac.cc bytecode.cc -std=c++14
parser.tab.o: parser.tab.cc
		g++ -g -w -c parser.tab.cc -std=c++14
parser.tab.cc: parser.yy
		bison parser.yy -v
lex.yy.c: minijava.flex parser.tab.cc
		flex minijava.flex
tree: 
		 dot -Tpdf tree.dot -otree.pdf
clean:
		rm -f parser.tab.* lex.yy.c* compiler stack.hh *.dot tree.pdf symbol_table.dot *.o parser.output *.bc
		rm -R compiler.dSYM