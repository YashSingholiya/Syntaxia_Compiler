#include <bits/stdc++.h>
#include "parser.h"
#include "semantics.h"
using namespace std;

SymbolTable st;
stack <string> scopeCount;
stack <tokenClass*> char_stack;
// stores all the operators while evaluating expressions
int scopeDepth;
string Fname = "";
string track_fname="";
int LvIndex = 0;
int lIndex = 0;
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
void error(string error, int lineNum)
{
    cout<<error<<" "<<lineNum<<endl;
    exit(1);
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
//                                  EXPRESSION EVALUATION

static void assignReg(int pointer);
static void readExpressList(int pointerStart, int pointerEnd);
int tenary = 0;
int expression(int pointer, TokenType express){
    // pointer points to first token in expression
    // express as last token of expression
    // Given F= 25*2+8/2-7(A-9)+3*B-C;
    // pointer->F and express = semicolon ;


    st.indexL = 0;
    // indexL -> index to the expression list
    int braces =0;
    int pointerStart = pointer;

    // expression function uses postfix notation
    // so we solve it from end to start...hence step1-> move pointer to 
    // end of expression:

    while(!(expect(express, pointer))||(braces != 0)){

        if((st.tokenTable[pointer]->type  > TOKEN_OUTPUT)
        ||(st.tokenTable[pointer]->type  == TOKEN_SEMICOLON)){
              error("invalid syntax in line", st.tokenTable[pointer]->lineNum);
            //   Tokens greater than token output from our tokens list, are not computable tokens so whenever they
            //    appear in an expression, we throw an error
          }else    
          if(st.tokenTable[pointer]->type  == TOKEN_EOF){
              error("Unexpected end of file on line", st.tokenTable[pointer-2]->lineNum);
          }else 
          if((expect(TOKEN_LEFT_PAREN, pointer))){
              braces++;
          }else
          if((expect(TOKEN_RIGHT_PAREN, pointer))){
              braces--;
          }else
          if((expect(TOKEN_TERNARY, pointer))){
              scopeCount.push("*ten_"+to_string(scopeDepth));
              scopeDepth++; 
              tenary = pointerStart;
              pointerStart = pointerStart + 2;                  
              break;
          }else
          if((expect(TOKEN_FUNCALL, pointer))){         
              c_tokenfuncall(pointer); 
  
          }else
          if(
          ((st.tokenTable[pointer+1]->type > TOKEN_END)&&(st.tokenTable[pointer]->type > TOKEN_END))
          ||(st.tokenTable[pointer]->type > TOKEN_INFINITE)
          ){
              error("Invalid token expression in line", st.tokenTable[pointer]->lineNum);
                
          }        

        //   finally, if last token is reached, and scope is finished, ONLY then 
        // is the expression over. This part is VVIMP
          if((expect(express, pointer))&&(braces == 0)){
              break;
          }
        
          pointer++;
      }
    int pointerEnd = pointer; 
    pointerEnd--;
    // last token (here semicolon) is just expression terminator
    // i.e. not part of the expression

    // ----------------------------------------------------------------------------
    // variable assignment and error checking part
    // Operand Stack updation part
    
    while(pointerEnd >= pointerStart){

        if((st.tokenTable[pointerEnd]->type == TOKEN_STRING) &&(!expect(TOKEN_SEMICOLON, pointerEnd+1))
        &&(!expect(TOKEN_EQUAL, pointerEnd-1)))
        {
            error("Invalid string expression on line", st.expressList[pointerEnd]->lineNum);
            // as the only string expression that our compiler supports is:
            // car = "Hello world" ;
        }

        if(expect(TOKEN_START, pointerEnd)){ 
            while(!expect(TOKEN_END, pointerEnd)){
                pointerEnd--;
            }pointerEnd--;
            st.expressList[st.indexL] = st.tokenTable[pointerEnd];
            st.indexL++;
        }else     
        if((expect(TOKEN_STRING, pointerEnd))
        ||(expect(TOKEN_IDENTIFIER, pointerEnd))||(expect(TOKEN_NUMBER, pointerEnd))){
            if((expect(TOKEN_RIGHT_PAREN, pointerEnd-1))||(expect(TOKEN_LEFT_PAREN, pointerEnd+1))){ 
                if(!(expect(TOKEN_RIGHT_PAREN, pointerStart-1))&& !(expect(TOKEN_LEFT_PAREN, pointer+1))){                 
                    char_stack.push(new tokenClass(TOKEN_STAR, 0, 0));
                    // outer if: basically if we have )7 or 8( 
                    //              it should be treated as )*7 and 8*(
                    // innner if: ensures if there is:   ..)2 = 3(...
                    // it doesnt do  ...)*2 = 3*(... 

                    // also
                    // strings can only appear in this form:
                    // a = "Hello World"
                }

            }
            if(st.tokenTable[pointerEnd]->type == TOKEN_IDENTIFIER){
                // only identifiers store data, so only they will have registers
            assignReg(pointerEnd); 
            }
            st.expressList[st.indexL] = st.tokenTable[pointerEnd];
            st.indexL++;
            
        }else  
        if((expect(TOKEN_FALSE, pointerEnd))||(expect(TOKEN_TRUE, pointerEnd))){
            if(expect(TOKEN_FALSE, pointerEnd)){
                // treat False as 0 and true as 1 when they are used in expressions
                // for example: while(true){c++}
                st.expressList[st.indexL] = new tokenClass(TOKEN_NUMBER, 0 , "0");
                st.indexL++;
            }else{
                st.expressList[st.indexL] = new tokenClass(TOKEN_NUMBER, 0, "1");
                st.indexL++;
            }
        }else 
        if(expect(TOKEN_RIGHT_PAREN, pointerEnd)){
            char_stack.push(st.tokenTable[pointerEnd]);
            // just follow the rules of postfix 

        }else 
        if (expect(TOKEN_LEFT_PAREN, pointerEnd)) {
			while (!(char_stack.top()->type == TOKEN_RIGHT_PAREN)) {
                // pop into main expression until you reach )
                st.expressList[st.indexL] = char_stack.top();      
                st.indexL++;
				char_stack.pop();
                if(char_stack.empty())                        
                    error("Missing ')' in expression on line", st.tokenTable[pointerEnd]->lineNo);                
			}
			char_stack.pop();
            // finally also pop the ) you obviously need not add this into 
            // main postfix expression as thats the purpose of postfix in teh first place

		}else           
        // -----------------------------------------------------------
        // Sign Accomodation part
        if((st.tokenTable[pointerEnd]->type == TOKEN_PLUS)
        &&(st.tokenTable[pointerEnd-1]->type < TOKEN_BANG)){
            // all tokens < tOKEN_BANG are operators. Here we simply 
            // ignore it as 
            // 7*+3 = 7*3 and 9/+8 = 9/8 and so on

        }else
        if((st.tokenTable[pointerEnd]->type == TOKEN_SUB)
        &&(st.tokenTable[pointerEnd-1]->type < TOKEN_BANG)){
            // Similarly even - sign is accomodated: 7*-3 is converted into 
            // 7* (0-3) into the internal expression so no errors are thrown
                st.expressList[st.indexL] = new tokenClass(TOKEN_NUMBER, 0, "0");
                st.indexL++;
                // push zero 
                st.expressList[st.indexL] = new tokenClass(TOKEN_SUB, 0, 0);
                st.indexL++;                
                // push minus
        }else  
        //----------------------------------------------------------------------
        // Operator Stack updation part
        if (st.tokenTable[pointerEnd]->type < TOKEN_LEFT_PAREN){ 
            // this includes all the operators including unary operators
            if(!char_stack.empty()){
                // pop all the operators having LOWER precedence such that 
                // 
                while ((st.tokenTable[pointerEnd]->type <= char_stack.top()->type)
                &&(char_stack.top()->type != TOKEN_RIGHT_PAREN))
                {
                    st.expressList[st.indexL] = char_stack.top();
                    st.indexL++;
                    char_stack.pop();
                    if(char_stack.empty())                        
                        break;
                }
                char_stack.push(st.tokenTable[pointerEnd]);
                
            }else{
                char_stack.push(st.tokenTable[pointerEnd]);
                }
		}              
        pointerEnd--;
    }
   
    while(!char_stack.empty())
    {
        if(char_stack.top()->type  != TOKEN_RIGHT_PAREN)
        {
            // don't forget we want to skip all parenthesis from being pushed into postfix
            st.expressList[st.indexL] = char_stack.top();
            st.indexL++;
        }
        char_stack.pop(); 
    }

    //---------------------------------------------------------------------------

    readExpressList(st.indexL, 0);
    pointer++;
    // pointer now points to the last terminator token, i.e. to the semicolon
    // at end of expression

    return pointer;

}

//-----------------------------------------------------------------------------
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

int c_rightBrace(int pointer){
    // meant for scope termination
    // many types of scopes- if scope, function scope, while scope etc
    pointer++;

    // case1: right brace belongs to function scope
    if(scopeCount.top().find("*func_") != string::npos)
    {
        // npos is highes possible value for string
        // npos is used to check if one strinh is contained in another
        // larger string
        // basically, this is in case "*func_" IS actually found
        // meaning that this is indeed a function scope (case 1)

        // Now,,, we need to pop the stackframe
        string printfunc = scopeCount.top();
        printfunc.erase(0, 5);
        codeGenerator(printfunc+"_exit:");       
        scopeCount.pop();   
        int NumLocalVariables = st.funcTable[st.indexE -1]->Vn;
        int vCnt = ( NumLocalVariables * 16 ) +  32;
        // total variables, including stackframe and stackpointer
        int cnt =0;
        while(vCnt > 32)
        {
            vCnt -= 16;
            codeGenerator("ld    "+saveV[cnt]+",  "+to_string(vCnt)+"(sp)");  
            cnt++; 
        }
        codeGenerator("ld    x8,  16(sp)"); 
        // x8 is for saved or frame pointer
        codeGenerator("ld    x1,  0(sp)"); 
        // x1 is for return address
        codeGenerator("addi  sp   sp   "+to_string(NumLocalVariables)); 
        codeGenerator("jalr  x0,   0(x1)");     

    }

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
    // first checking if function name already exists
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

    if(expect(TOKEN_LEFT_BRACE,pointer))
    {
        st.funcTable[st.indexE -1] ->call =1;
        scopeCount.push("*func_"+ Fname);
        // pusing the function into scope only if its initialized
        // This way, WHENEVER we access top of scope stack, we can 
        // easily identify which funcion we are currently in
        // *func_ is unique identifier for all functions
        // also, no need to add depth, as no two functions can have
        // same name
        
        codeGenerator(Fname : ":");
        // This is always the first code generated for ANY function
        // so that we can JUMP to that functin whenever its called
        int braces =1;
        int variable =0;
        pointer++;
        // pointer out of function
        count = pointer;

        // first we want to find total number of variables in this func
        while(braces != 0){
            if(st.tokenTable[count]->type == TOKEN_LEFT_BRACE){
                braces++;
            }
            else 
            if(st.tokenTable[count]->type == TOKEN_RIGHT_BRACE){
                braces--;
            }else 
            if(expect(TOKEN_VAR, TOKEN_IDENTIFIER, count) 
            || expect(TOKEN_STRINGVAR, TOKEN_IDENTIFIER, count) 
            || expect(TOKEN_BOOL, TOKEN_IDENTIFIER, count)){
                variable++;   
            }else
            if(st.tokenTable[count]->type == TOKEN_EOF){
                error("Function  "+Fname+"  have incompatible number of braces", 0);
            }
            count++;
        }

        int allocate_space = (variable * 16) + 32;
        // 32 is for the two ragisters- frame pointer fp and stack pointer sp
        braces = variable;
        count =0;

        ccodeGenerator("addi  sp,  sp,  -"+to_string(allocate_space));

        while(variable>32)
        {
            variable-= 16;
            codeGenerator("sd    "+saveV[count]+",  "+to_string(allocate_space)+"(sp)"); 
            // saves all the variables into stack..using save registers
        }
        codeGenerator("sd    x8,  16(sp)");
        // sd - store data to memory, which in our case is same as stack
        // 
        codeGenerator("sd    x1,  0(sp)");
        codeGenerator("addi  fp   sp   "+to_string(braces));

        st.funcTable[st.indexE -1] ->Vn = braces;
        // number of variable for the function is set
    }
    // case 2: It was function declaration only, not initialization 
    else if(expect(TOKEN_SEMICOLON, pointer))
    {
        st.funcTable[st.indexE -1]->call = 0;
        pointer++;
        // pointer out of function
    }
    else
    {
        error("MISSING ; or { in function "+Fname+ " on line", st.tokenTable[pointer]->lineNo);
    }
    return pointer;
}

int c_varDeclaration(int pointer){

                                // part a.

    // following if setup block executes only once for every function
    if(track_fname != Fname)
    {
        track_fname =Fname;
        // This will run a. first time for first variable in code
        // b.when Fname changes then track_fname still holds previous fName and 
        // hence change of Fname can be detected

        LvIndex =0;
        // This is local index of local variable for this function's LV list  
        // this index is reset once in every new function and is 
        // used to assign index to variables. 
        lIndex = st.indexV;
        // This is global index for this local variable in local Var list object
        // This lIndex will now point to index in memory where local_variables start...
        // This will be constant for the whole function as new variables are encountered 
        // and st.indexV will increase for every new variable
        st.funcTable[st.indexE-1]->Vs = lIndex;
        // Hence, the start index of all variable for this function is assigned
    }
    // Now if we want to search for a given local variable, then we can directly do so
    // by referring to the function object it belongs to and use its Vs attribute
    // rather than searching the whole local variable list

    int Lsearch =   st.indexV; //index of current variable

    if(!scopeCount.empty())
    {
        // scope not empty..so this variable is actually a local variable
        while(Lsearch>lIndex)
        {
            // basically searching all local vars encountered so far IN THIS FUNCTION
            if(st.tokenTable[pointer + 1]->identifier == st.localVariable[Lsearch-1]->Vname)
            {
                error("Variable double declared on line: ",st.tokenTable[pointer]->lineNum);
            }
            Lsearch--;
        }

        // if we reached so far it means there are no repeated names for this local variable
        // So no semantic problems here:
        st.localVariable[st.indexV] = new lVList(st.tokenTable[pointer]->type, 
            st.tokenTable[pointer+1]->identifier, LvIndex);
        LvIndex++;
        // we are able to use this as the function is parsed sequentially
        st.indexV++;

    }
    else if(scopeCount.empty())
    {
        // its a global variable
        int Lsearch = st.indexG;
        //  index of last global variable -1
        while(Lsearch >0)
        {
            if(st.tokenTable[pointer+1]->identifier == st.globalVariable[Lsearch-1])
            {
                error("Variable double declared on line: ",st.tokenTable[pointer]->lineNum);
            }
            Lsearch--;
        }

        st.globalVariable[st.indexG] = new gVList(st.tokenTable[pointer]->type, st.tokenTable[pointer+1]->identifier);
        st.indexG++
    }

    pointer++;
    // pointer was initially at int now its at x in : 'int x...'
    // ----------------------------------------------------------------------------
    //                              part b.
    // after this We dont know if this was a variable declaration or start of something else

    if(expect(TOKEN_SEMICOLON, pointer)){
        token+=2; 
        // skip the semicolon as there is no semantic error here its perfectly okay
    }
    else if(expect(TOKEN_COMMA, pointer+1))
    {
        // multiple variable declaration
        // int a,b
        // Cool trick we use here:
        st.tokenTable[pointer+1] = st.tokenTable[pointer-1];
        // hence it basically looks like comma has been REPLACED BY int !!
        // and now,
        pointer = c_varDeclaration(pointer+1);
        // same process repeats just there yay
    }
    else
    {
        pointer = expression(pointer, TOKEN_SEMICOLON);
        // note this pointer always points EXACTLY TO the variable 
        // to which you want to assign the expression yay
    }

    return pointer;
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