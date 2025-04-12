#include <bits/stdc++.h>
#include "semantics.h"
using namespace std;

//The parser is responsible for scanning our token list for correct sentences

// pointer variable -> points to the current token, say INT which has some number,
// 

void parse(int pointer)
{
    while(!expect(TOKEN_EOF,pointer ))
    {

    }

    if(expect(TOKEN_EOF, pointer))
    {
        exit(0);
    }
}
