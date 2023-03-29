#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void printFile(string nameFile)
{
    FILE *read_fp = popen(("cat " + nameFile + " -E -s").c_str(), "r");
    char buffer[BUFSIZ + 1];
    // memset(buffer, '\0' , sizeof(buffer));

    if (read_fp != NULL)
    {
        int chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        while (chars_read > 0)
        {
            buffer[chars_read - 1] = '\0';
            printf("%s\n",buffer);
            chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    printFile("/home/rodion/ocCode/4_lr/file1");

    return 0;
}