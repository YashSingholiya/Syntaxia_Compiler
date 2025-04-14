#include <bits/stdc++.h>

#include "semantics.h"
using namespace std;

//The parser is responsible for scanning our token list for correct sentences

// pointer variable -> points to the current token, 
// and we run multiple if statements to find which token it is exactly
// ans ensure that that token right relationship with the ones to next of it
// i.e. the group of tokens has the right syntax

void parse(int pointer){

    while(!expect(TOKEN_EOF, pointer)) //current pointer is NOT EOF
    {
        if(((expect(TOKEN_VAR, pointer))||(expect(TOKEN_BOOL, pointer))
        ||(expect(TOKEN_VOID, pointer))||(expect(TOKEN_PIN, pointer))
        ||(expect(TOKEN_STRING, pointer)))&&(expect(TOKEN_FUN, pointer+1))){
            // for ex, if we encounter: 
            // int hello()  bool scanner() etc.
            pointer = c_tokenfun(pointer);
        }  
        else
        if((expect(TOKEN_FUNCALL, pointer))){
            // for ex: jump();
            // parse(1)
            pointer = expression(pointer, TOKEN_SEMICOLON);
        } 
        else
        if(expect(TOKEN_VAR, TOKEN_IDENTIFIER, pointer) 
        || expect(TOKEN_STRINGVAR, TOKEN_IDENTIFIER, pointer) 
        || expect(TOKEN_BOOL, TOKEN_IDENTIFIER, pointer)){

            // now for sure after int we dont have function call, as this is else
            // so it must be variable declaration time yay
            // ex: int x, string hello
            pointer = c_varDeclaration(pointer);
        }
        else 
        if(scopeCount.top().find("*tenary") != string::npos){
            // F= a>b? 2+y :7;
            // this will be detected easily by scope as:
            // ternary operation is nothing but an if statement itself
            pointer = c_tenary(pointer);
        }
        else          
        if(expect(TOKEN_COUT, TOKEN_SHIFT_LEFT, pointer)){
            // cout<<
            pointer = c_cout(pointer);
        }
        else  
        if(expect(TOKEN_IDENTIFIER, TOKEN_COLON, pointer)){ 
            // label declaration means:
            // scan:
            // switch:
            // scanner: and so on
            pointer = c_label(pointer); 
        }else           
        if(expect(TOKEN_GOTO, TOKEN_COLON, pointer)){ 
            pointer = c_goto(pointer); 
        }else      
        if(expect(TOKEN_WHILE, TOKEN_LEFT_PAREN, pointer)){      
            //declared when while is followed by left paranthesis
            // note: we dont have to check for right parenthesis, it will surely exist 
            // as scanner.cpp ensured that
            //while(0
            pointer = c_while(pointer);            
        }else    
        if(expect(TOKEN_FOR, TOKEN_LEFT_PAREN, pointer)){
            // same as while
            pointer = c_forloop(pointer);
        }else    
        if(expect(TOKEN_RETURN, pointer)){
            pointer = c_return(pointer); 
        }else   
        if(expect(TOKEN_IF, TOKEN_LEFT_PAREN, pointer)){  
            // declared when if followed by left parenthesis
            pointer = c_if(pointer);
        }else           
        if(expect(TOKEN_ELSE, TOKEN_IF, pointer)){ 
            // declared when else is followed by if
            // else if
            pointer =  c_else(pointer);
        }else
        if(expect(TOKEN_ELSE, TOKEN_LEFT_BRACE, pointer)){
            // marked when we see: else{
            // here, we basically want to proceed as usual with what we were doing,
            // no control problems after this hence we directly do pointer+=2
            pointer = pointer +2 ;     
        }else         
        if((expect(TOKEN_CONTINUE, TOKEN_SEMICOLON, pointer))){    
            // continue;
            pointer = c_continue(pointer);
        }else    
        if(expect(TOKEN_IDENTIFIER, pointer)){ 
            // for special functoin
            pointer = expression(pointer, TOKEN_SEMICOLON); 
        }else       
        if(expect(TOKEN_SWITCH, TOKEN_LEFT_PAREN, pointer)){  
            // switch{
            // is what calls this
            pointer = c_switch(pointer);
        }else      
        if(expect(TOKEN_CASE, pointer)){  
            // case is enough to invoke this
            // this is also part of the switch statement
            pointer = c_case(pointer);
        }else       
        if(expect(TOKEN_DEFAULT,TOKEN_COLON, pointer)){
            // default:
            // these two tokens are required for this
            pointer = c_default(pointer);
        }else       
        if(expect(TOKEN_BREAK,TOKEN_SEMICOLON,  pointer)
        && (scopeCount.top().find("*case") != string::npos)){
            // this is the break; that we encounter in switch statement to terminate cases
            pointer = c_swbreak(pointer);
        }
        else    
        if(expect(TOKEN_BREAK, TOKEN_SEMICOLON, pointer)){
            // due to else if, this means that this break; is the one which we encounter
            // in a while or a for loop
            pointer = c_break(pointer);
        }
        else
        if(expect(TOKEN_RIGHT_BRACE, pointer)){
            // }
            // This is the VERY important token and is used to terminate a 
            // LOT of scopes in this program
            pointer = c_rightBrace(pointer);
        }
        else{
                error("Syntax error on line", st.tokenTable[pointer]->lineNo);
            } 
    }
    
    if(expect(TOKEN_EOF, pointer)){
        exit(0);
    }
}


// All the functions called here will be discussed in semantics.cpp
