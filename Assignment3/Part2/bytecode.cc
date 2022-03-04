#include "symbol_table.h"

class Program {
    public:
    unordered_map<string, Method*> methods;
    void print()
    {
        for(pair<string, Method*> kvp : methods) {
            cout << kvp.first << endl;
            kvp.second->print();
        }
    }

};

class Method {
    public:
    vector<Variable> variables;
    vector<Instruction*> instructions;
    void print() {
        for (Instruction* i : instructions) {
            i->print();
        }
    }
};

typedef union Argument {
    int i;
    string m;
} Argument;

class Instruction {
    public:
    int id;
    Argument *argument;

    void print() {
        cout << "instruction: " << id << " Argument: " << argument->i << argument->m << endl;
    }
};