#include "bytecode.h"


void Instruction::print() {
    cout << "instruction: " << id << " Argument: " << argument->i << argument->m << endl;
}

void BCMethod::print() {
    for (Instruction* i : instructions) {
        i->print();
    }
}

void Program::print()
{
    for(pair<string, BCMethod*> kvp : methods) {
        cout << kvp.first << endl;
        kvp.second->print();
    }
}

