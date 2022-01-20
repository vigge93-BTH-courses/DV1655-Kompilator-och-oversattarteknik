%top{
    #include "stdlib.h"
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
"class" {printf("CLASS\n");}
"{" {printf("LBRACE\n");}
"}" {printf("RBRACE\n");}
"[" {printf("LBRACKET\n");}
"]" {printf("RBRACKER\n");}
"(" {printf("LPAR\n");}
")" {printf("RPAR\n");}
";" {printf("SC\n");}
"<" {printf("LT\n");}
">" {printf("GT\n");}
"&&" {printf("AND\n");}
"||" {printf("OR\n");}
"==" {printf("EQ\n");}
"+" {printf("ADDSIGN\n");}
"-" {printf("SUBSIGN\n");}
"*" {printf("MULTSIGN\n");}
"/" {printf("DIVSIGN\n");}
"!" {printf("NOT\n");}
"." {printf("DOT\n");}
"=" {printf("EQASSIGN\n");}
"," {printf("COMMA\n");}
"extends" {printf("EXTENDS\n");}
"public" {printf("PUBLIC\n");}
"static" {printf("STATIC\n");}
"void" {printf("VOID\n");}
"main" {printf("MAIN\n");}
"String" {printf("STRING\n");}
"boolean" {printf("BOOL\n");}
"int" {printf("INT\n");}
"if" {printf("IF\n");}
"else" {printf("ELSE\n");}
"while" {printf("WHILE\n");}
"System.out.println" {printf("PRINTLN\n");}
"length" {printf("LENGTH\n");}
"true" {printf("TRUE\n");}
"false" {printf("FALSE\n");}
"this" {printf("THIS\n");}
"new" {printf("NEW\n");}
{INTEGER_LITERAL} {printf("INT_LITERAL: %s\n", yytext);}
{IDENTIFIER} {printf("ID: %s\n", yytext);}
{COMMENT} {printf("COMMENT: %s\n", yytext);}
[ \t\n]+ {}
. {if (buf == NULL) buf = malloc(10); buf[next_buff_pos] = yytext[0]; next_buff_pos += 1; if (next_buff_pos == buff_size) {buff_size = buff_size*2; buf = realloc(buf, buff_size);}}
<<EOF>> return yyerror(buf);
%%
int yyerror (char const *s) {
   fprintf (stderr, "Unrecognized inputs: %s\n", s);
   return 0;
}