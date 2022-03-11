#ifndef BYTECODE_H
#define BYTECODE_H

#include "symbol_table.h"
#include <map>



// typedef union Argument {
//     int i;
//     string m;
// } Argument;


class Instruction {
    public:
    int id;
    string argument;

    void print(ofstream *outStream);
};

class MethodBlock {
    public:
    string name;
    vector<Instruction*> instructions;
    void print(ofstream *outSteam);
};

class BCMethod {
    public:
    vector<Variable> variables;
    vector<MethodBlock*> methodBlocks;
    void print(ofstream *outStream);
};


class Program {
    public:
    map<string, BCMethod*> methods;
    void print();

};

#endif