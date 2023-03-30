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
                // buffer[chars_read] = '\0';
                // printf("%s\n", buffer);
                out_prog[i].append(buffer);
                chars_read = fread(buffer, sizeof(char), BUFSIZ, prog);
            }
            fclose(prog);
        }
        // cout << out_prog[i] << endl;
    }

    FILE *out_file = fopen(resFile.c_str(), "w");
    string string_out;

    if (out_file != NULL)
    {
        // const char *char_file1 = out_prog[0].c_str();
        // const char *code = out_prog[1].c_str();
        string file = out_prog[0];
        string code = out_prog[1];
        int size_code = code.size();
        // cout << "end" <<  code[size_code - 2] << "end1" << code[size_code - 3];

        for (int i = 0; i < file.size() - 1; i++)
        {
            char c = (file[i]);
            char cc = code[i % (size_code - 1)];

            if (c == cc)
            {
                string_out.push_back(c);
                continue;
                // for (int j = 1; c == cc; j++)
                // {
                //     cc = code[(i + j) % (size_code - 1)];
                // }
            }

            // cout << "c = " << c << endl;
            string_out.push_back(c ^ cc);
        }

        fputs(string_out.c_str(), out_file);
    }
}

int main(int argc, char *argv[])
{
    operationXor(argv[1], argv[2], argv[3]);
}