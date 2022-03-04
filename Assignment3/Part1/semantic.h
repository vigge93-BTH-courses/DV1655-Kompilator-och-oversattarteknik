#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "symbol_table.h"
#include "stdlib.h"


string traverseTreeSemantic(SymbolTable* symbolTable, Node* node);

int semantic_analysis(SymbolTable* symbolTable, Node* root);

#endif