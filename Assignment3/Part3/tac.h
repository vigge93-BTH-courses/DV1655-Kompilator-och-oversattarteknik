#ifndef TAC_H
#define TAC_H
#include "Node.h"
#include "stdlib.h"
#include "bytecode.h"

class Tac {
protected:
    string op, lhs, rhs, result;
public:
    
    Tac(string _op, string _lhs, string _rhs, string _result);
    
    string getOp();
    string getLhs();
    string getRhs();
    string getResult();

    void setOp(string op);
    void setLhs(string lhs);
    void setRhs(string rhs);
    void setResult(string result);

    virtual void generateCode(MethodBlock* method);

    
    
    virtual string get_str();

    void dump();
};

class Expression : public Tac {
    public:
    Expression(string _op, string _lhs, string _rhs, string _result);

    string get_str();
    void generateCode(MethodBlock* method);
};

class UnaryExpression : public Tac {
    public:
    UnaryExpression(string _op, string _rhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class Copy : public Tac {
    public:
    Copy(string _lhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class CopyArray : public Tac {
    public:
    CopyArray(string _lhs, string _rhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class ArrayAccess : public Tac {
    public:
    ArrayAccess(string _index, string _lhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class MethodCall : public Tac {
    public:
     MethodCall(string _f, string _N, string _result);

     string get_str();
    void generateCode(MethodBlock* method);
};

class Jump : public Tac {
    public:
    Jump(string _label);
    string get_str();
    void generateCode(MethodBlock* method);
};

class CondJump : public Tac {

    public:
    CondJump(string _x, string _label);

    string get_str();
    void generateCode(MethodBlock* method);
};

class New : public Tac {

    public:
    New(string _rhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class NewArray : public Tac {
    
    public:
    NewArray(string _type, string _N, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class Length : public Tac {
    public:
    Length(string _lhs, string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class Param : public Tac {
    public:
    Param(string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class MethodParam : public Tac {
    public:
    MethodParam(string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class Return : public Tac {
    public:
    Return(string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

class Print : public Tac {
    public:
    Print(string _result);
    string get_str();
    void generateCode(MethodBlock* method);
};

#endif