#include "bytecode.h"
#include <map>



Program* readFile(const char* filename);

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Too few arguments" << endl;
        return 0;
    }
    Program* program = readFile(argv[1]);
    return 0;
}

Program* readFile(const char* filename) {
    Program* program = new Program();

    ifstream infile(filename);

    string line;
    while (getline(infile, line)) {
        // 1. Check for : and . and main
        // 2. Create new methods
        // 3. Create method blocks
        // 4. Read instructions
        // 5. Parse instructions (split, get id, etc...)
        // 6. Add variables
        // 7. Create symboltable-like structure for method calls?

    }
    
    return program;  

}