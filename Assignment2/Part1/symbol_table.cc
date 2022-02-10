#include "Node.h"
#include "stdlib.h"
#include <unordered_map>

class Record
{
    protected:
        string id;
        string type;
        string record;
    public:
        Record(string id, string type) {
            this->id = id;
            this->type = type;
        }

        void setID(string newID)
        {
            id = newID;
        }
        string getID()
        {
            return this->id;
        }

        void setType(string newType)
        {
            type = newType;
        }
        string getType()
        {
            return type;
        }

        string printRecord()
        {
            return "name: " + id + "; record: " + record + "; type: " + type;
        }
};

class Variable : public Record {

    public:
        Variable(string name, string type) : Record(name, type) {
            record = "Variable";
        }
};

class Method : public Record {
    unordered_map<string, Variable*> params;
    unordered_map<string, Variable*> variables;

    public:
        Method(string name, string type) : Record(name, type){
            record = "Method";
        }
        
        void addVariable(Variable* var)
        {
            variables[var->getID()] = var;
        }
        void addParams(Variable* var)
        {
            params[var->getID()] = var;
        }
};

class Class : public Record {
    unordered_map<string, Variable*> variables;
    unordered_map<string, Method*> methods;

    public:
    Class(string name, string type) : Record(name, type){
        record = "Class";
    }

    void addVariable(Variable* var) {
        variables[var->getID()] = var;
    }

    void addMethod(Method* method) {
        methods[method->getID()] = method;
    }

    Variable* lookupVariable(string name) {
        if(variables.find(name) != variables.end()) {
            return variables[name];
        }
        return NULL;
    }

    Method* lookupMethod(string name) {
        if(methods.find(name) != methods.end()) {
            return methods[name];
        }
        return NULL;
    }
};

class Scope {
    int next = 0;
    Scope* parentScope;
    vector<Scope*> childrenScopes;
    unordered_map<std::string, Record*> records;
    string scopeName;

    public:
        Scope(Scope *parent, string name) {
            parentScope = parent;
            scopeName = name;
        }

        void put(string key, Record *record) {
            records[key] = record;
        }

        Scope* parent() {
            return parentScope;
        }

        Scope* nextChild(string name) {
            Scope* nextChild;
            if (next == childrenScopes.size()) {
                nextChild = new Scope(this, name);
                childrenScopes.push_back(nextChild);
            } else {
                nextChild = childrenScopes.at(next);
            }
            next++;
            return nextChild;
        }

        Record* lookup(string key) {
            if (records.find(key) != records.end()) {
                return records[key];
            }
            if(parentScope == NULL) {
                return NULL;
            }
            return parentScope->lookup(key);
        }
        
        void resetScope() {
            next = 0;
            for(int i = 0; i < childrenScopes.size(); i++) {
                childrenScopes.at(i)->resetScope();
            }
        }

        void printScope(int &count, ofstream *outStream) {
            int id = count;
            
            *outStream << "n" << id << " [label=\"Symbol table: ("<< scopeName << ")\\n";
            for (auto i = records.begin(); i != records.end(); ++i) {
                *outStream << records[i->first]->printRecord() << "\\n";
            }
            *outStream << "\"];" << endl;

            for(auto i=childrenScopes.begin(); i!=childrenScopes.end(); i++) {
                int n = ++count;
                (*i)->printScope(count, outStream);
                *outStream << "n" << id << " -> n" << n << endl;
            }
        }
        
};

class SymbolTable
{
    Scope *root;
    Scope *current;
    
    public:
        SymbolTable() {
            root = new Scope(NULL, "Program");
            current = root;
        }

        void enterScope(string name = NULL)
        {
            current = current->nextChild(name); // new scope if needed
        }
        void exitScope()
        {
            current = current->parent();
        }

        void put(string key, Record *item)
        {
            current->put(key, item);
        }

        Record* lookup(string key)
        {   
            return current->lookup(key);
        }

        void printTable()
        {
            std::ofstream outStream;
            outStream.open("symbol_tree.dot");

            int count = 0;
            outStream << "digraph {" << std::endl;
            root->printScope(count, &outStream);
            outStream << "}" << std::endl;
            outStream.close();

            std::cout << "\nBuilt a symbol-tree:" << std::endl;
            
        }
        
        void resetTable()
        {
            root->resetScope(); //preperation for new traversal
    
        }

};

void traverseTree(Node* node, SymbolTable* symbolTable) { // Pre order
    if (node->type == "Program") {
        for(auto i=node->children.begin(); i!=node->children.end(); i++) {
            symbolTable->put((*i)->value, new Class((*i)->value, (*i)->value));
            symbolTable->enterScope("Class: " + (*i)->value);
            traverseTree(*i, symbolTable);
            symbolTable->exitScope();
        }
    } else if (node->type == "Class") {
        symbolTable->put("this", new Variable("this", node->value));
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Main Class") {
        symbolTable->put("this", new Variable("this", node->value));
        symbolTable->put("main", new Method("main", "void"));
        symbolTable->enterScope("Method: main");
        symbolTable->put(node->children.front()->value, new Variable(node->children.front()->value, "String[]"));
        symbolTable->exitScope();
    } else if (node->type == "Class body") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Class vars") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Method list") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            Node* type = *(*i)->children.begin();
            symbolTable->put((*i)->value, new Method((*i)->value, type->value));
            symbolTable->enterScope("Method: " + (*i)->value);
            traverseTree(*i, symbolTable);
            symbolTable->exitScope();
        }
    } else if (node->type == "Method") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Method params") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);        
        }
    } else if (node->type == "Method body") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Method vars") {
        for (auto i=node->children.begin(); i!=node->children.end(); i++) {
            traverseTree(*i, symbolTable);
        }
    } else if (node->type == "Variable") {
        Node* type = (*node->children.begin());
        auto end_idx = node->children.end();
        Node* name = (*(--end_idx));
        symbolTable->put(name->value, new Variable(name->value, type->value));
    }
}