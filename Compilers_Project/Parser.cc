/*
    Filename    : Parser.cc
    Author      : Lauren Deaver/Evan Hanzelman
    Course      : CSCI 435
    Assignment  : Lab 8 - CMinus Parser
*/


#include "Parser.h"
#include "Lexer.h"

Parser::Parser (std::vector<Token> tokenVector)
{
    m_tokens = tokenVector;
    m_index = 0;
}

Parser::~Parser ()
{

}

void
Parser::match (const std::string& function, TokenType expectedType)
{
    if (m_tokens[m_index].type == expectedType)
    {
        m_index++;
    }
    else
    {
        error (function, expectedType);
    }

}

void
Parser::error (const std::string& function, TokenType expectedType)
{
    printf ("\n Error while parsing: \'%s\'\n", function.c_str ());
    printf ("\tEncountered: %s (line %d, column %d)\n", m_tokens[m_index].lexeme.c_str(), m_tokens[m_index].line, m_tokens[m_index].column);
    printf ("\t Expected :%u\n", expectedType);
    exit (1);
}

void
Parser::start()
{
    program();
    if (m_tokens[m_index].type != END_OF_FILE)
    {
        error("program", END_OF_FILE);
    }
    else
    {
        printf("Valid!\n");
    }
}
//program -> declarationList
void
Parser::program ()
{
    if (m_tokens[m_index].type == END_OF_FILE)
    {
        error("program", INT);
    }
    declarationList ();
}

//declarationList -> declaration {declaration}
void
Parser::declarationList ()
{
    declaration ();
    while (m_tokens[m_index].type != END_OF_FILE)
    {
        declaration ();
    }

}

//declaration -> varDeclaration | funDeclaration
void
Parser::declaration ()
{
    if (m_tokens[m_index + 2].type == LPAREN)
    {
        funDeclaration ();
    }
    else if (m_tokens[m_index + 2].type == LBRACK)
    {
        varDeclaration ();
    }
    else
    {
        //error ();
    }

}

//varDeclaration -> typeSpecifier 'ID' [ '[' 'NUM' ']' ] ';'
void
Parser::varDeclaration ()
{
    typeSpecifier ();
    match ("varDeclaration", ID);

    if (m_tokens[m_index].type == LBRACK)
    {
        match ("varDeclaration", LBRACK);
        match ("varDeclaration", NUM);
        match ("varDeclaration", RBRACK);
    }
    match ("varDeclaration", SEMI);
}

//typeSpecifier -> 'INT' | 'VOID'
void 
Parser::typeSpecifier ()
{
    if (m_tokens[m_index].type == INT)
    {
        match ("typeSpecifier", INT);
    }
    else if (m_tokens[m_index].type == VOID)
    {
        match ("typeSpecifier", VOID);
    }
    else
    {
        //error
    }

}

//funDeclaration -> typeSpecifier ID '(' params ')' compountStmt
void
Parser::funDeclaration ()
{
    typeSpecifier ();
    match ("funDeclaration", ID);
    match ("funDeclaration", LPAREN);
    params ();
    match ("funDeclaration", RPAREN);
    compoundStmt ();

}

//params -> paramList | 'VOID'
void
Parser::params ()
{
    if ((m_tokens[m_index].type == INT) && (m_tokens[m_index + 1].type == ID))
    {
        paramList ();
    }
    else if (m_tokens[m_index].type == VOID)
    {
        match ("params", VOID);
    }
    else
    {
        //error
    }
}

//paramList -> param { ',' param}
void
Parser::paramList ()
{
    param ();
    while (m_tokens[m_index].type == COMMA)
    {
        match ("paramList", COMMA);
        param ();
    }
}

//param -> typeSpecifier ID ['[' ']']
void
Parser::param()
{
    typeSpecifier();
    match("param", ID);
    if(m_tokens[m_index].type == LBRACK)
    {
        match("param", LBRACK);
        match("param", RBRACK);
    }

}

//compoundList -> '{' localDeclarations stmtList '}'
void
Parser::compoundStmt ()
{
    match("compoundStmt", LBRACE);
    localDeclarations();
    stmtList();
    match("compountStmt", RBRACE);

}

//localDeclarations -> {varDeclaration}
void
Parser::localDeclarations ()
{
    while((m_tokens[m_index].type == INT) || (m_tokens[m_index].type == VOID))
    {
        varDeclaration ();
    }
}

// stmtList -> {stmt}
void
Parser::stmtList ()
{
    while((m_tokens[m_index].type == ID) || (m_tokens[m_index].type == SEMI) ||
            (m_tokens[m_index].type == LBRACE) || (m_tokens[m_index].type == IF) ||
            (m_tokens[m_index].type == WHILE) || (m_tokens[m_index].type == RETURN))
    {
        stmt ();
    }
}

//stmt -> expressionStmt | compoundStmt | selectionStmt | iterationStmt | returnStmt
void
Parser::stmt ()
{
    if ((m_tokens[m_index].type == ID) || (m_tokens[m_index].type == SEMI))
    {
        expressionStmt ();
    }
    else if (m_tokens[m_index].type == LBRACE)
    {
        compoundStmt ();
    }
    else if (m_tokens[m_index].type == IF)
    {
        selectionStmt ();
    }
    else if (m_tokens[m_index].type == WHILE)
    {
        iterationStmt ();
    }
    else if (m_tokens[m_index].type == RETURN)
    {
        returnStmt ();
    }
    else
    {
        //error
    }

}

// expresionStmt -> [expr] ';'
void
Parser::expressionStmt ()
{
    if ((m_tokens[m_index].type == ID) || (m_tokens[m_index].type == LPAREN) || (m_tokens[m_index].type == NUM))
    {
        expr ();
    }
    match ("expressionStmt", SEMI);

}

//selectionStmt -> 'IF' '(' expr ')' stmt [ 'ELSE' stmt ]
void
Parser::selectionStmt ()
{
    match ("selectionStmt", IF);
    match ("selectionStmt", LPAREN);
    expr ();
    match ("selectionStmt", RPAREN);
    stmt ();
    if (m_tokens[m_index].type == ELSE)
    {
        match ("selectionStmt", ELSE);
        stmt ();
    }

}

//iterationStmt -> 'WHILE' '(' expr ')' stmt
void
Parser::iterationStmt ()
{
    match ("iterationStmt", WHILE);
    match ("iterationStmt", LPAREN);
    expr ();
    match ("iterationStmt", RPAREN);
    stmt ();

}

//returnStmt -> 'RETURN' [expr] ';'
void
Parser::returnStmt ()
{
    match ("returnStmt", RETURN);
    if ((m_tokens[m_index].type == ID) || (m_tokens[m_index].type == LPAREN) | (m_tokens[m_index].type == NUM))
    {
        expr ();
    }
    match ("returnStmt", SEMI);

}

//expr -> {var '='} simpleExpr
//expr -> var '=' expr | simpleExpr
void
Parser::expr ()
{
    while (m_tokens[m_index].type == ID) {
        int saved = m_index;
        var();
        // lookahead said there isn't an assign -- must be simpleExpr
        if (m_tokens[m_index].type != ASSIGN) {
            m_index = saved;
            break;
        }
        match("expr", ASSIGN);
    }
    // doesn't start with ID -- must be a simpleExpr
    simpleExpr();
}

//var -> ID [ '[' expr ']' ]
void
Parser::var ()
{
    match ("var", ID);
    if (m_tokens[m_index].type == LBRACK)
    {
        match ("var", LBRACK);
        expr ();
        match ("var", RBRACK);
    }

}

//simpleExpr -> additiveExpr {relop additiveExpr}
void
Parser::simpleExpr ()
{
    additiveExpr ();
    while ((m_tokens[m_index].type == LT) || (m_tokens[m_index].type == LTE) ||
        (m_tokens[m_index].type == GT) || (m_tokens[m_index].type == GTE) ||
        (m_tokens[m_index].type == EQ) || (m_tokens[m_index].type == NEQ))
    {
        relop ();
        additiveExpr ();
    }

}

//relop -> '<=' | '<' | '>' | '>=' | '==' |'!='
void
Parser::relop ()
{
    TokenType t = m_tokens[m_index].type;
    switch (t) {
        case LT:
        case LTE:
        case GT:
        case GTE:
        case EQ:
        case NEQ:
            match ("additiveExpr", t);
        default:
            ;
    }
}

//additiveExpr -> term {addop term}
void
Parser::additiveExpr ()
{
    term ();
    while ((m_tokens[m_index].type == PLUS) || (m_tokens[m_index].type == MINUS))
    {
        addop ();
        term ();
    }

}

//addop -> '+' | '-'
void
Parser::addop ()
{
    if (m_tokens[m_index].type == PLUS)
    {
        match ("addop", PLUS);
    }
    else if (m_tokens[m_index].type == MINUS)
    {
        match ("addop", MINUS);
    }
    else 
    {
        //error
    }

}

//term -> factor {mulop factor}
void
Parser::term ()
{
    factor ();
    while ((m_tokens[m_index].type == TIMES) || (m_tokens[m_index].type == DIVIDE))
    {
        mulop ();
        factor ();
    }

}

//mulop -> '*' | '/'
void
Parser::mulop ()
{
    if (m_tokens[m_index].type == TIMES)
    {
        match ("mulop", TIMES);
    }
    else if (m_tokens[m_index].type == DIVIDE)
    {
        match ("mulop", DIVIDE);
    }
    else
    {
        //error
    }
}

//factor -> '(' expr ')' | var | call | NUM
void
Parser::factor ()
{
    if (m_tokens[m_index].type == LPAREN)
    {
        match ("factor", LPAREN);
        expr ();
        match ("factor", RPAREN);
    }
    else if ((m_tokens[m_index].type == ID) && (m_tokens[m_index + 1].type == LPAREN))
    {
        call ();
    }
    else if (m_tokens[m_index].type == ID)
    {
        var ();
    }
    else if (m_tokens[m_index].type == NUM)
    {
        match ("factor", NUM);
    }
}

//call -> ID '(' args ')'
void
Parser::call ()
{
    match ("call", ID);
    match ("call", LPAREN);
    args ();
    match ("call", RPAREN);
}

//args -> [argList]
void
Parser::args ()
{
    if ((m_tokens[m_index].type == ID) || (m_tokens[m_index].type == LPAREN) | (m_tokens[m_index].type == NUM))
    {
        argList ();
    }

}

//argList -> expr { ',' expr }
void
Parser::argList ()
{
    expr ();
    while (m_tokens[m_index].type == COMMA)
    {
        match ("argList", COMMA);
        expr ();
    }

}
