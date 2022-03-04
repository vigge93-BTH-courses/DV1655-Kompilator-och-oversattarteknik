#include "tac.h"

#pragma region Tac
Tac::Tac(string _op, string _lhs, string _rhs, string _result) : op(_op), lhs(_lhs), rhs(_rhs), result(_result) {}
    
string Tac::getOp() { return op; }
string Tac::getLhs() { return lhs; }
string Tac::getRhs() { return rhs; }
string Tac::getResult() { return result; }

void Tac::setOp(string op) { op = op; }
void Tac::setLhs(string lhs) { lhs = lhs; }
void Tac::setRhs(string rhs) { rhs = rhs; }
void Tac::setResult(string result) { result = result; }
    
string Tac::get_str() { return ""; }
void Tac::generateCode() {}

void Tac::dump()
{
    fprintf(stdout, "%s := %s %s %s", result, lhs, op, rhs);
}
#pragma endregion

#pragma region Expression
Expression::Expression(string _op, string _lhs, string _rhs, string _result) : Tac(_op, _lhs, _rhs, _result) {}

string Expression::get_str() {
    return result + " := " + lhs + " " + op + " " + rhs ;
}
void Expression::generateCode() {}
#pragma endregion

#pragma region UnaryExpression
UnaryExpression::UnaryExpression(string _op, string _rhs, string _result) : Tac(_op, "", _rhs, _result) {}

string UnaryExpression::get_str() {
    return result + " := " + op + " " + rhs ;
}

void UnaryExpression::generateCode() {}
#pragma endregion

#pragma region Copy
Copy::Copy(string _lhs, string _result) : Tac("Copy", _lhs, "", _result) {}

string Copy::get_str() {
    return result + " := " + lhs ;
}

void Copy::generateCode() {}
#pragma endregion

#pragma region CopyArray
CopyArray::CopyArray(string _lhs, string _rhs, string _result) : Tac("CopyArray", _lhs, _rhs, _result) {}

string CopyArray::get_str() {
    return result + "[" + lhs + "] := " + rhs ;
}

void CopyArray::generateCode() {}
#pragma endregion

#pragma region ArrayAccess
ArrayAccess::ArrayAccess(string _index, string _lhs, string _result) : Tac("ArrayAccess", _lhs, _index, _result) {}

string ArrayAccess::get_str() {
        return result + " := " + lhs + "[" + rhs + "]" ;
    }

void ArrayAccess::generateCode() {}
#pragma endregion

#pragma region MethodCall
MethodCall::MethodCall(string _f, string _N, string _result) : Tac("call", _f, _N, _result) 
     {}

string MethodCall::get_str() {
    return result + " := " + op + " " + lhs + ", " + rhs ;
}

void MethodCall::generateCode() {}
#pragma endregion

#pragma region Jump
Jump::Jump(string _label) : Tac("goto", "", "", _label) { }
string Jump::get_str() {
        return op + " " + result ;
}
void Jump::generateCode() {}
#pragma endregion

#pragma region CondJump
CondJump::CondJump(string _x, string _label) : Tac("iffalse", _x, "goto", _label) {}

string CondJump::get_str() {
    return op + " " + lhs + " " + rhs + " " + result ;
}

void CondJump::generateCode() {}
#pragma endregion

#pragma region New
New::New(string _rhs, string _result) : Tac("new", "",_rhs, _result) {}
string New::get_str() {
        return result + " := " + op + " " + rhs ;
    }

void New::generateCode() {}
#pragma endregion

#pragma region NewArray
NewArray::NewArray(string _type, string _N, string _result) : Tac("new", _type, _N, _result) {}
string NewArray::get_str() {
        return result + " " + lhs + " " + rhs ;
    }
void NewArray::generateCode() {}
#pragma endregion

#pragma region Length
Length::Length(string _lhs, string _result) : Tac("length", _lhs, "", _result) {}
string Length::get_str() {
        return result + " := " + op + " " + lhs ;
    }

void Length::generateCode() {}
#pragma endregion

#pragma region Param
Param::Param(string _result) : Tac("param", "", "", _result) {}
string Param::get_str() {
        return op + " " + result;
    }
void Param::generateCode() {}

#pragma endregion

#pragma region Return
Return::Return(string _result) : Tac("return", "", "", _result) {}
string Return::get_str() {
        return op + " " + result ;
    }

void Return::generateCode() {}
#pragma endregion

#pragma region Print
Print::Print(string _result) : Tac("print", "", "", _result) {}
string Print::get_str() {
        return op + " " + result ;
    }
void Print::generateCode() {}
#pragma endregion

