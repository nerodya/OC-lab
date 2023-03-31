#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void operationXor(string file1, string file2, string resFile)
{
    int count_file = 2;

    string file[count_file];
    file[0] = file1;
    file[1] = file2;

    char buffer[BUFSIZ + 1];

    string out_prog[count_file];

    for (int i = 0; i < count_file; i++)
    {
        memset(buffer, '\0', sizeof(buffer));
        FILE *prog = popen(("./readingFile.out " + file[i]).c_str(), "r");
        if (prog != NULL)
        {
            int chars_read = fread(buffer, sizeof(char), BUFSIZ, prog);
            while (chars_read > 0)
            {
                out_prog[i].append(buffer);
                chars_read = fread(buffer, sizeof(char), BUFSIZ, prog);
            }
            fclose(prog);
        }
    }

    FILE *out_file = fopen(resFile.c_str(), "w");
    string string_out;

    if (out_file != NULL)
    {
        string file = out_prog[0];
        string code = out_prog[1];
        int size_code = code.size();
        for (int i = 0; i < file.size() - 1; i++)
        {
            char char_file = (file[i]);
            char char_code = code[i % (size_code - 1)];
            if (char_file == char_code)
            {
                string_out.push_back(char_file);
                continue;
            }
            string_out.push_back(char_file ^ char_code);
        }

        fputs(string_out.c_str(), out_file);
    }
}

int main(int argc, char *argv[])
{
    operationXor(argv[1], argv[2], argv[3]);
}