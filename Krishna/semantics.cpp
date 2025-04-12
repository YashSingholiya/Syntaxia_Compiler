#include <bits/stdc++.h>
#include "parser.h"
#include "semantics.h"
using namespace std;

SymbolTable st;
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