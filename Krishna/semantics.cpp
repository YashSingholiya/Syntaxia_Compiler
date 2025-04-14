#include <bits/stdc++.h>
#include "parser.h"
#include "semantics.h"
using namespace std;

SymbolTable st;
stack <string> scopeCount;
int scopeDepth;
// depth helps make every item within the scope stack as unique
// ex. if we encounter 2 iffs inside scope, they will be pushed as if_1 and if_2

// Now some helper functions:
bool expect(TokenType typeA, TokenType typeB, TokenType typeC,TokenType typeD, int count)
{
    if((typeA  == st.tokenTable[count]->type) && (typeB  == st.tokenTable[count +1]->type) && (typeC  == st.tokenTable[count+2]->type) && (typeD  == st.tokenTable[count +3]->type))
    {
        return true;
    }
    else return false;
}
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, int count)
{
    if((typeA  == st.tokenTable[count]->type) && (typeB  == st.tokenTable[count +1]->type) && (typeC  == st.tokenTable[count+2]->type))
    {
        return true;
    }
    else return false;
    
}
bool expect(TokenType typeA, TokenType typeB, int count)
{
    if((typeA  == st.tokenTable[count]->type) && (typeB  == st.tokenTable[count +1]->type) )
    {
        return true;
    }
    else return false;
    
}
bool expect(TokenType typeA, int count)
{
    if((typeA  == st.tokenTable[count]->type))
    {
        return true;
    }
    else return false;
    
}


int deleteOld = 0;

string saveG[12]       = {"g8", "g9", "g18", "g19", "g20", "g21", "g22", "g23", "g24", "g25", "g26", "g27"};
// to save global variables
string argV[8]         = {"x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17"};
// to save arguements
string saveV[12]       = { "x9", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27"};
// saved variable registers
string tempV[7]        = {"x5", "x6", "x7", "x28", "x29", "x30", "x31"};
// temporary variable registers storage
string instruction[19] = { "add","sub", "div","mod", "mul", "or", "xor", "and", "or",
                         "xor", "and", "sll", "srl", "not", "add", "sub", "div", "mod", "mul"};
// stores all the arithmetic expression our compiler will support in ORDER of precedence
// deals with TOKEN_ADD_ASS to TOKEN_STAR
string branch[9]       = { "ld", "0", "0", "bne","ble", "blt", "bge", "bgt", "beq"};
// deals with TOKEN_EQUAL to TOKEN_BANG
// "0", "0" because tokens logical OR and logical AND not printed on completion 


//-----------------------------------------------------------------------------
void error(string error, int lineNum)
{
    cout<<error<<" "<<lineNum<<endl;
    exit(1);
}
// Now creating the final file to print generated low level code into
void writeFile(string Data)
{
    int len = Data.length();
    char char_array[len+1];
    strcpy(char_array, Data.c_str());

    FILE * fp;
    if(deleteOld == 0)
    {
        deleteOld++;
        if(fp == fopen("assembly_code.txt","r"))
        {
            fclose(fp);
            if(remove("assembly_code.txt") == 0){}
        }
    }
    fp = fopen("assembly_code.txt", "a");
    fputs(char_array, fp);
    fputs("\n", fp);
    fclose(fp);
}


int c_tokenfun(int pointer)
{
    // This is for function declaration
    // Note that every function looks like: void scanner(...)
    // so tokens inputter will be TOKEN_VOID, TOKEN_FUN
    // or TOKEN_VAR, TOKEN_FUN and so on

    // So in this part we are going to do 4 steps:
    // A.add out function to functionlist
    // B.add its arguements to arguementlist
    // C.compiler function using risc-v
    // D.push the function to scope

    // pointer -> the index of token_type in the token table
    // -----------------------------------------------------------
                            // part A.
    int funcIndex = st.indexE -1;
    // subtract by one as we add 1 to all indexes whenever an 
    // item is added to symbol table

    pointer++;
    // now points to TOKEN_FUN
    while(funcIndex>0)
    {
        if((st.tokenTable[pointer]->identifier == st.funcTable[funcIndex]->Fname)
        && (st.funcTable[funcIndex]->call ==1) )
        {
            error("function already exists",st.tokenTable[pointer]->lineNum)
        }
        // if call ==1 then function also has been initialized 
        // and so we can make another function of the same name
        // (polymorphism)
        funcIndex--;
    }

    Fname = st.tokenTable[pointer]->identifier;
    // storing the identifier value of this function here
    st.funcTable[st.indexE] = new funcList(0,Fname,0,0,0,0);
    st.indexE++;
    // This is (the index of the current function in list of all functions) +1

    // -----------------------------------------------------------
                            // part B.
    // All this being done: lets move on and work on arguements of this function

    st.funcTable[st.indexE -1]->As = st.indexA;
    // also update the class funclist side by side with class s.t.
    pointer+=2;
    // ex. int scan(bool start) rn pointer is at scan
    // with +=2, pointer skips ( and goes to bool
    // now it could even be int scan() so its also possible that
    // pointer is pointing to ) (whitespaces are not tokenized)
    int count = pointer; //preserving the pointer
    int argNo =0;
    if(st.tokenTable[pointer]->type != TOKEN_RIGHT_PAREN)
    {
        // arguements DO exist
        argno =1;
        // first lets count total number of args. But we dont want
        // to disturb the main pointer here, so we use substitute pointer

        // how to do it? easy- num of params = num of commas +1 always
        while(!expect(TOKEN_RIGHT_PAREN, count)){
            if(st.tokenTable[count]->type == TOKEN_COMMA)
            {
                argNo++;
            }
            count++;
        }
    }
    st.funcTable[st.indexE -1]->An = argNo;
    if(argNo == 0)
    {
        pointer++;
        // now pointer has even left the closing brace
        // pointer points to next token after closing paren.
    }
    while(argNo!=0)
    {
        // initially pointer points to bool (datatype of first arg)
        st.ArguementTable[st.indexA] = new ArguementList(st.tokenTable[pointer]->type,
            st.tokenTable[pointer+1]->identifier, argNo-1 );
        pointer+=3;
        // skips name(which just got recorded above), and comma and goes straight to datatype
        // of next arg
        st.indexA++;
        argNo--;
    }
    // in the end, the pointer points to whatever is next right after the closing parenthesis
    // of function initialisation OR declaration

    // -----------------------------------------------------------
                                // part C.
    // case1: it was a function initialization
    st.funcTable[st.indexE -1] ->call =1;
    scopeCount.push("*func_"+ Fname);
    

}

void sendToParser(TokenType type, int line, string numToken)
{
    st.tokenTable[st.index] = new tokenClass(type, line, numToken);
    st.index++;
}
void sendToParser(TokenType type, int line)
{
    st.tokenTable[st.index] = new tokenClass(type, line);
    st.index++;
    // don't forget this

    if(type== TOKEN_EOF)
    {
        writeFile(".text");
        writeFile(".extern printf");
        writeFile(".global main");
        parse(1);
        // as soon as last token is seen, Start parsing
        // Also print the above three messages they are important
    }

} 