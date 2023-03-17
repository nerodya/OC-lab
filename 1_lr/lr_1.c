#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

const char *file_arch = "/home/rodion/ocCode/1_lr/file";
char dirArch[256];
char dirUnzip[256];
char *brackets = ">>>>\n";

char fileDirectoryName[265];

void unzip(const char *fileArch, char *dirUnZip);
void makeArch(char *dir);
int getFileSize(const char *file);
void removeHyphen(char *str);

int main()
{
    printf("enter the archive of which directory you want to create\n");
    scanf("%s[^\n]", dirArch);

    makeArch(dirArch);

    char ch;
    do
    {
        ch = getchar();
    } while (ch != 27);

    printf("enter the directory where to unzip\n");
    scanf("%s[^\n]", dirUnzip);

    unzip(file_arch, dirUnzip);

    exit(0);
}

void makeArch(char *dir)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    chdir(dir);

    if (strcmp(dir, dirArch) != 0)
    {
        strcat(fileDirectoryName, dir);
        strcat(fileDirectoryName, "/");
    }

    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;

            makeArch(entry->d_name);
        }
        else
        {
            FILE *file_in = fopen(entry->d_name, "r");
            if (file_in == NULL)
            {
                printf("%s", "the file did not open for reading");
                exit(0);
            }

            FILE *file_out = fopen(file_arch, "a");
            if (file_out == NULL)
            {
                printf("%s", "the file for recording did not open");
                exit(0);
            }

            fprintf(file_out, "%s\n%s\n%s\n%d\n", "<<<<", fileDirectoryName, entry->d_name, getFileSize(entry->d_name));

            char buffer[1024];

            while ((fgets(buffer, 1024, file_in)) != NULL)
                fputs(buffer, file_out);

            fprintf(file_out, "\n%s\n", ">>>>");
            fclose(file_in);
            fclose(file_out);
        }
    }

    for (int i = strlen(fileDirectoryName) - 2; i >= 0; i--)
    {
        if (fileDirectoryName[i] == '/')
            break;
        fileDirectoryName[i] = '\0';
    }
    chdir("..");
    closedir(dp);
}

void unzip(const char *fileArch, char *dirUnzip)
{
    FILE *arch = fopen(fileArch, "r");

    if (arch == NULL)
    {
        printf("%s", "the file did not open for reading");
        exit(0);
    }

    if ((opendir(dirUnzip)) == NULL)
    {
        printf("cannot open directory: %s\n", dirUnzip);
        exit(0);
    }

    char str[1024];
    char path[256];
    char nextFile[256];

    char *sysCom = "mkdir -p ";
    char *fileUnSiz = dirUnzip;

    chdir(dirUnzip);

    while (feof(arch) == 0)
    {
        fgets(str, 8, arch);
        fgets(path, 250, arch);

        char tempStr[1024];
        snprintf(tempStr, sizeof tempStr, "%s%s", sysCom, path);

        removeHyphen(path);

        system(tempStr);

        fgets(nextFile, 256, arch);
        snprintf(tempStr, sizeof tempStr, "%s/%s/%s", dirUnzip, path, nextFile);

        FILE *fileRead = fopen(tempStr, "w+");
        if (fileRead == NULL)
        {
            printf("%s", "the file for recording did not open");
        }
        fgets(str, 10, arch);

        while (1)
        {
            fgets(str, 1024, arch);
            if (strcmp(str, brackets) == 0)
                break;

            fputs(str, fileRead);
        }
        chdir(dirUnzip);
    }
    fclose(arch);
}

int getFileSize(const char *file)
{
    int fileSize = 0;
    FILE *fd = fopen(file, "rb");
    if (fd == NULL)
    {
        fileSize = -1;
    }
    else
    {
        while (getc(fd) != EOF)
            fileSize++;
        fclose(fd);
    }
    return fileSize;
}

void removeHyphen(char *str)
{
    int sl = strlen(str) - 1;
    if ((str[sl]) == '\n')
        str[sl] = 0;
}