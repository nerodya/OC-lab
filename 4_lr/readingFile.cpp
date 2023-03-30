#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void printFile(string nameFile)
{
    int data_processed;
    int file_pipes[2];

    FILE *read_fp1 = popen(("cat " + nameFile + " -s").c_str(), "r");

    char buffer_1[BUFSIZ + 1];
    memset(buffer_1, '\0', sizeof(buffer_1));

    if (read_fp1 != NULL)
    {
        int chars_read = fread(buffer_1, sizeof(char), BUFSIZ, read_fp1);
        while (chars_read > 0)
        {
            printf("%s\n", buffer_1);
            chars_read = fread(buffer_1, sizeof(char), BUFSIZ, read_fp1);
        }
        pclose(read_fp1);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    printFile(argv[1]);

    return 0;
}