#ifndef BYTECODE_H
#define BYTECODE_H

#include <list>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "stdlib.h"
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
    vector<string> variables;
    vector<MethodBlock*> methodBlocks;
    void print(ofstream *outStream);
};


class Program {
    public:
    map<string, BCMethod*> methods;
    void print();

};

#endif