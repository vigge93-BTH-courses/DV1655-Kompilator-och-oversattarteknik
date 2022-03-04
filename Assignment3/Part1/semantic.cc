#include "symbol_table.h"
#include "stdlib.h"
#include "semantic.h"


string traverseTreeSemantic(SymbolTable* symbolTable, Node* node) {
    if (node->type == "Program") {
        for(auto i=node->children.begin(); i!=node->children.end(); i++) {
            symbolTable->enterScope("", NULL);
            traverseTreeSemantic(symbolTable, *i);
            symbolTable->exitScope();
        }
        return "void";
    } else if (node->type == "Class") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        return "void";
    } else if (node->type == "Main Class") {
        symbolTable->enterScope("", NULL);
        for (auto i=++(node->children.begin()); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        symbolTable->exitScope();
        return "void";
    } else if (node->type == "Class body") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        return "void";
    } else if (node->type == "Method list") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            symbolTable->enterScope("", NULL);
            traverseTreeSemantic(symbolTable, *i);
            symbolTable->exitScope();
        }
        return "void";
    } else if (node->type == "Method") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        auto retType = traverseTreeSemantic(symbolTable, node->children.back());
        if (node->children.front()->value != retType) {
            cout << "TypeError: return type missmatch, cannot convert " << retType << " to " << node->children.front()->value << endl;
        }
        return node->children.front()->value;
    } else if (node->type == "Method body") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        return traverseTreeSemantic(symbolTable, node->children.back());
    } else if (node->type == "Method Statements") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        return "void";
    } else if (node->type == "Statements") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        return "void";
    } else if (node->type == "IF ELSE") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "boolean") {
            cout << "Cannot convert from '" << lhs << "' to boolean" << endl;
            exit(-1);
        }
        return "void";
    } else if (node->type == "While loop") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTreeSemantic(symbolTable, *i);
        }
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "boolean") {
            cout << "Cannot convert from '" << lhs << "' to boolean" << endl;
            exit(-1);
        }
        return "void";
    } else if (node->type == "Print") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "int") {
            cout << "Cannot convert from '" << lhs << "' to int" << endl;
            exit(-1);
        }
        return "void";
    } else if (node->type == "Assign") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != rhs) {
            cout << "TypeError: type missmatch, '" << lhs << "' does not match with '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "void";
    } else if (node->type == "Array assign") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto idx = traverseTreeSemantic(symbolTable, *(++(node->children.begin())));
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int[]"  ) {
            cout << "The type of the expression must be an array type but it resolved to " << lhs << endl;
            exit(-1);
        } else if (rhs != "int") {
            cout << "Cannot convert from '" << rhs << "' to int" << endl;
            exit(-1);
        } else if (idx != "int") {
            cout << "Cannot convert from '" << idx << "' to int" << endl;
            exit(-1);
        }
        return "void";
    } else if (node->type == "|| expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "boolean" || rhs != "boolean") {
            cout << "TypeError: Unsupported operand type(s) for ||: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "boolean";
    } else if (node->type == "&& expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "boolean" || rhs != "boolean") {
            cout << "TypeError: Unsupported operand type(s) for &&: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "boolean";
    } else if (node->type == "== expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != rhs || (rhs != "int" && rhs != "boolean")) {
            cout << "TypeError: Unsupported operand type(s) for ==: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "boolean";
    } else if (node->type == "< expression" || node->type == "> expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int" || rhs != "int") {
            cout << "TypeError: Unsupported operand type(s) for < or >: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "boolean";
    } else if (node->type == "+ expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int" || rhs != "int") {
            cout << "TypeError: Unsupported operand type(s) for +: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "int";
    } else if (node->type == "- expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int" || rhs != "int") {
            cout << "TypeError: Unsupported operand type(s) for -: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "int";
    } else if (node->type == "* expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int" || rhs != "int") {
            cout << "TypeError: Unsupported operand type(s) for *: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "int";
    } else if (node->type == "/ expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int" || rhs != "int") {
            cout << "TypeError: Unsupported operand type(s) for /: '" << lhs << "' and '" << rhs << "'"<< endl;
            exit(-1);
        }
        return "int";
    } else if (node->type == "! expression") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "boolean") {
            cout << "TypeError: Unsupported operand type for !: '" << lhs << "'" << endl;
            exit(-1);
        }
        return "boolean";
    }  else if (node->type == "(Expression)") {
        return traverseTreeSemantic(symbolTable, node->children.front());
    } else if (node->type == "Array access") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        auto rhs = traverseTreeSemantic(symbolTable, node->children.back());
        if (lhs != "int[]") {
            cout << "The type of the expression must be an array type but it resolved to " << lhs << endl;
            exit(-1);
        } else if (rhs != "int") {
            cout << "Cannot convert from '" << rhs << "' to int" << endl;
            exit(-1);
        }
        return "int";
    } 
    else if (node->type == "Method call") {
        auto caller = traverseTreeSemantic(symbolTable, node->children.front());
        auto callerRecord = symbolTable->lookup(caller);
        auto callerClass = (Class*)symbolTable->lookup(callerRecord->getType());
        if (callerClass == NULL) {
            cout << "Error: Method " << (*(++node->children.begin()))->value << " is not defined on instance of type " << callerRecord->getType() << endl;
            exit(-1);
        }
        auto childrenFront = node->children.begin();
        advance(childrenFront, 1);
        auto method = callerClass->lookupMethod((*childrenFront)->value);
        if (method == NULL) {
            cout << "Error: Method " << (*childrenFront)->value << " is not defined on instance of type " << callerRecord->getType() << endl;
            exit(-1);
        }
        advance(childrenFront, 1);
        if (childrenFront == node->children.end()) {
            if (method->getNumParams() != 0) {
                cout << "Not the correct number of parameters for method " << method->getID() << endl;
                exit(-1);
            }
        } else if (method->getNumParams() != (*childrenFront)->children.size()) {
            cout << "Not the correct number of parameters for method " << method->getID() << endl;
            exit(-1);
        } else {
            int idx = 0;
            for (auto i=(*childrenFront)->children.begin(); i!=(*childrenFront)->children.end(); i++) {
                auto type = traverseTreeSemantic(symbolTable, *i);
                if (type != method->getParam(idx)->getType()) {
                    cout << "TypeError: type missmatch, '" << type << "' does not match with '" << method->getParam(idx)->getType() << "' in method call " << method->getID() << endl;
                    exit(-1);
                }
                idx++;
            }
        }
        return method->getType();
    } else if (node->type == "Exp.length") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "int[]") {
            cout << "The type " << lhs << " does not have a field length" << endl;
            exit(-1);
        }
        return "int";
    } else if (node->type == "new int[] exp") {
        auto lhs = traverseTreeSemantic(symbolTable, node->children.front());
        if (lhs != "int") {
            cout << "Cannot convert from '" << lhs << "' to int" << endl;
            exit(-1);
        }
        return "int[]";
    } else if (node->type == "Object instantiation") {
        return traverseTreeSemantic(symbolTable, node->children.front());;
    } else if (node->type == "Variable") {
        Node* type = (*node->children.begin());
        return type->value;
    } else if (node->type == "ID" || node->type == "this") {
        auto id = symbolTable->lookup(node->value);
        if (id == NULL) {
            cout << "Error: Identifier " << node->value << " is not defined" << endl;
            exit(-1);
        }
        return id->getType();
    } else if (node->type == "Int") {
        return "int";
    } else if (node->type == "true" || node->type == "false") {
        return "boolean";
    }
    return node->value;
}


int semantic_analysis(SymbolTable* symbolTable, Node* root) {
    symbolTable->resetTable();
    return !traverseTreeSemantic(symbolTable, root).empty();
}