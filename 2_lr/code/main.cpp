#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stack>

using namespace std;

stack<pid_t> pids;

void runLsCatCommand(string comand, string options)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        try
        {
            string bin = "/bin/";
            bin += comand;
            execlp(bin.c_str(), comand.c_str(), options.c_str(), (char *)NULL);
            throw "Ошибка замещения текущего образа процесса новым образом процесса";
        }
        catch (const char *msg)
        {
            cout << msg << endl;
            exit(0);
        }
    }
}

void onC(int sig)
{
    cout << "kill process with pid = " << pids.top() << endl;
    kill(pids.top(), SIGTERM);
    pids.pop();
}

void execChildProcess(string options)
{
    signal(SIGINT, SIG_IGN);
    cout << "execlp из дочернего процесса pid = " << getpid() << endl;
    try
    {
        int e = execlp(options.c_str(), options.c_str(), (char *)NULL);
        throw "Ошибка замещения текущего образа процесса новым образом процесса";
    }
    catch (const char *msg)
    {
        cout << msg;
        exit(0);
    }
    exit(9);
}

void createAndRunChildProcess(string options)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execChildProcess(options);
    }
    else
    {
        pids.push(pid);
    }
}

int main(int argc, char *argv[])
{
    string command = "";
    string options = "";
    signal(SIGINT, onC);

    while (1)
    {
        cin >> command;
        if (command == "run")
        {
            cin >> options;
            createAndRunChildProcess(options);
        }
        else if (command == "ls" || command == "cat")
        {
            cin >> options;
            runLsCatCommand(command, options);
        }
        else if (command == "exit")
        {
            cout << "Завершение программы\n";
            return 1;
        }
    }
    return 0;
}