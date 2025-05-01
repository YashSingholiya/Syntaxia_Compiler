#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__
#include "parser.h"
#include <stack>
extern SymbolTable st;
extern string tokenNames[];
// used to print token names in token.txt file in lexer
extern stack<string> scopeCount;
// whenever we enter a scope, we push the scope name,
// and on encountering } we pop it out


bool expect(TokenType typeA, TokenType typeB, TokenType typeC, TokenType typeD, int count);
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, int count);
bool expect(TokenType typeA, TokenType typeB, int count);
bool expect(TokenType typeA, int count);
void error(string error, int lineNo);

int c_tokenfun(int counter);
int c_tokenfuncall(int counter);
int c_varDeclaration(int counter);
int c_cout(int counter);
int c_rightBrace(int counter);
int c_break(int counter);
int c_swbreak(int counter);
int c_default(int counter);
int c_case(int counter);
int c_switch(int counter);
int expression(int counter, TokenType express);
int c_continue(int counter);
int c_return(int counter);
int c_forloop(int counter);
int c_while(int counter);
int c_label(int counter);
int c_if(int counter);
int c_else(int counter);
int c_tenary(int counter);
int c_goto(int counter);
void parse(int counter);
#endif


