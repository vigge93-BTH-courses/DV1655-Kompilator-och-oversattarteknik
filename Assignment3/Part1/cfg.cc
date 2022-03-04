#include "cfg.h"

BBlock *currentBlock;
vector<BBlock*> methods;
vector<string> renderedBlocks;

string BBlock::genTempName() {
    return "_t" + to_string(tempCounter++);
}

string BBlock::genBlockName() {
    return "block" + to_string(blockCounter++);
}

string TraverseTreeTac (SymbolTable* symbolTable, Node* node) {
    if (node->type == "Method") {
        currentBlock = new BBlock();
        methods.push_back(currentBlock);
    }
    if (node->type == "Method body") {
        string name;
        for (auto i = node->children.begin(); i != node->children.end(); ++i) {
            name = TraverseTreeTac(symbolTable, *i);
        }
        Tac* ret = new Return(name);
        currentBlock->tacInstructions.push_back(ret);
        return "";
    } else if (node->type == "Empty statement") {
        return "";
    } else if (node->type == "IF ELSE") {
        BBlock* trueBlock = new BBlock();
        BBlock* falseBlock = new BBlock();
        BBlock* joinBlock = new BBlock();

        currentBlock->trueExit = trueBlock;
        currentBlock->falseExit = falseBlock;
        
        auto child = node->children.begin();
        string cond_name = TraverseTreeTac(symbolTable, *child);
        Tac* condition = new CondJump(cond_name, falseBlock->name);
        currentBlock->tacInstructions.push_back(condition);
        
        advance(child, 1);
        currentBlock = trueBlock;
        TraverseTreeTac(symbolTable, *child);
        currentBlock->trueExit = joinBlock;
        
        advance(child, 1);
        currentBlock = falseBlock;
        TraverseTreeTac(symbolTable, *child);
        currentBlock->trueExit = joinBlock;
        
        currentBlock = joinBlock;
        
        return "";
    } else if (node->type == "While loop") {
        BBlock* hBlock = new BBlock();
        BBlock* trueBlock = new BBlock();
        BBlock* joinBlock = new BBlock();

        currentBlock->trueExit = hBlock;
        
        auto child = node->children.begin();
        currentBlock = hBlock;
        string cond_name = TraverseTreeTac(symbolTable, *child);
        Tac* cond = new CondJump(cond_name, joinBlock->name);
        currentBlock->tacInstructions.push_back(cond);
        
        advance(child, 1);
        currentBlock = trueBlock;
        TraverseTreeTac(symbolTable, *child);
        Tac* jmp = new Jump(hBlock->name);
        currentBlock->tacInstructions.push_back(jmp);
        currentBlock->trueExit = hBlock;

        hBlock->trueExit = trueBlock;
        hBlock->falseExit = joinBlock;
        currentBlock = joinBlock;
        return "";
    } else if (node->type == "Print") {
        auto child = node->children.front();
        string name = TraverseTreeTac(symbolTable, child);
        Tac* in = new Print(name);
        currentBlock->tacInstructions.push_back(in);
        return "";
    } else if (node->type == "Assign") {
        auto child = node->children.begin();
        string name = (*child)->value;
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Copy(rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    } else if (node->type == "Array assign") {
        auto child = node->children.begin();
        string name = (*child)->value;
        advance(child, 1);
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new CopyArray(lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "|| expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("or", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;       
    } else if (node->type == "&& expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("and", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;       
    }
    else if (node->type == "== expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("==", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;       
    }
    else if (node->type == "< expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("<", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;       
    }
    else if (node->type == "> expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression(">", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;       
    }
    else if (node->type == "- expression") {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("-", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "+ expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child,1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("+", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "* expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child,1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("*", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;    
    }
    else if (node->type == "/ expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child,1);
        string rhs_name = TraverseTreeTac(symbolTable, *child);
        Tac* in = new Expression("/", lhs_name, rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name; 
    }
    else if (node->type == "! expression")
    {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "boolean"));
        auto child = node->children.front();
        string rhs_name = TraverseTreeTac(symbolTable, child);
        Tac* in = new UnaryExpression("!", rhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name; 
    }
    else if (node->type == "true") {
        return "true"; 
    }
    else if (node->type == "false") {
        return "false"; 
    }
    else if (node->type == "this") {
        return "this"; 
    }
    else if (node->type == "(Expression)") {
        return TraverseTreeTac(symbolTable, node->children.front()); 
    }
    else if (node->type == "Array access") {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.begin();
        string lhs_name = TraverseTreeTac(symbolTable, *child);
        advance(child, 1);
        string index = TraverseTreeTac(symbolTable, *child);
        Tac* in = new ArrayAccess(index, lhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name; 
    }
    else if (node->type == "Method call") {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, ""));
        auto child = node->children.begin();
        string caller = TraverseTreeTac(symbolTable, *child);
        Tac* caller_tac = new Param(caller);
        currentBlock->tacInstructions.push_back(caller_tac);
        advance(child, 1);
        string methodName = (*child)->value;
        int length = 1;
        advance(child, 1);
        if (child != node->children.end()) {
            TraverseTreeTac(symbolTable, *child);
            length = (*child)->children.size() + 1;
        }
        Tac* in = new MethodCall(methodName,to_string(length),name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "Exp.length") {
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int"));
        auto child = node->children.front();
        string lhs_name = TraverseTreeTac(symbolTable, child);
        Tac* in = new Length(lhs_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name; 
    }
    else if (node->type == "new int[] exp") {
        auto child = node->children.front();
        string len_name = TraverseTreeTac(symbolTable, child);
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, "int[]"));
        Tac* in = new NewArray("int[]", len_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "Object instantiation") {
        auto child = node->children.front();
        string class_name = TraverseTreeTac(symbolTable, child);
        string name = BBlock::genTempName();
        symbolTable->put(name, new Variable(name, class_name));
        Tac* in = new New(class_name, name);
        currentBlock->tacInstructions.push_back(in);
        return name;
    }
    else if (node->type == "Params") {
        for(auto i = node->children.begin(); i != node->children.end(); ++i) {
            string name = TraverseTreeTac(symbolTable, *i);
            Tac* in = new Param(name);
            currentBlock->tacInstructions.push_back(in);
        }
        return "";
    }
    else if (node->type == "ID")
    {
        return node->value;   
    }
    else if(node -> type == "Int") {
        return node->value;
    }
    else {
        for (auto i = node->children.begin(); i != node->children.end(); ++i) {
            TraverseTreeTac(symbolTable, *i);
        }
        return node->value;
    }
}

void create_block_cfg(BBlock* block, ofstream* outStream) {
    *outStream << block->name << " [label=\"" << block->name << "\\n";
    renderedBlocks.push_back(block->name);
    for (auto i = block->tacInstructions.begin(); i != block->tacInstructions.end(); ++i) {
        *outStream << (*i)->get_str() << "\\n";
    }
    *outStream << "\"];" << std::endl;
    if (block->trueExit != NULL) {
        *outStream << block->name << " -> " << block->trueExit->name << "[xlabel=\"true\"];" << endl;
        if (find(renderedBlocks.begin(), renderedBlocks.end(), block->trueExit->name) == renderedBlocks.end()) {
            create_block_cfg(block->trueExit, outStream);
        }
    }
    if (block->falseExit != NULL) {
        *outStream << block->name << " -> " << block->falseExit->name << "[xlabel=\"false\"];" << endl;
        create_block_cfg(block->falseExit, outStream);
    }
}

void create_cfg(BBlock* block) {
    std::ofstream outStream;
    outStream.open("cfg.dot");

    outStream << "digraph {\ngraph [splines=ortho]\nnode [shape=box]\n";
    for(auto i = methods.begin(); i != methods.end(); ++i) {
        create_block_cfg(*i, &outStream);
    }
    outStream << "\n}";
    outStream.close();
    std::cout << "Built a cfg-tree" << std::endl;
}
