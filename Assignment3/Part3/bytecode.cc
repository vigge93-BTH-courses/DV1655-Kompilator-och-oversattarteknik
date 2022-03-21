#include "bytecode.h"


void Instruction::print(ofstream* outStream) {
    string inst_name;
    switch(id) {
        case 0:
            inst_name = "iload";
            break;
        case 1:
            inst_name = "iconst";
            break;
        case 2:
            inst_name = "istore";
            break;
        case 3:
            inst_name = "iadd";
            break;
        case 4:
            inst_name = "isub";
            break;
        case 5:
            inst_name = "imul";
            break;
        case 6:
            inst_name = "idiv";
            break;
        case 7:
            inst_name = "ilt";
            break;
        case 8:
            inst_name = "ieq";
            break;
        case 9:
            inst_name = "iand";
            break;
        case 10:
            inst_name = "ior";
            break;
        case 11:
            inst_name = "inot";
            break;
        case 12:
            inst_name = "goto";
            break;
        case 13:
            inst_name = "iffalse";
            break;
        case 14:
            inst_name = "invokevirtual";
            break;
        case 15:
            inst_name = "ireturn";
            break;
        case 16:
            inst_name = "print";
            break;
        case 17:
            inst_name = "stop";
            break;
    }
    *outStream << "    " << inst_name << " " << argument << endl;
}

void MethodBlock::print(ofstream *outStream) {
    *outStream << name << ":" << endl;
    for (Instruction* i : instructions) {
        i->print(outStream);
    }
}

void BCMethod::print(ofstream *outStream) {
    for (MethodBlock* i : methodBlocks) {
        i->print(outStream);
    }
}

Instruction* BCMethod::getInstruction(int pc) {
    int i = 0;
    MethodBlock* currentBlock; 
    while (pc >= 0) {
        currentBlock = methodBlocks[i++];
        pc -= currentBlock->instructions.size();
    }
    return currentBlock->instructions[currentBlock->instructions.size() + pc];
}

int BCMethod::getPcValue(string block) {
    int i = 0;
    int pc = 0;
    MethodBlock* currentBlock = methodBlocks[i++];
    while (currentBlock->name != block){
        pc += currentBlock->instructions.size();
        currentBlock = methodBlocks[i++];

    }
    return pc;
}

void Program::print()
{
    ofstream outStream;
    outStream.open("output.bc");

    for(pair<string, BCMethod*> kvp : methods) {
        // outStream << kvp.first  << ":" << endl;
        kvp.second->print(&outStream);
    }
    std::cout << "Printed Bytecode!" << std::endl;
    outStream.close();
}

