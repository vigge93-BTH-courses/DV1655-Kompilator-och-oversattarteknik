#ifndef BYTECODE_H
#define BYTECODE_H

#include "symbol_table.h"



typedef union Argument {
    int i;
    string m;
} Argument;


class Instruction {
    public:
    int id;
    Argument *argument;

    void print();
};

class BCMethod {
    public:
    vector<Variable> variables;
    vector<Instruction*> instructions;
    void print();
};


class Program {
    public:
    unordered_map<string, BCMethod*> methods;
    void print();

};

#endif