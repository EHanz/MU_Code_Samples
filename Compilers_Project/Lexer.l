/*
    Filename: Lexer.l
    Author: Evan Hanzelman
    Course: CSCI 435
    Assignment: Assignment 4
*/

%{
    #include "TokenType.h"

    extern "C"
    int
    yylex ();

    int cCount = 0;
    int lCount = 1;
%}

NUM     [0-9]
ID      [a-z][a-z]*
%x      C_COMMENT

%%

int     {
            cCount += 3;
            return INT;
        }

if      {
            cCount += 2;
            return IF;
        }

else    {
            cCount += 4;
            return ELSE;
        }

void    {
            cCount += 4;
            return VOID;
        }

return  {
            cCount += 6;
            return RETURN;
        }

while   {
            cCount += 5;
            return WHILE;
        }

"+"     {
            ++cCount;
            return PLUS;
        }

"-"     {
            ++cCount;
            return MINUS;
        }

"*"     {
            ++cCount;
            return TIMES;
        }

"/"     {
            ++cCount;
            return DIVIDE;
        }

"<"     {
            ++cCount;
            return LT;
        }

"<="    {
            cCount += 2;
            return LTE;
        }

">"     {
            ++cCount;
            return GT; 
        }

">="    {
            cCount += 2;
            return GTE;
        }

"=="    {
            cCount += 2;
            return EQ;
        }

"!="    {
            cCount += 2;
            return NEQ;
        }

"="     {
            ++cCount;
            return ASSIGN;
        }

";"     {
            ++cCount;
            return SEMI;
        }

","     {
            ++cCount;
            return COMMA;
        }

"("     {
            ++cCount;
            return LPAREN;
        }

")"     {
            ++cCount;
            return RPAREN;
        }

"["     {
            ++cCount;
            return LBRACK;
        }

"]"     {
            ++cCount;
            return RBRACK;
        }

"{"     {
            ++cCount;
            return LBRACE;
        }

"}"     {
            ++cCount;
            return RBRACE;
        }

{NUM}+  {
            cCount += yyleng;
            return NUM;
        }

{ID}    {
            cCount += yyleng;
            return ID;
        }

\n      {
            ++lCount;
        }

[ \t]*   {
            cCount += yyleng;
        }

.       {
            ++cCount;
            return ERROR;
        }

"/*"                { BEGIN (C_COMMENT); }
<C_COMMENT>"*/"     { BEGIN (BEGIN(INITIAL)); }
<C_COMMENT>\n       { }
<C_COMMENT>.        { }

%%