#include <bits/stdc++.h>
#include <stdio.h>
#include "scanner.h"
using namespace std;

int main(int argc, char** argv)
{
    char * source_file = argv[1];

    FILE * fp;
    fp = fopen("test.cpp", "r");

    if(!fp)
    {
        cout<<"Failed to open file";
        exit(1);
    }
    else
    {
        fseek(fp, 0, SEEK_END);
        int file_size = ftell(fp);
        // In c++, ftell is used to calculate size of file, in this way
        fseek(fp,0,SEEK_SET);
        // bringing fp back to start of file


        char * buffer = NULL; 
        buffer = new (nothrow) char[file_size+1];

        if(!buffer) 
        {
            cout<<"Allocation of buffer memory failed\n";
        }
        else
        {
            fread(buffer, sizeof(char), file_size, fp);
            // contents of test.cpp copied into buffer
            scanner(buffer); 
            //called from scanner.h

            cout<<"Compiled Successfully!";
        }
        delete buffer;
        fclose(fp);

    }

    return 0;

}

