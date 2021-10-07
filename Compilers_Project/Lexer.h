/*
    Filename    : Lexer.h
    Author      : Lauren Deaver
    Course      : CSCI 435
    Assignment  : Lab 8 - CMinus Parser
*/

/***********************/

#ifndef LEXER_H
#define LEXER_H

/***********************/

#include <string>
#include <vector>

/***********************/

enum TokenType
{
    // Special tokens
    END_OF_FILE, ERROR,

    // Keywords
    IF, ELSE, INT, VOID, RETURN, WHILE,

    // Operators
    PLUS, MINUS, TIMES, DIVIDE, LT, LTE, GT, GTE, EQ, NEQ, ASSIGN,

    // Punctuators
    SEMI, COMMA, LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE,

    // Identifiers and integer literals
    ID, NUM
};

/***********************/

struct Token
{
    Token (TokenType pType = END_OF_FILE,
            std::string pLexeme = "",
            int pNumber = 0, int lineNo = 1, int columnNo = 1)
        : type (pType), lexeme (pLexeme), number (pNumber), line (lineNo), column (columnNo)
    { }

    TokenType   type;
    std::string lexeme;
    int         number;
    int         line;
    int         column;
};

/***********************/

class Lexer
{
public:
    Lexer (FILE* srcFile);

    ~Lexer ();

    Token
    getToken ();

    int
    getLineNum ();

    int
    getColumnNum ();

    Token
    lexId();

    Token
    lexNum();

    std::vector<Token>
    tokenize();

private:
    int
    getChar ();

    void
    ungetChar (int c);

private:
    FILE* m_srcFile;
    int m_lineNum;
    int m_columnNum;
};

/***********************/

#endif