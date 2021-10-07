/*
    Filename    : Parser.h
    Author      : Lauren Deaver/Evan Hanzelman
    Course      : CSCI 435
    Assignment  : Lab 8 - CMinus Parser
*/
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include "Lexer.h"

class Parser
{
    public :
        Parser (std::vector<Token> tokenVector);

        ~Parser ();

        void
        match (const std::string& function, TokenType expectedType);

        void
        error (const std::string& function, TokenType expectedType);

        void
        start();

        void
        program();

        void
        declarationList();

        void
        declaration();

        void
        varDeclaration();

        void 
        typeSpecifier();

        void
        funDeclaration();

        void
        params();

        void
        paramList();

        void
        param();

        void
        compoundStmt();

        void
        localDeclarations();

        void
        stmtList();

        void
        stmt();

        void
        expressionStmt();

        void
        selectionStmt();

        void
        iterationStmt();

        void
        returnStmt();

        void
        expr();

        void
        var();

        void
        simpleExpr();

        void
        relop();

        void
        additiveExpr();

        void
        addop();

        void
        term();

        void
        mulop();

        void
        factor();

        void
        call();

        void
        args();

        void
        argList();

    public:
        std::vector<Token> m_tokens;
        int m_index;
};

#endif