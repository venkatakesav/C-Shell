#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "general_command.h"

char *background[10000000] = {NULL};
pid_t Pid[100] = {0};
pid_t pid, pi;
int tot_jobs = 0;
int isBackground = 0;

void sigtstp_handler(int signum)
{
    if (isBackground == 0)
    {
        int frt = fork();
        if (frt == 0)
        {

            kill(pi, SIGSTOP);
            exit(0);
        }
        else
        {
            printf("[%d] %d\n", tot_jobs, pi);
            Pid[tot_jobs++] = pi;
            tot_jobs--;
            Pid[tot_jobs] = 0;
        }
    }
}

void sigint_handler(int signum){
    if(isBackground == 0){
        kill(pi, 9);
        printf("\n");
    }
}

void sig(char *Command[])
{
    int abc, def;
    int i = 0;
    int flag = 0;
    int a = atoi(Command[1]) + 1;
    int b = atoi(Command[2]);
    while (flag == 0)
    {
        abc = 0;
        if (i == a)
        {
            flag = 1;
            abc = Pid[i];
        }
        i++;
    }
    if (abc != 0)
    {
        def = kill(abc, b);
    }
}
void Shiftforeground(char *Command[])
{
     int dfs = 0;
    int i = 0;
    int a = atoi(Command[1]);
    int amma;
    while (Pid[i] != 0)
    {
        if (i + 1 == a)
        {
            dfs = Pid[i];
            break;
        }
        i++;
    }
    if (dfs != 0)
    {
        printf("Bought [%d] %s  to foreground\n", dfs, background[dfs]);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(0, getpgid(dfs));
        tcsetpgrp(0, getpid());
        kill(dfs, SIGCONT);
        waitpid(dfs, &amma, WUNTRACED);
        fflush(stdout);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        if (WIFSTOPPED(amma))
        {
            printf("[%d] %s  Stopped\n", dfs, background[dfs]);
        }
    }
}
void Shiftbackground(char *Command[])
{
    int abc, def;
    int i = 0;
    int flag = 0;
    int x;
    int a = atoi(Command[1]) + 1;
    while (flag == 0)
    {
        abc = 0;
        if (i == a)
        {
            flag = 1;
            abc = Pid[i];
        }
        i++;
    }
    if (abc != 0)
    {
        kill(abc, 18);
    }
}
void jobs()
{
    // printf("F\n");
    for (int i = 0; Pid[i] > 0; i++)
    {
        // printf("%s\n", background[Pid[i]]);
        // printf("F\n");
        char *processStatFile = (char *)malloc(sizeof(char) * 1000);
        char *garbageString = (char *)malloc(sizeof(char) * 1000);
        char *stat = (char *)malloc(10);
        char *runOrStop = (char *)malloc(1000);
        sprintf(processStatFile, "/proc/%d/stat", Pid[i]);
        FILE *fileObject = fopen(processStatFile, "r");
        if (!fileObject)
            continue;
        fscanf(fileObject, "%s", garbageString);
        fscanf(fileObject, "%s", garbageString);

        fscanf(fileObject, "%s", stat);

        strcpy(runOrStop, strcmp(stat, "T") == 0 ? "Stopped" : "Running");
        // printf("%s\n", background[Pid[i]]);
        dprintf(1, "[%d] %s %s [%d]\n", i, runOrStop, background[Pid[i]], Pid[i]);
        // printf("Reached!!!");
    }

    return;
}

void handler()
{
    pid_t pid;
    int stat;
    pid = waitpid(-1, &stat, WNOHANG);
    if (pid <= 0)
    {
        return;
    }
    if (stat != 0)
        fprintf(stderr, "The process %s with pid = %d, failed\n", background[pid], pid);
    else
        fprintf(stderr, "The process %s with pid = %d exited normally\n", background[pid], pid);
}

int executeGeneralOperations(char **foreground_processes, char **background_processes, directoryInfo *currInfo, char *commands, int num_parts, char *raw_command, int num_background)
{
    isBackground = 0;
    char delimiter_set_1[4] = " \t\n";
    char delimiter_set_2[3] = "\n&";

    // printf("The Raw Command is: %s\n", commands);

    if (commands[strlen(commands) - 2] == '&') // If the Last command is containing & all are background only
        isBackground = 1;

    // printf("The Raw Command is: %s\n", commands);

    char *tok, *b[100] = {NULL};

    // printf("The Raw Command is: %s\n", commands);

    tok = strtok(commands, delimiter_set_2);
    b[0] = tok;
    int i = 1;
    while (tok != NULL)
    {
        tok = strtok(NULL, delimiter_set_2);
        if (tok != NULL)
            b[i++] = tok;
    }

    // printf("The Raw Command is: %s\n", commands);

    int j = 0;
    while (b[j] != NULL)
    {

        char *token;
        token = strtok(b[j], delimiter_set_1);
        int z = 0;
        char *a[100] = {"\0"};
        a[z++] = token;
        while (token != NULL)
        {
            token = strtok(NULL, delimiter_set_1);
            if (token != NULL)
                a[z++] = token;
        }
        // printf("The Raw Command is: %s\n", commands);
        signal(SIGCHLD, handler);
        // printf("%s\n", a[2]);
        pid = fork();
        if (pid == 0)
        {
            setpgid(0, 0);
            if (execvp(a[0], a) < 0)
            {
                perror("Command Doesn't exist");
                exit(1);
            }
            exit(0);
        }
        else
        {
            Pid[tot_jobs] = pid;
            background[pid] = a[0];
            tot_jobs++;
            j++;
            i--;
            if (i == 0 && isBackground == 0)
                pi = pid;
            else
            {
                printf("[%d] %d\n", j, pid);
            }
        }
    }
    if (isBackground == 0)
    {
        waitpid(pi, NULL, WUNTRACED);
    }
}
