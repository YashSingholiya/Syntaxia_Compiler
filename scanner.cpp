#include  <bits/stdc++.h>
#include "tokens.h"
using namespace std;
// Note:
// There are 4 groups the scanner/ lexical analyser searches for:
// Numbers(0 to 9 only), Whitespaces(spaces, tabs, comments and newlines) - to be skipped,
// Special Characters(neither letter nor number ex. !@#()<>?.&*{}[],/ etc.) 
// Alphabets- Keywords and Identifiers
// All other tokens will be treated as error


char* token;
int line =1; //stores the line where the token occurs. Useful to report errors too
// All c++ files start at line 1 not 0
int parencount =0;
int bracecount =0;
//to check for invalid braces and parenthesis

// all function prototypes
static bool end_of_file();
static void skip_whitespace();
void generateToken(TokenType type);
static bool isNum(char ch);
static void keywordToken();
static void charToken();
static bool isAlphabet(char ch);
void makeToken(TokenType type);


void scanner(char * buffer)
{
    token = buffer;

    while(!end_of_file())
    {
        skip_whitespace();
        if(isNum(*token))
        {
            generateToken(TOKEN_NUMBER);
        }
        else if(isAlphabet(*token))
        {
            keywordToken();
            // accomodates both keywords and identifiers
        }
        else
        {
            charToken();
            // This is for all the special characters
            // will do the renaming later
        }
        token++;
    }

    if(end_of_file())
    {
        makeToken(TOKEN_EOF);
    }
    
}


// ........................................................
// Now initializing all the functions defined above scanner
static bool end_of_file()
{
    if(token[0] == '\0')return true;
    else return false;
}

//whitespace functions
static void skipTillToken(char ch);
static void skipTillToken(char c1, char c2);
static void skip_whitespace()
{
    switch(*token)
    {
        case ' ':
            token++;
            skip_whitespace();
            break;

        case '\r':
            token++;
            skip_whitespace();
            break;

        case '\t':
            token++;
            skip_whitespace();
            break;

        case '\n':
            token++;
            line++;
            skip_whitespace();
            break;

        // Next up is // and /*   */, i.e. the comments
        case '/':
            if(token[1] == '/')
            {
                token++;
                skipTillToken('\n');
                // skips everything till the next '\n' and even skips that
                line++;
            }
            else if(token[1] == '*')
            {
                token++;
                skipTillToken('*','/');
                // as the multiline comment stops being read when */ is encountered
                line++;
            }
            else
            {
                makeToken(TOKEN_SLASH);
                token++;
            }
            skip_whitespace();
            break;

        default:
            break; 
    }
}
static void skipTillToken(char ch)
{
    while((!end_of_file())  && (token[0] != ch )) token++;

    if(end_of_file())
    {
        error("Unexpected End of file on line ", line);
    }
}
static void skipTillToken(char c1, char c2)
{
    while((!end_of_file())  &&  !((token[0] == c1)  &&  (token[1] == c2)) )
    {
        token++;
    }

    // token++;
    // Not doing this, seems logically wrong pls check again

    if(end_of_file())
    {
        error("Unexpected End of file on line ", line);
    }
    else// */ has been encountered 
    {
        token+=2;
    }
}


//functions to check for number/alphabet
static bool isNum(char ch)
{
    int a = ch - '0';
    if(a>=0 && a<=9 )return true;
    else return false;
}
static bool isAlphabet(char ch)
{
    if( (ch >= 'a' && ch<='z')  || (ch >='A' && ch<='Z')  || (ch == '_'))
    {
        return true;
    }
    else return false;
}


//Functions to check for Keyword and identifiers 
static bool checkKeyword(string charValue, int numchar, TokenType type);
static void keywordToken()
{
    switch(*token)
    {
        case 'b':
            if(checkKeyword("reak", 4, TOKEN_BREAK)) {token += 4; }
            else if(checkKeyword("ool", 3, TOKEN_BOOL)) {token += 3;}
            else generateToken(TOKEN_IDENTIFIER);
            break;

        case 'c':
            if(checkKeyword("ase", 3, TOKEN_CASE)) {token += 3; }
            else if(checkKeyword("out", 3, TOKEN_COUT)) {token += 3;}
            else if(checkKeyword("ontinue", 7, TOKEN_CONTINUE)) {token += 7;}
            else generateToken(TOKEN_IDENTIFIER);
            break;

        case 'd':
            if(checkKeyword("efault", 6, TOKEN_DEFAULT)) {token += 6; }
            else generateToken(TOKEN_IDENTIFIER);
            break;

        case 'e':
            if(checkKeyword("lse", 3, TOKEN_ELSE)){token = token+3;}
            else if(checkKeyword("ndl", 3, TOKEN_ENDL)){token = token+3;}
            else{generateToken(TOKEN_IDENTIFIER);}            
            break;

        case 'f':
            if(checkKeyword("or", 2, TOKEN_FOR)){
                token = token+2;}              
            else if(checkKeyword("alse", 4, TOKEN_FALSE)){
                token = token+4;}
            else{generateToken(TOKEN_IDENTIFIER);}          
            break;

        case 'g':
            if(checkKeyword("oto", 3, TOKEN_GOTO)){
                token = token+3;}              
            else{generateToken(TOKEN_IDENTIFIER);}          
            break;

        case 'i':
            if(checkKeyword("f", 1, TOKEN_IF)){token++;}
            else
            if(checkKeyword("nt", 2, TOKEN_VAR)){
                token = token+ 2;}
            else{generateToken(TOKEN_IDENTIFIER);}          
            break;  

        case 'n':
            if(checkKeyword("il", 2, TOKEN_NIL)){token = token+2;}
            else{generateToken(TOKEN_IDENTIFIER);}
            break;

        case 'p':
            if(checkKeyword("rint", 4, TOKEN_PRINT)){
                token = token+4;}
            else if(!checkKeyword("in", 2, TOKEN_PIN)){
                    generateToken(TOKEN_IDENTIFIER);}               
            break;

        case 'r':
            if(checkKeyword("eturn", 5, TOKEN_RETURN)){token = token+5;}
            else{generateToken(TOKEN_IDENTIFIER);}
            break;

        case 's':
            if(checkKeyword("witch", 5, TOKEN_SWITCH)){
                token = token+5;
            }else            
            if(checkKeyword("tring", 5, TOKEN_STRINGVAR)){  
                token = token+5;}           
            else{generateToken(TOKEN_IDENTIFIER);}
            break;   

        case 't':
            if(checkKeyword("rue", 3, TOKEN_TRUE)){token = token+3;}
            else{generateToken(TOKEN_IDENTIFIER);}
            break;

        case 'w':
            if(checkKeyword("hile", 4, TOKEN_WHILE)){token = token+4;}
            else{generateToken(TOKEN_IDENTIFIER);}
            break;

        case 'v':
            if(checkKeyword("oid", 3, TOKEN_VOID)){token = token+3;}
            else{generateToken(TOKEN_IDENTIFIER);}
            break;    

        default:
            generateToken(TOKEN_IDENTIFIER);
            break;
    }
}
static bool checkKeyword(string charValue, int numchar, TokenType type)
{
    if(!(isAlphabet(token[numchar+1])) && !(isNum(token[numchar+1])) )
    {
        // for ex: break1 or breaks should not be take as keyword, but break= or break; 
        // should indeed be taken as keyword

        for(int i = numchar; i>0; i--)
        {
            if(charValue[i-1] != token[i]) return false;
        }
        //verefying if SYNTAX is correct for a keyword
        if(type == TOKEN_ENDL)
        {
            makeToken("'/n'", TOKEN_STRING);
        }
        else
        {
            makeToken(type);
            // thus the actual token is generated
        }

        return true;
    }
    else return false;
}


//Functions to check for special characters
static void checkField(int type); //checks the validity of brace syntax
static void charToken()
{
    if(end_of_file()) return;

    switch( *token)
    {
        case '(':
            checkField(0);
            parencount++;
            makeToken(TOKEN_LEFT_PAREN);
            break;
        
        case ')':
            checkField(1);
            parencount--;
            makeToken(TOKEN_RIGHT_PAREN);
            break;

        case '{':
            checkField(2);
            bracecount++;
            makeToken(TOKEN_LEFT_BRACE);
            break;

        case '}':
            checkField(3);
            bracecount--;
            makeToken(TOKEN_RIGHT_BRACE);
            break;

        case ';':
            if(token[1] == ';')
            {
                generateToken(TOKEN_INFINITE);
                token++;
                // skipping over the extra ;
            }
            else makeToken(TOKEN_SEMICOLON);
            break;
        
        case ',':
            makeToken(TOKEN_COMMA);
            break;
        
        case '.':
            makeToken(TOKEN_DOT);
            break;
        
        case '-':
            if(token[1] == '+')
            {
                makeToken(TOKEN_SUB);
                token++;
            }
            else if(token[1] == '-')
            {
                makeToken(TOKEN_MINUS_MINUS);
                token++;
            }
            else if(token[1] == '=')
            {
                makeToken(TOKEN_SUB_ASS);
                token++;
            }
            else
            {
                makeToken(TOKEN_SUB);
            }
            break;
        
        case '+':
            if(token[1] == '+')
            {
                makeToken(TOKEN_PLUS_PLUS);
                token++;
            }
            else if(token[1] == '-')
            {
                makeToken(TOKEN_SUB);
                token++;
            }
            else if(token[1] == '=')
            {
                makeToken(TOKEN_ADD_ASS);
                token++;
            }
            else
            {
                makeToken(TOKEN_PLUS);
            }
            break;

        

    }
}
static void checkField(int type)
{
    switch(type)
    {
        case 1:
            if(parencount == 0) {error("Unexpected ) on line ", line);}
            break;
            // Basically, every ) must have a ( prior to it 
        
        case 2:
            if(parencount!= 0) {error("Unexpected { on line ", line);}
            break;
            // basically, ({) is not alowed in c++

        case 3:
            if(bracecount == 0) {error("Unexpected } on line ", line);}
            if(parencount != 0) {error("Unexpected } on line ", line);}
            break;
            // Basically, every } must have a { prior to it 
            // and (} is not allowed by c++
    }
}