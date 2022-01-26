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
%token <std::string> CLASS LBRACE RBRACE LBRACKET RBRACKET LPAR RPAR SC LT GT AND OR EQ ADDSIGN SUBSIGN MULTSIGN DIVSIGN NOT DOT EQASSIGN COMMA EXTENDS PUBLIC STATIC VOID MAIN STRING BOOL INT IF ELSE WHILE PRINTLN LENGTH TRUE FALSE THIS NEW RETURN ID INT_LITERAL
%token END 0 "end of file"

// definition of the production rules. All production rules are of type Node
%type <Node *> Goal MainClass ClassDeclaration ClassBody ClassDeclarationP ClassDeclarationMethods ClassDeclarationVars VarDeclaration MethodDeclaration MethodBody MethodDeclarationParams MethodDeclarationVars MethodDeclarationStatements Type Statement LRStatement Expression orExpression andExpression eqExpression lgExpression  multdivExpression  notExpression comExpression addsubExpression  parExpression  Identifier Int 

%%
/* Goal:
  | Statement Goal {$$ = new Node("Root statement", ""); $$->children.push_back($1); $$->children.push_back($2); root = $$;}
  | Statement {$$ = new Node("Root statement", ""); $$->children.push_back($1); if (root == NULL) {root = $$;}};
  | Statement END {$$ = new Node("Root expression", ""); $$->children.push_back($1); if (root == NULL) {root = $$;}}; */

Goal:
  MainClass END  {$$ = $1; root = $$;}
  | MainClass ClassDeclarationP END {$$ = $1; $$->children.push_back($2); root = $$;};

MainClass: 
  CLASS Identifier LBRACE PUBLIC STATIC VOID MAIN LPAR STRING LBRACKET RBRACKET Identifier RPAR LBRACE Statement RBRACE RBRACE {$$ = new Node("Main Class", ""); $$->children.push_back($2); $$->children.push_back($12); $$->children.push_back($15);};

ClassDeclarationP:
  ClassDeclaration {$$ = $1;}
  | ClassDeclarationP ClassDeclaration {$$ = $1; $$->children.push_back($2);};

ClassDeclaration: CLASS Identifier ClassBody {$$ = new Node("Class", ""); $$->children.push_back($2); $$->children.push_back($3);}
| CLASS Identifier EXTENDS Identifier ClassBody {$$ = new Node("Class", ""); $$->children.push_back($2); $$->children.push_back($4); $$->children.push_back($5);};

ClassBody: 
  LBRACE RBRACE {$$ = new Node("No class body", "");}
| LBRACE ClassDeclarationVars RBRACE {$$ = $2;}
| LBRACE ClassDeclarationMethods RBRACE{$$ = $2;}
| LBRACE ClassDeclarationVars ClassDeclarationMethods RBRACE {$$ = new Node("Class body", ""); $$->children.push_back($2); $$->children.push_back($3);};

ClassDeclarationVars: VarDeclaration {$$ = $1;}
| ClassDeclarationVars VarDeclaration {$$ = new Node("Class vars", ""); $$->children.push_back($1); $$->children.push_back($2);}

ClassDeclarationMethods: MethodDeclaration {$$ = $1;}
| ClassDeclarationMethods MethodDeclaration {$$ = new Node("Class methods", ""); $$->children.push_back($1); $$->children.push_back($2);}

MethodDeclaration:
PUBLIC Type Identifier LPAR RPAR MethodBody {$$ = new Node("Method", ""); $$->children.push_back($2); $$->children.push_back($3); $$->children.push_back($6);}
| PUBLIC Type Identifier LPAR MethodDeclarationParams RPAR MethodBody {$$ = new Node("Method", ""); $$->children.push_back($2); $$->children.push_back($3); $$->children.push_back($5); $$->children.push_back($7);};

MethodBody: LBRACE RETURN Expression SC RBRACE {$$ = $3;}
| LBRACE MethodDeclarationVars RETURN Expression SC RBRACE {$$ = new Node("Method body", "");$$->children.push_back($2); $$->children.push_back($4);}
| LBRACE MethodDeclarationStatements RETURN Expression SC RBRACE {$$ = new Node("Method body", "");$$->children.push_back($2); $$->children.push_back($4);}
| LBRACE MethodDeclarationVars MethodDeclarationStatements RETURN Expression SC RBRACE {$$ = new Node("Method body", "");$$->children.push_back($2); $$->children.push_back($3); $$->children.push_back($5);};


MethodDeclarationParams: Type Identifier {$$ = new Node("Method param", ""); $$->children.push_back($1); $$->children.push_back($2);}
| MethodDeclarationParams COMMA Type Identifier {$$ = new Node("Method params", ""); $$->children.push_back($1); $$->children.push_back($3);};

MethodDeclarationVars: VarDeclaration {$$ = $1;}
| MethodDeclarationVars VarDeclaration {$$ = new Node("Method vars", ""); $$->children.push_back($1); $$->children.push_back($2);};

MethodDeclarationStatements: Statement {$$ = $1;}
| MethodDeclarationStatements Statement {$$ = new Node("Method statements", ""); $$->children.push_back($1); $$->children.push_back($2);};

VarDeclaration:
  Type Identifier SC {$$ = new Node("Variable", ""); $$->children.push_back($1); $$->children.push_back($2);};

Type: INT LBRACKET RBRACKET {$$ = new Node("Type", "int[]");}
  | BOOL {$$ = new Node("Type", "boolean");}
  | INT {$$ = new Node("Type", "int");}
  | Identifier {$$ = new Node("Type", "Identifier"); $$->children.push_back($1);};

Statement: 
  LBRACE RBRACE { $$ = new Node("Empty statement", ""); }
  | LBRACE LRStatement RBRACE {$$ = $2;}
  | IF LPAR Expression RPAR Statement ELSE Statement {$$ = new Node(" IF ELSE", ""); $$->children.push_back($3); $$->children.push_back($5); $$->children.push_back($7);}
  | WHILE LPAR Expression RPAR Statement {$$ = new Node("While loop", ""), $$->children.push_back($3), $$->children.push_back($5);}
  | PRINTLN LPAR Expression RPAR SC {$$ = new Node("Print", ""); $$->children.push_back($3);}
  | Identifier EQASSIGN Expression SC {$$ = new Node("Assign", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | Identifier LBRACKET Expression RBRACKET EQASSIGN Expression SC {$$ = new Node("Array assign", ""); $$->children.push_back($1); $$->children.push_back($3); $$->children.push_back($6);};

LRStatement: 
  Statement {$$ = $1;}
| LRStatement Statement { $$ = new Node("Statements", ""); $$->children.push_back($1); $$->children.push_back($2);};


Expression:
  orExpression {$$ = $1;};

orExpression: orExpression OR andExpression {$$ = new Node("|| expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | andExpression {$$ = $1;};

andExpression: andExpression AND eqExpression {$$ = new Node("&& expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | eqExpression {$$=$1;};

eqExpression: eqExpression EQ lgExpression {$$ = new Node("== expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | lgExpression {$$ = $1;};

lgExpression: lgExpression LT addsubExpression {$$ = new Node("< expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | lgExpression GT addsubExpression {$$ = new Node("> expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | addsubExpression {$$ = $1;};

addsubExpression: addsubExpression ADDSIGN multdivExpression {$$ = new Node("+ expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
                | addsubExpression SUBSIGN multdivExpression {$$ = new Node("- expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | multdivExpression {$$ = $1;};

multdivExpression: multdivExpression MULTSIGN notExpression {$$ = new Node("* expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | multdivExpression DIVSIGN notExpression {$$ = new Node("/ expression", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | notExpression {$$ = $1;};

notExpression: NOT parExpression {$$ = new Node("! expression", ""); $$->children.push_back($2);}
  | parExpression {$$ = $1;};

parExpression: Identifier {$$ = $1;}
  | Int {$$ = $1;}
  | TRUE {$$ = new Node("TRUE", "");}
  | FALSE {$$ = new Node("FALSE", "");}
  | THIS {$$ = new Node("THIS", "");}
  | LPAR Expression RPAR {$$ = new Node("(Expression)", ""); $$->children.push_back($2);} 
  | Expression LBRACKET Expression RBRACKET {$$ = new Node("Array access", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | Expression DOT Identifier LPAR comExpression RPAR {$$ = new Node("Method call", ""); $$->children.push_back($1); $$->children.push_back($3); $$->children.push_back($5);}
  | Expression DOT Identifier LPAR RPAR {$$ = new Node("Method call", ""); $$->children.push_back($1); $$->children.push_back($3);}
  | Expression DOT LENGTH {$$ = new Node("Exp.length", ""); $$->children.push_back($1);}
  | NEW INT LBRACKET Expression RBRACKET {$$ = new Node("new int[] exp", ""); $$->children.push_back($4);}
  | NEW Identifier LPAR RPAR {$$ = new Node("Object instantiation", ""); $$->children.push_back($2);};

comExpression: Expression {$$ = $1;}
  | comExpression COMMA Expression {$$ = new Node("Multiple Exp", ""); $$->children.push_back($1); $$->children.push_back($3);};

// Par -> NOT -> * / -> +- -> LT / GT -> EQ -> AND -> OR
Identifier: ID {
  $$ = new Node("ID", $1);
};

Int: INT_LITERAL {$$ = new Node("Int", $1);};
| SUBSIGN INT_LITERAL {$$ = new Node("Int", "-" + $2);}
