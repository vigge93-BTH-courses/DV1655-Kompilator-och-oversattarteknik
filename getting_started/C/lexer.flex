%top{
#define YYSTYPE int
}
%option noyywrap
%x STRING
%%
[^"]*           { printf("OTHER: %s\n",yytext);}    /* Any character */
["]              {BEGIN STRING;}
<STRING>[^"]*\"   {yytext[yyleng - 1] = '\0'; printf("STRING: %s\n", yytext); BEGIN INITIAL;}
<STRING>[^"]*	{printf("OTHER: %s\n", yytext); BEGIN INITIAL;}
%%