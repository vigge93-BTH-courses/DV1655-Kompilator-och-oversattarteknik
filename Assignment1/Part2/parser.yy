%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
}
%code{
  #define YY_DECL yy::parser::symbol_type yylex()

  YY_DECL;
  
  Node* root;
  
}
// definition of set of tokens. All tokens are of type string
%token <std::string> CLASS LBRACE RBRACE LBRACKET RBRACKET LPAR RPAR SC LT GT AND OR EQ ADDSIGN SUBSIGN MULTSIGN DIVSIGN NOT DOT EQASSIGN COMMA EXTENDS PUBLIC STATIC VOID MAIN STRING BOOL INT IF ELSE WHILE PRINTLN LENGTH TRUE FALSE THIS NEW ID INT_LITERAL
%token END 0 "end of file"

// definition of the production rules. All production rules are of type Node
%type <Node *> Goal MainClass ClassDeclaration VarDeclaration MethodDeclaration Type Statement Expression orExpression andExpression eqExpression lgExpression lgExpressionP multdivExpression multdivExpressionP notExpression comExpression addsubExpression addsubExpressionP parExpression parExpressionP1 parExpressionP1P parExpressionP2 parExpressionP3 Identifier Int 

%%
Goal:
  | Expression SC Goal {$$ = new Node("Root expression", ""); $$->children.push_back($1); $$->children.push_back($3); root = $$;}
  | Expression {$$ = new Node("Root expression", ""); $$->children.push_back($1); if (root == NULL) {root = $$;}}
  | Expression END {$$ = new Node("Root expression", ""); $$->children.push_back($1); if (root == NULL) {root = $$;}};

/* Statement: IF LPAR Expression RPAR Statement ELSE Statement
  | WHILE LPAR Expression RPAR Statement
  | PRINTLN  LPAR Expression RPAR SC
  | Identifier StatementP

StatementP: EQASSIGN Expression SC
  | LBRACKET Expression RBRACKET EQASSIGN Expression SC; */

Expression:
  orExpression {$$ = $1;};

/* Statement: 
  LBRACE (Statement)* RBRACE
  | IF LPAR Expression RPAR  */

orExpression: orExpression OR andExpression {$$ = new Node("Or expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | andExpression {$$ = $1;};

andExpression: andExpression AND eqExpression {$$ = new Node("and expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | eqExpression {$$=$1;};

eqExpression: eqExpression EQ lgExpression {$$ = new Node("eq expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | lgExpression {$$ = $1;};

lgExpression: lgExpression lgExpressionP {$$ = $2; $$->children.push_back($1);}
  | addsubExpression {$$ = $1;};

lgExpressionP: LT addsubExpression {$$ = new Node("lT expression", ""); $$->children.push_back($2);}
  | GT addsubExpression {$$ = new Node("gT expression", ""); $$->children.push_back($2);};

addsubExpression: addsubExpression addsubExpressionP {$$ = $2; $$->children.push_back($1);}
  | multdivExpression {$$ = $1;};

addsubExpressionP:
  ADDSIGN multdivExpression {$$ = new Node("add expression", ""); $$->children.push_back($2);}
  | SUBSIGN multdivExpression {$$ = new Node("sub expression", ""); $$->children.push_back($2);};

multdivExpression: multdivExpression multdivExpressionP {$$ = $2; $$->children.push_back($1);}
  | notExpression {$$ = $1;};

multdivExpressionP: MULTSIGN notExpression {$$ = new Node("mult expression", ""); $$->children.push_back($2);}
  |  DIVSIGN notExpression {$$ = new Node("div expression", ""); $$->children.push_back($2);};

notExpression: notExpression NOT parExpression {$$ = new Node("not expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | parExpression {$$ = $1;};

parExpression: Identifier {$$ = $1;}
  | Int {$$ = $1;}
  | TRUE {$$ = new Node("TRUE", "");}
  | FALSE {$$ = new Node("FALSE", "");}
  | THIS {$$ = new Node("THIS", "");}
  | LPAR Expression RPAR {$$ = new Node("(Expression)", ""); $$->children.push_back($2);} 
  | Expression parExpressionP1 {$$ = $2; $$->children.push_back($1);}
  | NEW parExpressionP2 {$$ = $2;};

parExpressionP1: LBRACKET Expression RBRACKET {$$ = new Node("E[expression]", ""); $$->children.push_back($2);}
  | DOT parExpressionP1P {$$ = $2;};
  | Expression

parExpressionP1P: Identifier LPAR Expression comExpression RPAR {$$ = new Node("Method call", ""); $$->children.push_back($1); $$->children.push_back($3); $$->children.push_back($4);}
  | LENGTH {$$ = new Node(".length", "");};

parExpressionP2: 
  INT LBRACKET Expression RBRACKET {$$ = new Node("new int[]", ""); $$->children.push_back($3);}
| Identifier LPAR RPAR {$$ = new Node("Object instantiation", ""); $$->children.push_back($1);};

comExpression:
  | COMMA Expression {$$ = new Node(",E", ""); $$->children.push_back($2);}
  | comExpression comExpression {$$ = new Node("CC", ""); $$->children.push_back($1); $$->children.push_back($2);};

// Par -> NOT -> * / -> +- -> LT / GT -> EQ -> AND -> OR
Identifier: ID {
  $$ = new Node("ID", $1);
};

Int: INT_LITERAL {$$ = new Node("Int", $1);};


/* 
expression: addExpression 
                          { /*  
                                Here we create the root node (named Expression), then we add the content of addExpression (accessed through $1) as a child of the root node. 
                                The "root" is a reference to the root node. 
                            
                            $$ = new Node("Expression", "");
                            $$->children.push_back($1);
                            root = $$;
                            printf("r1 ");
                          };

addExpression: multExpression { $$ = $1; printf("r2 "); /*simply return the content of multExpression}
             | addExpression PLUSOP multExpression {  /*
                                                  Create a subtree that corresponds to the AddExpressions
                                                  The root of the subtree is AddExpression
                                                  The childs of the AddExpression subtree are the left hand side (addExpression accessed through $1) and right hand side of the expression (multExpression accessed through $3)
                                                
                            $$ = new Node("AddExpression", "");
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            printf("r3 ");
                          }
      ;

multExpression: factor { $$ = $1; printf("r4 "); /*simply return the content of multExpression}
              | multExpression MULTOP factor { /*
                                                  Create a subtree that corresponds to the MultExpression
                                                  The root of the subtree is MultExpression
                                                  The childs of the MultExpression subtree are the left hand side (multExpression accessed through $1) and right hand side of the expression (factor accessed through $3)
                                                
                            $$ = new Node("MultExpression", ""); 
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            printf("r5 ");
                      }
        ;

factor: INT  {  $$ = new Node("Int", $1); printf("r6 "); /* Here we create a leaf node Int. The value of the leaf node is $1 }
    | LP expression RP { $$ = $2; printf("r7 "); /* simply return the expression }
    ; */
