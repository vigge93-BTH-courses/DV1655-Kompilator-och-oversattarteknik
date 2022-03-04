#include<iostream>
#include "parser.tab.hh"
#include "symbol_table.h"
#include "semantic.h"
#include "cfg.h"
#include "bytecode.h"

extern Node* root;
extern FILE* yyin;

void yy::parser::error(std::string const&err)
{
  std::cout << "Cannot generate a syntax tree for this input: " << err << std::endl;
}

int main(int argc, char **argv)
{
  //Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
  if(argc > 1) {
    if(!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return 1;
    }
  }

  yy::parser parser;
	  
  if(!parser.parse()) {
    root->print_tree();
    root->generate_tree();
    SymbolTable *symbolTable = new SymbolTable();
    traverseTree(root, symbolTable);
    symbolTable->printTable();
    semantic_analysis(symbolTable, root);
    
    BBlock *entry = new BBlock();
    currentBlock = entry;
    methods.push_back(currentBlock);
    TraverseTreeTac(symbolTable, root);
    create_cfg(entry);
    Program* program = new Program();

  }

  return 0;
}