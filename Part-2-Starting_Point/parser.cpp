//**************************************************************************
 
// Replace with appropriate header comment......

//**************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
 
using namespace std;

int nextToken = 0;            // hold nextToken returned by lex

// Which tree level are we currently in?  
static int level = 0;

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
set<string> symbolTable; // Symbol Table

//*****************************************************************************
// Indent to reveal tree structure
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
// Report what we found
void output(string what) {
  cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.
bool first_of_program(void);

void lex() {
    nextToken = yylex();
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
void program() 
{
    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    
    if (nextToken != TOK_PROGRAM) throw("error");
    output("PROGRAM");
    
    cout << psp() << "enter <program>" << endl;
    ++level;

    lex();
    output("IDENTIFIER");
    lex();
    output("SEMICOLON");
    lex();
    output("BLOCK");
    block();

    --level;
    cout << psp() << "exit <program>" << endl;
    
}
bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <block> → stuff
void block() {
    
    cout << psp() << "enter <block>" << endl;
    ++level;
    
    if (nextToken != TOK_VAR && nextToken != TOK_BEGIN) throw("error");

    bool checker = true;
    if (nextToken == TOK_BEGIN) checker = false;
    
    output("BEGINBLOCK");
    
    while(checker) {
        output("IDENTIFIER");
        lex();
        output("COLON");
        lex();
        output("TYPE");
        lex();
        output("SEMICOLON");
        lex();
        if (nextToken == TOK_BEGIN) checker = false;
    }
    
    checker = true;
    statement();
    while(checker){
        output("SEMICOLON");
        if (nextToken == TOK_END) checker = false;
        statement();
    }
    output("END");
    
    --level;
    cout << psp() << "exit <block>" << endl;
    
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <statement> → stuff
void statement() {
    bool skipper = false;
    bool looper = true;
    switch(nextToken) {
        case TOK_BEGIN:
            output("BEGIN");
            cout << psp() << "enter <compound_statement>" << endl;
            ++level;

            while(looper){
                statement();
                if (nextToken != TOK_SEMICOLON) looper = false;
                output("SEMICOLON");
            }
            
            if (nextToken != TOK_END) throw("error");
            output("END");
            --level;
            cout << psp() << "exit <compound_statement>" << endl;
            break;
            
        case TOK_IF:
            output("IF");
            cout << psp() << "enter <if>" << endl;
            ++level;
            
            expression();
            if (nextToken != TOK_THEN) throw("error");
            statement();
            if (nextToken == TOK_ELSE) skipper = true;
            if (skipper) statement();
            --level;
            cout << psp() << "exit <if>" << endl;
            break;
            
        case TOK_WHILE:
            output("WHILE");
            cout << psp() << "enter <while>" << endl;
            ++level;
            
            expression();
            statement();
            --level;
            cout << psp() << "exit <while>" << endl;
            break;
            
        case TOK_READ:
            output("READ");
            cout << psp() << "enter <read>" << endl;
            ++level;
            
            if (nextToken != TOK_OPENPAREN) throw("error");
            output("OPENPAREN");
            //identifier();
            if (nextToken != TOK_CLOSEPAREN) throw("error");
            output("CLOSEPAREN");
            --level;
            cout << psp() << "exit <read>" << endl;
            break;
            
        case TOK_WRITE:
            output("WRITE");
            cout << psp() << "enter <write>" << endl;
            ++level;
            if (nextToken != TOK_OPENPAREN) throw("error");
            output("OPENPAREN");
            output("WRITE");
            if (nextToken != TOK_CLOSEPAREN) throw("error");
            output("CLOSEPAREN");
            --level;
            cout << psp() << "exit <write>" << endl;
            break;
            
        default:
            //identifier();
            if(nextToken != TOK_ASSIGN) throw("error");
            expression();
            break;
    }
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <expression> → stuff
void expression() {
    cout << psp() << "enter <expression>" << endl;
    ++level;
    
    bool checker = false;
    switch(nextToken) {
        case TOK_EQUALTO:
            output("EQUALTO");
            checker = true;
            break;
        case TOK_LESSTHAN:
            output("LESSTHAN");
            checker = true;
            break;
        case TOK_GREATERTHAN:
            output("GREATERTHAN");
            checker = true;
            break;
        case TOK_NOTEQUALTO:
            output("NOTEQUALTO");
            checker = true;
            break;
    }
    
    if(checker) simple_expression();
    
    
    --level;
    cout << psp() << "exit <expression>" << endl;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <simple_expression> → stuff
void simple_expression() {
    cout << psp() << "enter <simple_expression>" << endl;
    ++level;
    
    bool outerloop = true;
    
    while(outerloop) {
        bool checker = false;
        switch(nextToken) {
            case TOK_PLUS:
                output("PLUS");
                checker = true;
                break;
            case TOK_MINUS:
                output("MINUS");
                checker = true;
                break;
            case TOK_OR:
                output("OR");
                checker = true;
                break;
        }
        
        if(checker) term();
        
        if (nextToken != TOK_PLUS || nextToken != TOK_MINUS || nextToken != TOK_OR) outerloop = false;
    }
    
    --level;
    cout << psp() << "exit <simple_expression>" << endl;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <term> → stuff
void term() {
    cout << psp() << "enter <term>" << endl;
    ++level;
    
    bool outerloop = true;
    
    while(outerloop) {
        bool checker = false;
        switch(nextToken) {
            case TOK_MULTIPLY:
                output("MULTIPLY");
                checker = true;
                break;
            case TOK_DIVIDE:
                output("DIVIDE");
                checker = true;
                break;
            case TOK_AND:
                output("AND");
                checker = true;
                break;
        }
        
        if(checker) term();
        
        if (nextToken != TOK_MULTIPLY || nextToken != TOK_DIVIDE || nextToken != TOK_AND) outerloop = false;
    }
    
    --level;
    cout << psp() << "exit <term>" << endl;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <factor> → stuff
void factor() {
    cout << psp() << "enter <factor>" << endl;
    ++level;
    
    --level;
    cout << psp() << "exit <factor>" << endl;
}
