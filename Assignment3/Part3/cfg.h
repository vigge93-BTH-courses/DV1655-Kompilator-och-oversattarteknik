#ifndef CFG_H
#define CFG_H
#include "tac.h"
#include "symbol_table.h"
#include "Node.h"
#include <algorithm>
#include "bytecode.h"


class BBlock {
    public:
    inline static int blockCounter = 0;
    inline static int tempCounter = 0;
    string name;
    vector<Tac*> tacInstructions;
    Tac *condition;
    BBlock *trueExit, *falseExit;

    BBlock() : trueExit(NULL), falseExit(NULL), condition(NULL), name(genBlockName()) {}

    BBlock(string _name) : trueExit(NULL), falseExit(NULL), condition(NULL), name(_name) {}
    void generateCode(BCMethod* method, string methodName);

    static string genTempName();

    static string genBlockName();

    static void generateBytecode(Program* program);

};

void genCondJumpIns(Tac* cond, BBlock* trueBlock, BBlock* falseBlock);

extern BBlock *currentBlock;
extern vector<BBlock*> methods;
extern vector<string> renderedBlocks;
extern vector<string> visited;

string TraverseTreeTac (SymbolTable* symbolTable, Node* node);
void create_block_cfg(BBlock* block, ofstream *outStream);
void create_cfg(BBlock* block);

#endif