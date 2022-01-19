HEXDIGIT [0-9a-f]
%%
0x{HEXDIGIT}+ {printf("Token: %s\n", yytext);}

%%
int main(int argc, char **argv) {
    yylex();
}