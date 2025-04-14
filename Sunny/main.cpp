#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"

using namespace std;

string readFile(const string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FUN: return "FUN";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_VOID: return "VOID";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_SUB: return "SUB";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "OTHER";
    }
}

int main() {
    string source = readFile("input.txt");
    vector<Token> tokens = scan(source);

    for (const auto& token : tokens) {
        cout << "Line " << token.line << ": "
             << token.lexeme << " -> " << tokenTypeToString(token.type) << endl;
    }

    return 0;
}
