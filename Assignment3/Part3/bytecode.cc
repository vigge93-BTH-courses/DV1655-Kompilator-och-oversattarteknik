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
            inst_name = "iand";
            break;
        case 9:
            inst_name = "ior";
            break;
        case 10:
            inst_name = "inot";
            break;
        case 11:
            inst_name = "goto";
            break;
        case 12:
            inst_name = "iffalse goto";
            break;
        case 13:
            inst_name = "invokevirtual";
            break;
        case 14:
            inst_name = "ireturn";
            break;
        case 15:
            inst_name = "print";
            break;
        case 16:
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

void Program::print()
{
    ofstream outStream;
    outStream.open("bytecode.bc");

    for(pair<string, BCMethod*> kvp : methods) {
        // outStream << kvp.first  << ":" << endl;
        kvp.second->print(&outStream);
    }
    std::cout << "Printed Bytecode!" << std::endl;
    outStream.close();
}

