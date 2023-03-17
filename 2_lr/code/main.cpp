#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void runFerstTask(char *arg[])
{
    try
    {
        if (strcmp(arg[1], "ls") == 0 || strcmp(arg[1], "cat") == 0)
        {
            if (fork() == 0)
            {
                wait(0);
            }
            else
            {
                if (strcmp(arg[1], "ls") == 0)
                {
                    execlp("/bin/ls", arg[1], arg[2], (char *)NULL);
                }
                else
                {
                    execlp("/bin/cat", arg[1], arg[2], (char *)NULL);
                }
            }
        }
        else
        {
            throw "Такой команды не найдено";
        }
    }
    catch (const char *msg)
    {
        cout << msg << endl;
    }
}

pid_t child_pid;
pid_t parent_pid;

void killParent(int sig)
{
    kill(parent_pid, SIGINT);
}

void killChild(int sig)
{
    kill(child_pid, SIGINT);
}

void runSecondTask() {
    pid_t pid = fork();

    if (pid == 0)
    {
        cout << "Дочерний процесс\n";
        parent_pid = getppid();

        signal(SIGINT, killParent);

        pause();

        cout << "\nРодительский процесс убит";
    }
    else
    {
        cout << "execlp из родительского процесса " << endl;
        execlp("telegram-desktop", "telegram-desktop", (char *)NULL);
    }
}

int main(int argc, char *argv[])
{
    
    runSecondTask();

    return 0;
}