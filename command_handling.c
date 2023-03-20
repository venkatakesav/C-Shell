#include "command_handling.h"

void command_handling(char **foreground_command, int number, char *echo_req, char *old_pwd, directoryInfo *currInfo, char **backgroundProcesses, char *commands, int num_parts, int num_background)
{
    int i = 0;
    while (echo_req[i] != '\0')
    {
        if (echo_req[i] == '>' || echo_req[i] == '<')
        {
            redirect(foreground_command, number, echo_req, old_pwd, currInfo, commands);
            // printf("Returning....\n");
            return;
        }
        i++;
    }

    i = 0;
    // printf("Echo Req is : %s", echo_req);

    int isExecuted = 0;
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
        echo(foreground_command, number, echo_req);
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
    else if (strcmp(foreground_command[0], "jobs") == 0)
    {
        jobs();
    }
    else if (strcmp(foreground_command[0], "exit") == 0)
    {
        exit(1);
    }
    else if (strcmp(foreground_command[0], "sig") == 0)
    {
        sig(foreground_command);
    }
    else if (strcmp(foreground_command[0], "fg") == 0)
    {
        Shiftforeground(foreground_command);
    }
    else if (strcmp(foreground_command[0], "bg") == 0)
    {
        Shiftbackground(foreground_command);
    }
    else
    {
        executeGeneralOperations(foreground_command, backgroundProcesses, currInfo, commands, num_parts, echo_req, num_background);
        isExecuted = 1;
    }

    if (num_background >= 2 && isExecuted != 1)
    {
        executeGeneralOperations(foreground_command, backgroundProcesses, currInfo, commands, num_parts, echo_req, num_background);
    }
}