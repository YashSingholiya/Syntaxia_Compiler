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
static bool end_of_file();
static void skip_whitespace();
void generateToken(TokenType type);
bool isNum(char ch);
void keywordToken();
void charToken();
bool isAlphabet(char ch);
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



// Now initializing all the functions defined above scanner
static bool end_of_file()
{
    if(token[0] == '\0')return true;
    else return false;
}

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

    // token++;Not doing this, seems logically wrong pls check again

    if(end_of_file())
    {
        error("Unexpected End of file on line ", line);
    }
    else// */ has been encountered 
    {
        token+=2;
    }
}
