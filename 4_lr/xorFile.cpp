#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void operationXor(string file1, string file2, string resFile)
{
    FILE *read_fp = popen(("./printFile.out " + file1).c_str(), "r");
    char buffer[BUFSIZ + 1];
    if (read_fp != NULL)
    {
        int chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        while (chars_read > 0)
        {
            buffer[chars_read - 1] = '\0';
            printf("%s\n", buffer);
            chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    operationXor(argv[1], argv[2], argv[3]);
}