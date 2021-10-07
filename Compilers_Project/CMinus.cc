/*
    Filename    : CMinus.cc
    Author      : Lauren Deaver/Evan Hanzelman
    Course      : CSCI 435
    Assignment  : Assignment 8 - CMinus Parser
*/


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Parser.h"

using std::cout;
using std::endl;

//extern FILE* yyin;
//extern char* yytext;
//extern int columnNum;
//extern int lineNum;

//extern "C"
//int
//yylex ();

int
main (int argc, char* argv[])
{
    ++argv;
    --argc;
    FILE* srcFile;
    if (argc > 0)
    {
        srcFile = fopen(argv[0], "r");
    }
    else 
    {
        srcFile = stdin;
    }
    Lexer lex(srcFile);

    //printf("TOKEN\t\tLEXEME\t\tVALUE\n");
    //printf("=====\t\t======\t\t=====\n");

    //std::string tokens[29] = {"EOF", "ERROR", "IF", "ELSE", "INT", "VOID", "RETURN", "WHILE", "PLUS",
        //"MINUS", "TIMES", "DIVIDE", "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "ASSIGN", "SEMI",
       // "COMMA", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "ID", "NUM"};

    std::vector<Token> tokenVector;
    tokenVector = lex.tokenize();
    
    //Parser pars(lex.tokenize());
    Parser pars(tokenVector);
    pars.start();
    /*
    Token result;
    int token;
    std::string lexeme;
    int num;

    do
    {
        result = lex.getToken();
        token = result.type;
        lexeme = result.lexeme;
        num = result.number;

        //std::string strText = std::string(yytext);
        
        std::cout << tokens[token] << "\t\t\"" << lexeme << "\"\t\t";
        if (token == ID)
        {
            std::cout << "\"" << lexeme << "\"\n";
        }
        else if (token == NUM)
        {
            //int intText = stoi(strText);
            std::cout << num << "\n";
        } 
        else if (token == ERROR)
        {
            std::cout << "Line: " << lex.getLineNum() << "; Column: " << lex.getColumnNum() << "\n";
        }
        else
        {
            std::cout << "\n";
        }
    } while (token != 0);
*/
    return EXIT_SUCCESS;
}
