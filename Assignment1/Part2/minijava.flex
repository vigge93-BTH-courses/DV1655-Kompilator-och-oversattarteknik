%top{
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
}
%option noyywrap nounput batch noinput stack nodefault
INTEGER_LITERAL 0|-?[1-9][0-9]*
IDENTIFIER [a-zA-Z][a-zA-Z0-9_]*
COMMENT \/\/.*
%%
"class" {return yy::parser::make_CLASS(yytext);}
"{" {return yy::parser::make_LBRACE(yytext);}
"}" {return yy::parser::make_RBRACE(yytext);}
"[" {return yy::parser::make_LBRACKET(yytext);}
"]" {return yy::parser::make_RBRACKET(yytext);}
"(" {return yy::parser::make_LPAR(yytext);}
")" {return yy::parser::make_RPAR(yytext);}
";" {return yy::parser::make_SC(yytext);}
"<" {return yy::parser::make_LT(yytext);}
">" {return yy::parser::make_GT(yytext);}
"&&" {return yy::parser::make_AND(yytext);}
"||" {return yy::parser::make_OR(yytext);}
"==" {return yy::parser::make_EQ(yytext);}
"+" {return yy::parser::make_ADDSIGN(yytext);}
"-" {return yy::parser::make_SUBSIGN(yytext);}
"*" {return yy::parser::make_MULTSIGN(yytext);}
"/" {return yy::parser::make_DIVSIGN(yytext);}
"!" {return yy::parser::make_NOT(yytext);}
"." {return yy::parser::make_DOT(yytext);}
"=" {return yy::parser::make_EQASSIGN(yytext);}
"," {return yy::parser::make_COMMA(yytext);}
"extends" {return yy::parser::make_EXTENDS(yytext);}
"public" {return yy::parser::make_PUBLIC(yytext);}
"static" {return yy::parser::make_STATIC(yytext);}
"void" {return yy::parser::make_VOID(yytext);}
"main" {return yy::parser::make_MAIN(yytext);}
"String" {return yy::parser::make_STRING(yytext);}
"boolean" {return yy::parser::make_BOOL(yytext);}
"int" {return yy::parser::make_INT(yytext);}
"if" {return yy::parser::make_IF(yytext);}
"else" {return yy::parser::make_ELSE(yytext);}
"while" {return yy::parser::make_WHILE(yytext);}
"System.out.println" {return yy::parser::make_PRINTLN(yytext);}
"length" {return yy::parser::make_LENGTH(yytext);}
"true" {return yy::parser::make_TRUE(yytext);}
"false" {return yy::parser::make_FALSE(yytext);}
"this" {return yy::parser::make_THIS(yytext);}
"new" {return yy::parser::make_NEW(yytext);}
{INTEGER_LITERAL} {return yy::parser::make_INT_LITERAL(yytext);}
{IDENTIFIER} {return yy::parser::make_ID(yytext);}
{COMMENT} {printf("COMMENT: %s\n", yytext);}
[ \t\n]+ {}
. {return *yytext;}
%%
