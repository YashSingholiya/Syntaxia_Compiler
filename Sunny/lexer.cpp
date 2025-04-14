#include "lexer.h"
#include <cctype>
#include <iostream>
#include <unordered_map>

using namespace std;

static unordered_map<string, TokenType> keywords = {
    {"if", TOKEN_IF}, {"else", TOKEN_ELSE}, {"for", TOKEN_FOR},
    {"while", TOKEN_INFINITE}, {"return", TOKEN_RETURN},
    {"true", TOKEN_TRUE}, {"false", TOKEN_FALSE},
    {"print", TOKEN_PRINT}, {"break", TOKEN_BREAK},
    {"continue", TOKEN_CONTINUE}, {"void", TOKEN_VOID},
    {"fun", TOKEN_FUN}
};

vector<Token> scan(const string& input) {
    vector<Token> tokens;
    size_t i = 0;
    int line = 1;

    while (i < input.length()) {
        char ch = input[i];

        if (isspace(ch)) {
            if (ch == '\n') line++;
            i++;
            continue;
        }

        // Identifiers or keywords
        if (isalpha(ch) || ch == '_') {
            string lexeme;
            while (i < input.length() && (isalnum(input[i]) || input[i] == '_')) {
                lexeme += input[i++];
            }
            if (keywords.count(lexeme))
                tokens.push_back({keywords[lexeme], lexeme, line});
            else
                tokens.push_back({TOKEN_IDENTIFIER, lexeme, line});
            continue;
        }

        // Numbers
        if (isdigit(ch)) {
            string number;
            while (i < input.length() && isdigit(input[i])) {
                number += input[i++];
            }
            tokens.push_back({TOKEN_NUMBER, number, line});
            continue;
        }

        // Single-character tokens
        switch (ch) {
            case '+': tokens.push_back({TOKEN_PLUS, "+", line}); break;
            case '-': tokens.push_back({TOKEN_SUB, "-", line}); break;
            case '*': tokens.push_back({TOKEN_STAR, "*", line}); break;
            case '/': tokens.push_back({TOKEN_SLASH, "/", line}); break;
            case '=': tokens.push_back({TOKEN_EQUAL, "=", line}); break;
            case ';': tokens.push_back({TOKEN_SEMICOLON, ";", line}); break;
            case '(': tokens.push_back({TOKEN_LEFT_PAREN, "(", line}); break;
            case ')': tokens.push_back({TOKEN_RIGHT_PAREN, ")", line}); break;
            case '{': tokens.push_back({TOKEN_LEFT_BRACE, "{", line}); break;
            case '}': tokens.push_back({TOKEN_RIGHT_BRACE, "}", line}); break;
            case ',': tokens.push_back({TOKEN_COMMA, ",", line}); break;
            default:
                tokens.push_back({TOKEN_UNKNOWN, string(1, ch), line});
                break;
        }

        i++;
    }

    return tokens;
}
