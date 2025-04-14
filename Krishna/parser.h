#ifndef __PARSER_H__
#define __PARSER_H__
using namespace std;
#include <string>
#include "string.h"
const int MAX = 100000;
// This is the maximum number of tokens

// token:smallest definable unit in programming language syntax
// '\0' is TOKEN_EOF - located at end of every file - marks end of scanning process

// We are ordering all tokens in order of standard c++ precedence. This will make later processes easier
typedef enum{
TOKEN_EQUAL, TOKEN_LOGICAL_OR, TOKEN_LOGICAL_AND, TOKEN_EQUAL_EQUAL,
TOKEN_GREATER, TOKEN_GREATER_EQUAK, TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_BANG_EQUAL,

TOKEN_COLON, TOKEN_TERNARY,

TOKEN_ADD_ASS, TOKEN_SUB_ASS, TOKEN_SLASH_ASS, TOKEN_STAR_ASS, 
TOKEN_ASS_OR, TOKEN_ASS_XOR, TOKEN_ASS_AND,

TOKEN_BIT_OR, TOKEN_BIT_XOR, TOKEN_BIT_AND, 
TOKEN_SHIFT_LEFT, TOKEN_SHIFT_RIGHT, TOKEN_INVERSE, TOKEN_PLUS,
TOKEN_SUB, TOKEN_SLASH, TOKEN_MODULUS, TOKEN_STAR,

TOKEN_BANG, TOKEN_MINUS_MINUS, TOKEN_PLUS_PLUS,
// they have used token_sub_self for token_minus_minus but i wont

// SINGLE CHARACTER TOKENS
TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, 
TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,

TOKEN_START, TOKEN_END,

// ONE/TWO CHARACTER TOKENS
// Literals.
TOKEN_BOOL, TOKEN_VAR, TOKEN_STRINGVAR, TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER, 
TOKEN_PIN,TOKEN_FALSE, TOKEN_TRUE, TOKEN_FUNCALL, TOKEN_OUTPUT, TOKEN_ENDL,

TOKEN_INFINITE, TOKEN_FUN,TOKEN_VOID,  TOKEN_LEFT_BLOCK, TOKEN_RIGHT_BLOCK,

TOKEN_BREAK,TOKEN_CONTINUE, TOKEN_SWITCH,TOKEN_CASE, TOKEN_DEFAULT, TOKEN_ELSE,
TOKEN_FOR, TOKEN_IF, TOKEN_NIL, TOKEN_PRINT, TOKEN_RETURN,
TOKEN_WHILE, TOKEN_HYPHEN, TOKEN_ERROR, TOKEN_COUT,TOKEN_GOTO,
TOKEN_EOF
}TokenType;


// This is the enumerated tokens with all their names written down for convenience
string tokenNames[] = {
    "TOKEN_EQUAL",  "TOKEN_LOGICAL_OR", "TOKEN_LOGICAL_AND", "TOKEN_EQUAL_EQUAL", 
    "TOKEN_GREATER","TOKEN_GREATER_EQUAL",  "TOKEN_LESS", 
    "TOKEN_LESS_EQUAL", "TOKEN_BANG_EQUAL", 
    
     "TOKEN_COLON", "TOKEN_TENARY",
    
     "TOKEN_ADD_ASS", "TOKEN_SUB_ASS", "TOKEN_SLASH_ASS", 
    "TOKEN_STAR_ASS", "TOKEN_ASS_OR", "TOKEN_ASS_XOR", "TOKEN_ASS_AND",
    
    "TOKEN_BIT_OR",  "TOKEN_BIT_XOR", "TOKEN_BIT_AND", 
    "TOKEN_SHIFT_LEFT", "TOKEN_SHIFT_RIGHT", "TOKEN_INVERSE",
    "TOKEN_PLUS", "TOKEN_SUB",   "TOKEN_SLASH", "TOKEN_MODULUS",  "TOKEN_STAR", 
    "TOKEN_BANG", "TOKEN_SUB_SELF", "TOKEN_ADD_SELF",
    
     // Single-character "TOKENs.
     "TOKEN_LEFT_PAREN", "TOKEN_RIGHT_PAREN",
     "TOKEN_LEFT_BRACE", "TOKEN_RIGHT_BRACE",
    
     "TOKEN_COMMA", "TOKEN_DOT", "TOKEN_SEMICOLON",
     "TOKEN_START", "TOKEN_END",
     // One or two character "TOKENs.
     // Literals.
      "TOKEN_BOOL", "TOKEN_VAR", "TOKEN_STRINGVAR", 
      "TOKEN_IDENTIFIER", "TOKEN_STRING", "TOKEN_NUMBER",
      "TOKEN_PIN","TOKEN_FALSE", "TOKEN_TRUE", "TOKEN_FUNCALL",
      "TOKEN_OUTPUT","TOKEN_ENDL",
     // Keywords.
    "TOKEN_INFINITE",   "TOKEN_FUN", 
     "TOKEN_VOID", "TOKEN_LEFT_BLOCK", "TOKEN_RIGHT_BLOCK",
    
    "TOKEN_BREAK", "TOKEN_CONTINUE",  "TOKEN_SWITCH", 
     "TOKEN_CASE", "TOKEN_DEFAULT", "TOKEN_ELSE",
     "TOKEN_FOR", "TOKEN_IF", "TOKEN_NIL", 
     "TOKEN_PRINT", "TOKEN_RETURN", "TOKEN_WHILE", "TOKEN_HYPHEN",
     "TOKEN_ERROR", "TOKEN_COUT","TOKEN_GOTO", "TOKEN_EOF"
    };


class funcList{
// stores all lists of functions in our compiler
    public:   
    string Fname;
    // name of function
    int An, Vn, Vs,  As;
    // An- number of arguements it has
    // Vn- number of local variables the function has
    // Vs-index of the local variable in class lVList where the local var. STARTS
    // As- index of the arguement in class ArguementList where the Arguement STARTS
    int call = 0;
    funcList(int An, string Fname, int Vn, int Vs, int As, int call)
    {
        this->An   = An;
        this->Fname        = Fname; 
        this->Vn   = Vn;  
        this->Vs   = Vs;               
        this->As   = As; 
        this->call = call;                 
    }
    friend class SymbolTable; 
};

// basically, we need to know the index where we started registering the args of a list
// and these args, so we can directly access that arg instead of searching the whole list

class lVList{
// stores the list of local variables 
    public:  
        string  Vname;
        TokenType vType;
        int vNo;
        // this serial number is used to assign registers during code generation
        lVList(TokenType vType, string Vname, int vNo)
        {
            this->vType  = vType;
            this->Vname  = Vname;
            this->vNo    = vNo;
        }
        friend class SymbolTable; 
};

class gVList{
// stores all the global variables
    public:  
        string  Gname;
        TokenType vType;
        gVList(TokenType vType, string Gname)
        {
            this->vType  = vType;
            this->Gname  = Gname;
        }
        friend class SymbolTable;

};

class ArguementList{
// stores all arguements of functions
    TokenType argType;
    string  argName;
    int  argNo;

    ArgumentList(TokenType argType, string argName, int argNo)
    {
        this->argType        = argType;
        this->argName        = argName;
        this->argNo          = argNo;
    }
    friend class SymbolTable; 
};



// tokenClass basically is going to store all the tokens we will parse
class tokenClass{
    public:
        TokenType type;
        TokenType Vtype;
        // this stores identifier type and makes our life easier
        string regName;
        // stores the name of register in which you want to store your variable data

        string identifier;
        // stores data of token for ex. int jump() so stores jump
        // IFF there is data to be stored by token
        int lineNum;

        tokenClass(TokenType type, int line)
        {
            this->lineNum = line;
            this->type = type;
        }

        tokenClass(TokenType type, int line, string identifier)
        {
            this->type = type;
            this->lineNum = line;
            this->identifier = identifier;
        }

        friend class SymbolTable;
        // this means that class SymbolTable can actually access the private 
        // and protected members of this class too
};


class SymbolTable{
    public:
        tokenClass *    tokenTable[MAX];
        // is an array of pointers to tokenClass objects
        // this array basically points to ALL tokens we make..
        // and all of these tokens will be of type tokenClass

        funcList * funcTable[MAX];
        tokenClass* expressList[MAX];
        lVList * localVariable[MAX];
        gVList * globalVariable[MAX];
        ArguementList * ArguementTable[MAX];


        int index;
        int indexA, indexV,indexE, indexG, indexL;
        // Arguement,   , Expression, Global Var,  
        SymbolTable()
        {
            index =1;
            indexA =1;
            indexV =1;
            indexE =1;
            indexG =1;
            indexL =1;


            for(int i =0; i<MAX; i++) 
            {
                tokenTable[i] = NULL;
                // now all pointers point to null
                expressList[i] = NULL;
                funcTable[i] = NULL;
                localVariable[i] = NULL;
                globalVariable[i] = NULL;
                ArguementTable[i] = NULL;

            }

        }
};

void sendToParser(TokenType type, int line,string numToken );
void sendToParser(TokenType type, int line);
void error(string error, int lineNum);

#endif