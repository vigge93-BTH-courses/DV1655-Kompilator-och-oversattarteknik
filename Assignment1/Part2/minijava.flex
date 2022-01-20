%top{
    #include "stdlib.h"
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
    int yyerror (char const *s);
    int buff_size = 10;
    char* buf = NULL;
    int next_buff_pos = 0;
}
%option noyywrap nounput batch noinput stack
INTEGER_LITERAL 0|-?[1-9][0-9]*
IDENTIFIER [a-zA-Z][a-zA-Z0-9_]*
COMMENT \/\/.*
%%
"class" {return yy::parser::make_CLASS();}
"{" {return yy::parser::make_LBRACE();}
"}" {return yy::parser::make_RBRACE();}
"[" {return yy::parser::make_LBRACKET();}
"]" {return yy::parser::make_RBRACKER();}
"(" {return yy::parser::make_LPAR();}
")" {return yy::parser::make_RPAR();}
";" {return yy::parser::make_SC();}
"<" {return yy::parser::make_LT();}
">" {return yy::parser::make_GT();}
"&&" {return yy::parser::make_AND();}
"||" {return yy::parser::make_OR();}
"==" {return yy::parser::make_EQ();}
"+" {return yy::parser::make_ADDSIGN();}
"-" {return yy::parser::make_SUBSIGN();}
"*" {return yy::parser::make_MULTSIGN();}
"/" {return yy::parser::make_DIVSIGN();}
"!" {return yy::parser::make_NOT();}
"." {return yy::parser::make_DOT();}
"=" {return yy::parser::make_EQASSIGN();}
"," {return yy::parser::make_COMMA();}
"extends" {return yy::parser::make_EXTENDS();}
"public" {return yy::parser::make_PUBLIC();}
"static" {return yy::parser::make_STATIC();}
"void" {return yy::parser::make_VOID();}
"main" {return yy::parser::make_MAIN();}
"String" {return yy::parser::make_STRING();}
"boolean" {return yy::parser::make_BOOL();}
"int" {return yy::parser::make_INT();}
"if" {return yy::parser::make_IF();}
"else" {return yy::parser::make_ELSE();}
"while" {return yy::parser::make_WHILE();}
"System.out.println" {return yy::parser::make_PRINTLN();}
"length" {return yy::parser::make_LENGTH();}
"true" {return yy::parser::make_TRUE();}
"false" {return yy::parser::make_FALSE();}
"this" {return yy::parser::make_THIS();}
"new" {return yy::parser::make_NEW();}
{INTEGER_LITERAL} {return yy::parser::make_INT_LITERAL(yytext);}
{IDENTIFIER} {return yy::parser::make_ID(yytext);}
{COMMENT} {printf("COMMENT: %s\n", yytext);}
[ \t\n]+ {}
. {if (buf == NULL) buf = malloc(10); buf[next_buff_pos] = yytext[0]; next_buff_pos += 1; if (next_buff_pos == buff_size) {buff_size = buff_size*2; buf = realloc(buf, buff_size);}}
<<EOF>> return yyerror(buf);
%%
int yyerror (char const *s) {
   fprintf (stderr, "Unrecognized inputs: %s\n", s);
   return 0;
}