#include <iostream>
#include "exp.h"
using namespace std;
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdentifierExp::~IdentifierExp() { }
AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}
Program::Program() {}
void Program::add(Stm* s) {
    slist.push_back(s);
}
Program::~Program() {
    for (Stm* s : slist) {
        delete s;
    }
}
Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    string s = "";
    switch(op) {
        case PLUS_OP: s += '+'; break;
        case MINUS_OP: s += '-'; break;
        case MUL_OP: s += '*'; break;
        case DIV_OP: s += '/'; break;
        case MENOR: s += '<'; break;
        case MENORIGUAL: s += "<="; break;
        case IGUAL: s += "=="; break;
        default: s += '$';
    }
    return s;
}