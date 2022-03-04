#ifndef TAC_H
#define TAC_H
#include "Node.h"
#include "stdlib.h"

#include "Node.h"
#include "stdlib.h"

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
    
    virtual string get_str();

    void dump();
};

class Expression : public Tac {
    public:
    Expression(string _op, string _lhs, string _rhs, string _result);

    string get_str();
};

class UnaryExpression : public Tac {
    public:
    UnaryExpression(string _op, string _rhs, string _result);
    string get_str();
};

class Copy : public Tac {
    public:
    Copy(string _lhs, string _result);
    string get_str();
};

class CopyArray : public Tac {
    public:
    CopyArray(string _lhs, string _rhs, string _result);
    string get_str();
};

class ArrayAccess : public Tac {
    public:
    ArrayAccess(string _index, string _lhs, string _result);
    string get_str();
};

class MethodCall : public Tac {
    public:
     MethodCall(string _f, string _N, string _result);

     string get_str();
};

class Jump : public Tac {
    public:
    Jump(string _label);
    string get_str();
};

class CondJump : public Tac {

    public:
    CondJump(string _x, string _label);

    string get_str();
};

class New : public Tac {

    public:
    New(string _rhs, string _result);
    string get_str();
};

class NewArray : public Tac {
    
    public:
    NewArray(string _type, string _N, string _result);
    string get_str();
};

class Length : public Tac {
    public:
    Length(string _lhs, string _result);
    string get_str();
};

class Param : public Tac {
    public:
    Param(string _result);
    string get_str();
};

class Return : public Tac {
    public:
    Return(string _result);
    string get_str();
};

class Print : public Tac {
    public:
    Print(string _result);
    string get_str();
};

#endif