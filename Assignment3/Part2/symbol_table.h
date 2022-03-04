#ifndef SYMBOL_TABLE_H
#define	SYMBOL_TABLE_H

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
        Record(string id, string type);

        void setID(string newID);

        string getID();

        void setType(string newType);

        string getType();

        string printRecord();
};

class Variable : public Record {

    public:
        Variable(string name, string type);
};

class Method : public Record {
    vector<Variable*> params;
    unordered_map<string, Variable*> variables;

    public:
        Method(string name, string type);
        
        void addVariable(Variable* var);

        void addParams(Variable* var);

        Variable* getParam(int idx);

        int getNumParams();
};

class Class : public Record {
    unordered_map<string, Variable*> variables;
    unordered_map<string, Method*> methods;

    public:
    Class(string name, string type);

    void addVariable(Variable* var);

    void addMethod(Method* method);

    Variable* lookupVariable(string name);

    Method* lookupMethod(string name);
};

class Scope {
    int next = 0;
    Scope* parentScope;
    vector<Scope*> childrenScopes;
    unordered_map<std::string, Record*> records;
    string scopeName;
    Record* scopeRecord;

    public:
        Scope(Scope *parent, string name, Record *record);

        void put(string key, Record *record);

        Record* getScopeRecord ();

        Scope* parent();

        string getScopeName();

        Scope* nextChild(string name, Record* record);

        Record* lookup(string key);
        
        void resetScope();

        void printScope(int &count, ofstream *outStream);
        
};

class SymbolTable
{
    Scope *root;
    Scope *current;
    unordered_map<string, vector<Record*>> variables;
    
    public:
        SymbolTable();

        vector<Record*> getVariables(string name);

        void enterScope(string name, Record* record);

        void exitScope();

        void put(string key, Record *item);

        Record* lookup(string key);

        Scope* GetCurrentScope();

        void printTable();
        
        void resetTable();

};

void traverseTree(Node* node, SymbolTable* symbolTable);

#endif