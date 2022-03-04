#ifndef CFG_H
#define CFG_H
#include "tac.h"
#include "symbol_table.h"
#include "Node.h"
#include <algorithm>

class BBlock {
    public:
    inline static int blockCounter = 0;
    inline static int tempCounter = 0;
    string name;
    vector<Tac*> tacInstructions;
    Tac *condition;
    BBlock *trueExit, *falseExit;

    BBlock() : trueExit(NULL), falseExit(NULL), condition(NULL), name(genBlockName()) {}

    static string genTempName();

    static string genBlockName();

};

extern BBlock *currentBlock;
extern vector<BBlock*> methods;
extern vector<string> renderedBlocks;

string TraverseTreeTac (SymbolTable* symbolTable, Node* node);
void create_block_cfg(BBlock* block, ofstream *outStream);
void create_cfg(BBlock* block);

#endif