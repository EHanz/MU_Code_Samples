/*
    Filename    : Lexer.cc
    Author      : Lauren Deaver/Evan Hanzelman
    Course      : CSCI 435
    Assignment  : Lab 8 - CMinus Parser
*/

/***********************/
// System includes

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

/***********************/
// Local includes

#include "Lexer.h"

/***********************/
// Using declarations

using std::cout;
using std::endl;
using std::string;
using std::stoi; 

/***********************/
Lexer::Lexer (FILE* srcFile)
{
    m_lineNum = 1;
    m_columnNum = 1;
    m_srcFile = srcFile;
    //fopen(m_srcFile, "r");
}

Lexer::~Lexer ()
{
    fclose (m_srcFile);
}

int
Lexer::getChar ()
{
    ++m_columnNum;
    return fgetc (m_srcFile);
}

void
Lexer::ungetChar (int c)
{
    --m_columnNum;
    ungetc (c, m_srcFile);
}

int
Lexer::getLineNum ()
{
    return m_lineNum;
}

int
Lexer::getColumnNum()
{
    return m_columnNum;
}

std::vector <Token>
Lexer::tokenize ()
{
    std::vector<Token> tokenVector;
    while (true)
    {
		tokenVector.push_back(getToken());
        if (tokenVector.back().type == END_OF_FILE)
	    {
            return tokenVector;
	    }
    }
}

Token
Lexer::lexId ()
{
    std::string id;
    char c = getChar ();
    while (isalpha (c))
    {
        id.push_back (c);
        c = getChar ();
    }
    ungetChar (c);
    if (!id.compare ("if"))
    {
        return Token (IF, "if", 0, m_lineNum, m_columnNum);
    }
    else if (!id.compare ("else"))
    {
        return Token (ELSE, "else", 0, m_lineNum, m_columnNum); 
    }
    else if (!id.compare ("int"))
    {
        return Token (INT, "int", 0, m_lineNum, m_columnNum); 
    }
    else if (!id.compare ("void"))
    {
        return Token (VOID, "void", 0, m_lineNum, m_columnNum); 
    }
    else if (!id.compare ("return"))
    {
        return Token (RETURN, "return", 0, m_lineNum, m_columnNum); 
    }
    else if (!id.compare("while"))
    {
        return Token (WHILE, "while", 0, m_lineNum, m_columnNum); 
    }
    else
    {
        return Token (ID, id, 0, m_lineNum, m_columnNum);
    }
}

Token
Lexer::lexNum ()
{
    std::string strNum;
    char c = getChar ();
    while (isdigit (c))
    {
        strNum.push_back (c);
        c = getChar ();
    }
    ungetChar (c);
    int intNum = stoi (strNum);
    return Token (NUM, strNum, intNum, m_lineNum, m_columnNum);
    //similar to lexId but change the string to int
}

Token
Lexer::getToken ()
{
    while (true)
    {
        char c = getChar ();
        if (isalpha (c))
        {
            ungetChar (c);
            return lexId ();
        }
        if (isdigit (c))
        {
            ungetChar (c);
            return lexNum ();
        }
        switch (c)
        {
            case '\n':
                ++m_lineNum;
                m_columnNum = 1;
            case ' ':
            case '\t':
                // try to consume again
                break; 

            case EOF:
                return Token (END_OF_FILE);

            //Operators
            case '+':
                return Token (PLUS, "+", 0, m_lineNum, m_columnNum);
            /*if (c != '+')
            {
                ungetChar(c);
                return Token (PLUS, "+");
            }
            return Token (INCREMENT, "++");*/
            case '-':
                return Token (MINUS, "-", 0, m_lineNum, m_columnNum);

            case '*':
                return Token (TIMES, "*", 0, m_lineNum, m_columnNum);

            case '/':
                c = getChar();
                if (c == '*')
                {
                    while (true)
                    {
                        c = getChar ();
                        if (c == '\n')
                        {
                            ++m_lineNum;
                            m_columnNum = 0; // why 0?
                        }
                        else if (c == '*')
                        {
                            c = getChar ();
                            if (c == '/')
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    ungetChar (c);
                    return Token (DIVIDE, "/", 0, m_lineNum, m_columnNum);
                }
                break;
            case '<':
                c = getChar ();
                if (c != '=')
                {
                    ungetChar (c);
                    return Token (LT, "<", 0, m_lineNum, m_columnNum);
                }
                return Token (LTE, "<=", 0, m_lineNum, m_columnNum);

            case '>':
                c = getChar ();
                if (c != '=')
                {
                    ungetChar (c);
                    return Token (GT, ">", 0, m_lineNum, m_columnNum);
                }
                return Token (GTE, ">=", 0, m_lineNum, m_columnNum);

            case '=':
                c = getChar ();
                if (c != '=')
                {
                    ungetChar (c);
                    return Token (ASSIGN, "=", 0, m_lineNum, m_columnNum);
                }
                return Token (EQ, "==", 0, m_lineNum, m_columnNum);
            
            case '!':
                c = getChar ();
                if (c != '=')
                {
                    ungetChar (c);
                    return Token (ERROR, "!", 0, m_lineNum, m_columnNum);
                }
                return Token (NEQ, "!=", 0, m_lineNum, m_columnNum);

            //Puncuators
            case ';':
                return Token (SEMI, ";", 0, m_lineNum, m_columnNum);
            
            case ',':
                return Token (COMMA, ",", 0, m_lineNum, m_columnNum);

            case '(':
                return Token (LPAREN, "(", 0, m_lineNum, m_columnNum);

            case ')':
                return Token (RPAREN, ")", 0, m_lineNum, m_columnNum);

            case '[':
                return Token (LBRACK, "[", 0, m_lineNum, m_columnNum);
            
            case ']':
                return Token (RBRACK, "]", 0, m_lineNum, m_columnNum);

            case '{':
                return Token (LBRACE, "{", 0, m_lineNum, m_columnNum);

            case '}':
                return Token (RBRACE, "}", 0, m_lineNum, m_columnNum);

            default:
                std::string err;
                err.push_back (c);
                return Token (ERROR, err, 0, m_lineNum, m_columnNum);
        } // switch
    } // while
}


