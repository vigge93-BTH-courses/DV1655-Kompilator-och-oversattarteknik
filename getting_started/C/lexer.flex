%top{
#define YYSTYPE int
}
%option noyywrap
%x STRING
%%
[^"]*           {printf("OTHER: %s\n",yytext);}    /* Any character */
["]             {BEGIN STRING;}
<STRING>[^"]*   {printf("STRING: %s\n", yytext);}
<STRING>["]	    {BEGIN INITIAL;}
%%