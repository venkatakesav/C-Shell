#include "redir.h"

void redirect(char **foreground_command, int number, char *echo_req, char *old_pwd, directoryInfo *currInfo, char *commands)
{
    int isWrite = 0;
    int isAppend = 0;
    int isInputFile = 0;
    int fd, fd2;

    int std_output = 1;
    int std_input = 0;

    int curr_input, curr_output;

    char Ouput_File[100];
    char Input_File[100];

    for (int i = 0; i < number - 1; i++)
    {
        if (foreground_command[i] != NULL && strcmp(foreground_command[i], ">>") == 0)
        {
            isAppend = 1;
            foreground_command[i] = NULL;
            strcpy(Ouput_File, foreground_command[i + 1]);
        }
        if (foreground_command[i] != NULL && strcmp(foreground_command[i], ">") == 0)
        {
            isWrite = 1;
            foreground_command[i] = NULL;
            strcpy(Ouput_File, foreground_command[i + 1]);
        }
        if (foreground_command[i] != NULL && strcmp(foreground_command[i], "<") == 0)
        {
            isInputFile = 1;
            foreground_command[i] = NULL;
            strcpy(Input_File, foreground_command[i + 1]);
        }
    }

    if (isAppend == 1)
    {
        if (isWrite == 1)
        {
            perror("Can't Write and Append at the Same time");
        }
    }

    if (isWrite == 1)
    {
        std_output = dup(1);
        fd = open(Ouput_File, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error in the output: ");
            return;
        }
        dup2(fd, 1);
    }
    if (isAppend == 1)
    {
        std_output = dup(1);
        fd = open(Ouput_File, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error in the output: ");
            return;
        }
        dup2(fd, 1);
    }
    if (isInputFile == 1)
    {
        std_input = dup(0);
        printf("Input File is -> %s\n", Input_File);
        fd2 = open(Input_File, O_RDONLY, 0644);
        if (fd2 < 0)
        {
            perror("Error in the output: ");
            return;
        }
        dup2(fd2, 0);
    }

    // printf("Foreground Command[0] ->%s\n", foreground_command[0]);

    int pid = 0;
    if (strcmp(foreground_command[0], "cd") == 0)
    {
        cd(foreground_command, currInfo, old_pwd);
    }
    else if (strcmp(foreground_command[0], "pwd") == 0)
    {
        char *cwd = (char *)malloc(sizeof(char) * 1000);
        getcwd(cwd, 1000);
        printf("%s\n", cwd);
    }
    else if (strcmp(foreground_command[0], "echo") == 0)
    {
        char echo_reqt[1000];
        strcpy(echo_reqt, echo_req);
        if (isWrite == 1)
        {
            strtok(echo_reqt, ">");
        }
        if (isAppend == 1)
        {
            strtok(echo_reqt, ">>");
        }
        if (isInputFile == 1)
        {
            strtok(echo_reqt, "<");
        }
        echo(foreground_command, number, echo_reqt);
    }
    else if (strcmp(foreground_command[0], "ls") == 0)
    {
        ls(foreground_command, currInfo, number);
    }
    else if (strcmp(foreground_command[0], "history") == 0)
    {
        printHistory(currInfo);
    }
    else if (strcmp(foreground_command[0], "discover") == 0)
    {
        discover_all(currInfo, foreground_command, number);
    }
    else if (strcmp(foreground_command[0], "pinfo") == 0)
    {
        pinfo(currInfo, foreground_command);
    }
    else if (strcmp(foreground_command[0], "exit") == 0)
    {
        exit(1);
    }
    else
    {
        char reqt[1000];
        strcpy(reqt, commands);
        if (isWrite == 1)
        {
            strtok(reqt, ">");
        }
        if (isAppend == 1)
        {
            strtok(reqt, ">>");
        }
        if (isInputFile == 1)
        {
            strtok(reqt, "<");
        }
        executeGeneralOperations(foreground_command, NULL, currInfo, reqt, 0, NULL, 0);
    }

    // Restore the standard inputs/outputs
    if (isWrite == 1 || isAppend == 1)
    {
        dup2(std_output, 1);
    }
    if (isInputFile)
    {
        dup2(std_input, 0);
    }
}