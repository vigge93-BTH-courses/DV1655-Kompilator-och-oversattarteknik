#include "bytecode.h"
#include <unordered_map>
#include <algorithm>
#include <stack>

#pragma region define
#define ILOAD 0
#define ICONST 1
#define ISTORE 2
#define IADD 3
#define ISUB 4
#define IMUL 5
#define IDIV 6
#define ILT 7
#define IEQ 8
#define IAND 9
#define IOR 10
#define INOT 11
#define GOTO 12
#define IFFALSE 13
#define INVOKEVIRTUAL 14
#define IRETURN 15
#define PRINT 16
#define STOP 17
#pragma endregion

vector<string> split_string(string inp, string del);

Program* readFile(const char* filename);

void interpret(Program* program);

class Activation {
    public:
        BCMethod* method;
        unordered_map<string, int> variables;
        int pc = 0;
        Activation(BCMethod* _method) {
            method = _method;
            for (auto i : _method->variables) {
                variables[i] = 0;
            }
        }

        Instruction* getNextInstruction () {
            return method->getInstruction(pc++);
        }

        void jump(string block) {
            pc = method->getPcValue(block);
        }
};

const unordered_map<string, int> instructions = {
    {"iload", 0},
    {"iconst", 1},
    {"istore", 2},
    {"iadd", 3},
    {"isub", 4},
    {"imul", 5},
    {"idiv", 6},
    {"ilt", 7},
    {"ieq", 8},
    {"iand", 9},
    {"ior", 10},
    {"inot", 11},
    {"goto", 12},
    {"iffalse", 13},
    {"invokevirtual", 14},
    {"ireturn", 15},
    {"print", 16},
    {"stop", 17}
};


int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Too few arguments" << endl;
        return 0;
    }
    Program* program = readFile(argv[1]);
    interpret(program);
    return 0;
}


Program* readFile(const char* filename) {
    Program* program = new Program();
    BCMethod* currentMethod;
    MethodBlock* currentMethodBlock;
    ifstream infile(filename);

    string line;
    while (getline(infile, line)) {
        // cout << line << endl;
        if (line == "main:") {
            currentMethod = new BCMethod();
            currentMethodBlock = new MethodBlock();
            currentMethodBlock->name = "main";
            currentMethod->methodBlocks.push_back(currentMethodBlock);
            program->methods["main"] = currentMethod;
        } else if (line.find(".") != string::npos && line.find(":") != string::npos) {
            line.pop_back();
            currentMethod = new BCMethod();
            currentMethodBlock = new MethodBlock();
            currentMethodBlock->name = line;
            currentMethod->methodBlocks.push_back(currentMethodBlock);
            program->methods[line] = currentMethod;

        } else if (line.find(":") != string::npos) {
            currentMethodBlock = new MethodBlock();
            line.pop_back();
            currentMethodBlock->name = line;
            currentMethod->methodBlocks.push_back(currentMethodBlock);
        } else {
            Instruction* inst = new Instruction();
            vector<string> lineSplit = split_string(line, " ");
            inst->id = instructions.at(lineSplit[0]);
            if (lineSplit.size() > 1) {
                inst->argument = lineSplit[1];
                if ((lineSplit[0] == "iload" || lineSplit[0] == "istore") && find(currentMethod->variables.begin(), currentMethod->variables.end(), lineSplit[1]) != currentMethod->variables.end()) {
                    currentMethod->variables.push_back(lineSplit[1]);
                }
            }
            currentMethodBlock->instructions.push_back(inst);
        }
    }   
    return program;  

}

void interpret(Program* program) {
    stack<Activation*> activationStack;
    stack<int> dataStack;
    Activation* currentActivation = new Activation(program->methods["main"]);
    int instructionId = -1;
    int v1, v2, v;
    while(instructionId != STOP) {
        Instruction* instruction = currentActivation->getNextInstruction();
        // cout << instruction->id << " " << instruction->argument << endl;
        instructionId = instruction->id;
        switch(instruction->id) {
            case ILOAD:
                dataStack.push(currentActivation->variables[instruction->argument]);
                break;
            case ICONST:
                dataStack.push(stoi(instruction->argument));
                break;
            case ISTORE:
                currentActivation->variables[instruction->argument] = dataStack.top();
                dataStack.pop();
                break;
            case IADD:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v1 + v2);
                break;
            case ISUB:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 - v1);
                break;
            case IMUL:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 * v1);
                break;
            case IDIV:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 / v1);
                break;
            case ILT:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 < v1);
                break;
            case IEQ:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 == v1);
                break;
            case IAND:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 * v1 != 0);
                break;
            case IOR:
                v1 = dataStack.top();
                dataStack.pop();
                v2 = dataStack.top();
                dataStack.pop();
                dataStack.push(v2 + v1 != 0);
                break;
            case INOT:
                v = dataStack.top();
                dataStack.pop();
                dataStack.push(v == 0);
                break;
            case GOTO:
                currentActivation->jump(instruction->argument);
                break;
            case IFFALSE:
                v = dataStack.top();
                dataStack.pop();
                if(v == 0)
                {
                    currentActivation->jump(instruction->argument);
                }
                break;
            case INVOKEVIRTUAL:
            {
                activationStack.push(currentActivation);
                BCMethod* callMethod = program->methods[instruction->argument];
                currentActivation = new Activation(callMethod);
                break;  
            }
            case IRETURN:
                currentActivation = activationStack.top();
                activationStack.pop();
                break;
            case PRINT:
                v = dataStack.top();
                dataStack.pop();
                cout << v << endl;
                break;
            case STOP:
                break;
        }
    }

}

vector<string> split_string(string inp, string del) {
    vector<string> res;
    int str_pos = 0;
    while (str_pos != string::npos) {
        str_pos = inp.find(del, str_pos);
        string part = inp.substr(0, str_pos);
        inp.erase(0, str_pos + 1);
        if (str_pos == 0) continue;
        res.push_back(part);
    }
    return res;
}