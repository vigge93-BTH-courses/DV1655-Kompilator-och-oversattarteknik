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
void Tac::generateCode(MethodBlock* method) {}

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
void Expression::generateCode(MethodBlock* method) {
    Instruction* load_lhs_inst = new Instruction();
    Instruction* load_rhs_inst = new Instruction();
    Instruction* op_inst = new Instruction();
    Instruction* store_inst = new Instruction();
    if (op == ">") { // a > b => b < a
        string t = lhs;
        lhs = rhs;
        rhs = t;
    }
    if (lhs == "true") lhs = "1";
    else if (lhs == "false") lhs = "0";
    try {
        stoi(lhs);
        load_lhs_inst->id = 1; // iconst
    } catch (invalid_argument ex) {
        load_lhs_inst->id = 0; // iload
    }
    load_lhs_inst->argument = lhs;
    
    if (rhs == "true") rhs = "1";
    else if (rhs == "false") rhs = "0";
    try {
        stoi(rhs);
        load_rhs_inst->id = 1; // iconst
    } catch (invalid_argument ex) {
        load_rhs_inst->id = 0; // iload
    }
    load_rhs_inst->argument = rhs;
    
    if (op == "+") op_inst->id = 3;
    else if (op == "-") op_inst->id = 4;
    else if (op == "*") op_inst->id = 5;
    else if (op == "/") op_inst->id = 6;
    else if (op == "<") op_inst->id = 7;
    else if (op == ">") op_inst->id = 7; // Check order of lhs, rhs
    else if (op == "==") op_inst->id = 8;
    else if (op == "and") op_inst->id = 9;
    else if (op == "or") op_inst->id = 10;
    store_inst->id = 2;
    store_inst->argument = result;
    method->instructions.push_back(load_lhs_inst);
    method->instructions.push_back(load_rhs_inst);
    method->instructions.push_back(op_inst);
    method->instructions.push_back(store_inst);
}
#pragma endregion

#pragma region UnaryExpression
UnaryExpression::UnaryExpression(string _op, string _rhs, string _result) : Tac(_op, "", _rhs, _result) {}

string UnaryExpression::get_str() {
    return result + " := " + op + " " + rhs ;
}

void UnaryExpression::generateCode(MethodBlock* method) {
    Instruction* load_rhs_inst = new Instruction();
    Instruction* op_inst = new Instruction();
    Instruction* store_inst = new Instruction();

    if (rhs == "true") rhs = "1";
    else if (rhs == "false") rhs = "0";
    try {
        stoi(rhs);
        load_rhs_inst->id = 1; // iconst
    } catch (invalid_argument ex) {
        load_rhs_inst->id = 0; // iload
    }
    load_rhs_inst->argument = rhs;
    
    if (op == "!") op_inst->id = 11;

    store_inst->id = 2;
    store_inst->argument = result;
    method->instructions.push_back(load_rhs_inst);
    method->instructions.push_back(op_inst);
    method->instructions.push_back(store_inst);

}
#pragma endregion

#pragma region Copy
Copy::Copy(string _lhs, string _result) : Tac("Copy", _lhs, "", _result) {}

string Copy::get_str() {
    return result + " := " + lhs ;
}

void Copy::generateCode(MethodBlock* method) {
    Instruction* load_inst = new Instruction();
    Instruction* store_inst = new Instruction();

    if (lhs == "true") lhs = "1";
    else if (lhs == "false") lhs = "0";
    try {
        stoi(lhs);
        load_inst->id = 1; // iconst
    } catch (invalid_argument ex) {
        load_inst->id = 0; // iload
    }
    load_inst->argument = lhs;

    store_inst->id = 2;
    store_inst->argument = result;
    method->instructions.push_back(load_inst);
    method->instructions.push_back(store_inst);
}
#pragma endregion

#pragma region CopyArray
CopyArray::CopyArray(string _lhs, string _rhs, string _result) : Tac("CopyArray", _lhs, _rhs, _result) {}

string CopyArray::get_str() {
    return result + "[" + lhs + "] := " + rhs ;
}

void CopyArray::generateCode(MethodBlock* method) {}
#pragma endregion

#pragma region ArrayAccess
ArrayAccess::ArrayAccess(string _index, string _lhs, string _result) : Tac("ArrayAccess", _lhs, _index, _result) {}

string ArrayAccess::get_str() {
        return result + " := " + lhs + "[" + rhs + "]" ;
    }

void ArrayAccess::generateCode(MethodBlock* method) {}
#pragma endregion

#pragma region MethodCall
MethodCall::MethodCall(string _f, string _N, string _result) : Tac("call", _f, _N, _result) 
     {}

string MethodCall::get_str() {
    return result + " := " + op + " " + lhs + ", " + rhs ;
}

void MethodCall::generateCode(MethodBlock* method) {
    Instruction* call_inst = new Instruction();
    Instruction* ret_inst = new Instruction();
    call_inst->id = 14;
    call_inst->argument = lhs;
    ret_inst->id = 2;
    ret_inst->argument = result;
    method->instructions.push_back(call_inst);
    method->instructions.push_back(ret_inst);
}
#pragma endregion

#pragma region Jump
Jump::Jump(string _label) : Tac("goto", "", "", _label) { }
string Jump::get_str() {
        return op + " " + result ;
}
void Jump::generateCode(MethodBlock* method) {
   Instruction* gotoInst = new Instruction();
   gotoInst->id = 12;
   gotoInst->argument = result;
   method->instructions.push_back(gotoInst); 
}
#pragma endregion

#pragma region CondJump
CondJump::CondJump(string _x, string _label) : Tac("iffalse", _x, "goto", _label) {}

string CondJump::get_str() {
    return op + " " + lhs + " " + rhs + " " + result ;
}

void CondJump::generateCode(MethodBlock* method) {
    Instruction* loadInstr = new Instruction();
    Instruction* jmpInstr = new Instruction();
    if (lhs == "true") lhs = "1";
    else if (lhs == "false") lhs = "0";
    try {
        stoi(lhs);
        loadInstr->id = 1; // iconst
    } catch (invalid_argument ex) {
        loadInstr->id = 0; // iload
    }
    loadInstr->argument = lhs;
    jmpInstr->id = 13;
    jmpInstr->argument = result;
    method->instructions.push_back(loadInstr);
    method->instructions.push_back(jmpInstr);
}
#pragma endregion

#pragma region New
New::New(string _rhs, string _result) : Tac("new", "",_rhs, _result) {}
string New::get_str() {
        return result + " := " + op + " " + rhs ;
    }

void New::generateCode(MethodBlock* method) {}
#pragma endregion

#pragma region NewArray
NewArray::NewArray(string _type, string _N, string _result) : Tac("new", _type, _N, _result) {}
string NewArray::get_str() {
        return result + " " + lhs + " " + rhs ;
    }
void NewArray::generateCode(MethodBlock* method) {}
#pragma endregion

#pragma region Length
Length::Length(string _lhs, string _result) : Tac("length", _lhs, "", _result) {}
string Length::get_str() {
        return result + " := " + op + " " + lhs ;
    }

void Length::generateCode(MethodBlock* method) {}
#pragma endregion

#pragma region Param
Param::Param(string _result) : Tac("param", "", "", _result) {}
string Param::get_str() {
        return op + " " + result;
    }
void Param::generateCode(MethodBlock* method) {
    Instruction* paramInst = new Instruction();
    if (result == "true") result = "1";
    else if (result == "false") result = "0";
    try {
        stoi(result);
        paramInst->id = 1; // iconst
    } catch (invalid_argument ex) {
        paramInst->id = 0; // iload
    }
    paramInst->argument = result;
    method->instructions.push_back(paramInst);
}

#pragma endregion

#pragma region MethodParam
MethodParam::MethodParam(string _result) : Tac("methodparam", "", "", _result) {}
string MethodParam::get_str() {
        return op + " " + result;
    }
void MethodParam::generateCode(MethodBlock* method) {
    Instruction* paramInst = new Instruction();
    paramInst->id = 2;
    paramInst->argument = result;
    method->instructions.push_back(paramInst);
}

#pragma endregion

#pragma region Return
Return::Return(string _result) : Tac("return", "", "", _result) {}
string Return::get_str() {
        return op + " " + result ;
    }

void Return::generateCode(MethodBlock* method) {
    Instruction* valInstr = new Instruction();
    Instruction* retInstr = new Instruction();
    if (result == "true") result = "1";
    else if (result == "false") result = "0";
    try {
        stoi(result);
        valInstr->id = 1; // iconst
    } catch (invalid_argument ex) {
        valInstr->id = 0; // iload
    }
    valInstr->argument = result;
    retInstr->id = 15;
    method->instructions.push_back(valInstr);
    method->instructions.push_back(retInstr);
}
#pragma endregion

#pragma region Print
Print::Print(string _result) : Tac("print", "", "", _result) {}
string Print::get_str() {
        return op + " " + result ;
    }
void Print::generateCode(MethodBlock* method) {

    Instruction* load_res_inst = new Instruction();
    Instruction* op_inst = new Instruction();

    if (result == "true") result = "1";
    else if (result == "false") result = "0";
    try {
        stoi(result);
        load_res_inst->id = 1; // iconst
    } catch (invalid_argument ex) {
        load_res_inst->id = 0; // iload
    }
    load_res_inst->argument = result;
    
    op_inst->id = 16;

    method->instructions.push_back(load_res_inst);
    method->instructions.push_back(op_inst);
}
#pragma endregion

