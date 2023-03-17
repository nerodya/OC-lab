#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void runFerstTask(string comand, string options)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        try
        {
            {
                string bin = "/bin/";
                bin += comand;

                if (execlp(bin.c_str(), comand.c_str(), options.c_str(), (char *)NULL) == -1)
                {
                    throw "Ошибка замещения текущего образа процесса новым образом процесса";
                }
            }
        }
        catch (const char *msg)
        {
            cout << msg << endl;
            exit(0);
        }
    }
}

pid_t parent_pid;

void killParent(int sig)
{
    kill(parent_pid, SIGINT);
}

void runSecondTask(string options)
{
    pid_t pid = fork();

    if (pid != 0)
    {
        cout << "Родительский процесс\n";
        parent_pid = getppid();
        signal(SIGINT, killParent);
        pause();
        cout << "\nДочерний процесс убит";
    }
    else
    {
        cout << "execlp из дочернего процесса " << endl;
        try
        {
            int e = execlp(options.c_str(), options.c_str(), (char *)NULL);
            if (e == -1)
            {
                throw "Ошибка замещения текущего образа процесса новым образом процесса";
            }
        }
        catch (const char *msg)
        {
            cout << msg;
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    string command = "";
    string options = "";

    while (1)
    {
        cin >> command;

        if (command == "run")
        {
            cin >> options;
            runSecondTask(options);
        }
        else if (command == "ls" || command == "cat")
        {
            cin >> options;
            runFerstTask(command, options);
        }
        else if (command == "exit")
        {
            cout << "Завершение программы\n";
            return 1;
        }
    }

    return 0;
}