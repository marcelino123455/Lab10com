#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program* Parser::parseProgram() {
    Program* p = new Program();
    try {
        while (!isAtEnd()) {
            p->add(parseStatement());
            if (!isAtEnd() && !match(Token::PC)) {
                throw runtime_error("Error: se esperaba ';' al final de la declaración.");
            }
        }
    } catch (const exception& e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        delete p;
        exit(1);
    }
    return p;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e;

    Exp* expif;
    list<Stm*> slist1;
    list<Stm*> slist2;
    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }
    
    if (match(Token::ID)) {
        string lex = previous->text;
        
        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExp();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }else if (match(Token::IF)) {
        expif = parseCExp();
        if (!match(Token::THEN)) {
            cout << "Error: Se esperaba un THEN" << endl;
            exit(1);
        }
        cout<<"Estoy en el if"<<endl; //Del
        cout<<"El token es: "<<current->text<<endl; //Del
        while (!check(Token::ELSE) and !check(Token::ENDIF) and !isAtEnd()) {
            if (!check(Token::PC)) {
                slist1.push_back(parseStatement());
            } else {
                match(Token::PC);
            }
        }
        if (match(Token::ELSE)) {
            while (!check(Token::ENDIF) and !isAtEnd()) {
                if (!check(Token::PC)) {
                    slist2.push_back(parseStatement());
                } else {
                    match(Token::PC);
                }
            }
        }

        if(!match(Token::ENDIF)) {
            cout<<"Uy error como podria terminar un if"<<endl;
            exit(9);
        }
        s = new IFStatement(expif, slist1, slist2);
    }

    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}
Exp* Parser::parseCExp() {
    Exp* left = parseExpression();
    BinaryOp op;
    if (match(Token::MENOR)) {
        Exp* right = parseExpression();
        op = MENOR;
        left = new BinaryExp(left, right, op);
    } else if(match(Token::MENORIGUAL)) {
        Exp* right = parseExpression();
        op = MENORIGUAL;
        left = new BinaryExp(left, right, op);
    }else if(match(Token::IGUAL)) {
        Exp* right = parseExpression();
        op = IGUAL;
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    cout<<"El token en factor  es: "<<current->text<<endl; //Del

    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        return new IdentifierExp(previous->text);
    }
    else if (match(Token::PI)){
        e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }

    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}

