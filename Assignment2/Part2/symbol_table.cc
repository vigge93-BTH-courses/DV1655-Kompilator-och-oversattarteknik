#include "Node.h"
#include "stdlib.h"
#include <unordered_map>
#include "symbol_table.h"


Record::Record(string id, string type) {
    this->id = id;
    this->type = type;
}

void Record::setID(string newID)
{
    id = newID;
}

string Record::getID()
{
    return this->id;
}

void Record::setType(string newType)
{
    type = newType;
}

string Record::getType()
{
    return type;
}

string Record::printRecord()
{
    return "name: " + id + "; record: " + record + "; type: " + type;
}


Variable::Variable(string name, string type) : Record(name, type) {
    record = "Variable";
}


Method::Method(string name, string type) : Record(name, type){
    record = "Method";
}

void Method::addVariable(Variable* var)
{
    variables[var->getID()] = var;
}

void Method::addParams(Variable* var)
{
    params.push_back(var);
}

Variable* Method::getParam(int idx) {
    return params.at(idx);
}

int Method::getNumParams() {
    return params.size();
}

Class::Class(string name, string type) : Record(name, type){
    record = "Class";
}

void Class::addVariable(Variable* var) {
    variables[var->getID()] = var;
}

void Class::addMethod(Method* method) {
    methods[method->getID()] = method;
}

Variable* Class::lookupVariable(string name) {
    if(variables.find(name) != variables.end()) {
        return variables[name];
    }
    return NULL;
}

Method* Class::lookupMethod(string name) {
    if(methods.find(name) != methods.end()) {
        return methods[name];
    }
    return NULL;
}


Scope::Scope(Scope *parent, string name, Record *record) {
    parentScope = parent;
    scopeName = name;
    scopeRecord = record;
}

void Scope::put(string key, Record *record) {
    records[key] = record;
}

Record* Scope::getScopeRecord () {
    return scopeRecord;
}

Scope* Scope::parent() {
    return parentScope;
}

string Scope::getScopeName() {
    return scopeName;
}

Scope* Scope::nextChild(string name, Record* record) {
    Scope* nextChild;
    if (next == childrenScopes.size()) {
        nextChild = new Scope(this, name, record);
        childrenScopes.push_back(nextChild);
    } else {
        nextChild = childrenScopes.at(next);
    }
    next++;
    return nextChild;
}

Record* Scope::lookup(string key) {
    if (records.find(key) != records.end()) {
        return records[key];
    }
    if(parentScope == NULL) {
        return NULL;
    }
    return parentScope->lookup(key);
}

void Scope::resetScope() {
    next = 0;
    for(int i = 0; i < childrenScopes.size(); i++) {
        childrenScopes.at(i)->resetScope();
    }
}

void Scope::printScope(int &count, ofstream *outStream) {
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
      

SymbolTable::SymbolTable() {
    root = new Scope(NULL, "Program", NULL);
    current = root;
}

void SymbolTable::enterScope(string name, Record* record)
{
    current = current->nextChild(name, record); // new scope if needed
}
void SymbolTable::exitScope()
{
    current = current->parent();
}

void SymbolTable::put(string key, Record *item)
{
    current->put(key, item);
}

Record* SymbolTable::lookup(string key)
{
    return current->lookup(key);
}

Scope* SymbolTable::GetCurrentScope() {
    return current;
}

void SymbolTable::printTable()
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

void SymbolTable::resetTable()
{
    root->resetScope(); //preperation for new traversal

}


void traverseTree(Node* node, SymbolTable* symbolTable) { // Pre order
    cout << (node->type) << endl;
    if (node->type == "Program") {
        for(auto i=node->children.begin(); i!=node->children.end(); i++) {
            if (symbolTable->lookup((*i)->value) != NULL) {
                cout << "Error: Class \"" << (*i)->value << "\" is already defined" << endl;
                exit(-1);
            }
            auto newClass = new Class((*i)->value, (*i)->value);
            symbolTable->put((*i)->value, newClass);
            symbolTable->enterScope("Class: " + (*i)->value, newClass);
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
        symbolTable->enterScope("Method: main", NULL);
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
            if ( symbolTable->lookup((*i)->value) != NULL) {
                cout << "Error: Identifier with name \"" << (*i)->value << "\" already exists" << endl;
                exit(-1);
            }
            Node* type = *(*i)->children.begin();
            auto new_method = new Method((*i)->value, type->value);
            symbolTable->put((*i)->value, new_method);
            ((Class*)symbolTable->GetCurrentScope()->getScopeRecord())->addMethod(new_method);
            symbolTable->enterScope("Method: " + (*i)->value, new_method);
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
            auto end_idx = (*i)->children.end();
            Node* name = (*(--end_idx));
            auto new_param = (Variable*)symbolTable->lookup(name->value);
            ((Method*)symbolTable->GetCurrentScope()->getScopeRecord())->addParams(new_param);
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
        if (symbolTable->lookup(name->value) != NULL) {
            cout << "Error: Identifier with name \"" << name->value << "\" already exists" << endl;
            exit(-1);
        }
        symbolTable->put(name->value, new Variable(name->value, type->value));
    }
}