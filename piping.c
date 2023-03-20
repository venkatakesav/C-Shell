#include "piping.h"
#include "command_handling.h"

int splitStringOnSpaces(char *string, char *parsedArray[])
{
    // This parses the string on spaces and returns the number of things it parsed it to.
    char delim[] = " \t\n";
    char *stringCopy = (char *)malloc(sizeof(char) * 1000);
    strcpy(stringCopy, string);
    char *part = strtok(stringCopy, delim);
    int numParts = 0;
    for (int i = 0; part; i++)
    {
        parsedArray[i] = part;
        part = strtok(NULL, delim);
        numParts = i + 1;
    }
    return numParts;
}

void piping(char **piped_commands, int num_pipes, directoryInfo *Curr_Info, char *old_pwd)
{
    // for(int i = 0; i < num_pipes; i++){
    //     printf("The %dth piped command is -> %s\n", i, piped_commands[i]);
    // }
    char *commandParts[1000] = {'\0'};
    int standard_input = dup(0), standard_output = dup(1);
    int pipeObject[2];
    if (pipe(pipeObject) < 0)
    {
        perror(" Couldn't Create pipe. :( ");
        return;
    }
    if (dup2(pipeObject[1], 1) != 1)
        perror("Gadbad"); // writer here
    close(pipeObject[1]);
    char *piped_command_copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(piped_command_copy, piped_commands[0]);
    int numParts = splitStringOnSpaces(piped_commands[0], commandParts);
    command_handling(commandParts, numParts, piped_command_copy, old_pwd, Curr_Info, NULL, piped_command_copy, 0, 0);
    dup2(pipeObject[0], 0); // reader here
    close(pipeObject[0]);

    for (int i = 1; i < num_pipes - 1; i++)
    {
        if (pipe(pipeObject) < 0)
        {
            perror(" Couldn't Create pipe. :( ");
            return;
        }
        dup2(pipeObject[1], 1);
        close(pipeObject[1]);
        strcpy(piped_command_copy, piped_commands[i]);
        numParts = splitStringOnSpaces(piped_commands[i], commandParts);
        command_handling(commandParts, numParts, piped_command_copy, old_pwd, Curr_Info, NULL, piped_command_copy, 0, 0);
        dup2(pipeObject[0], 0);
        close(pipeObject[0]);
    }

    dup2(standard_output, 1);
    int stat;
    strcpy(piped_command_copy, piped_commands[num_pipes - 1]);
    numParts = splitStringOnSpaces(piped_commands[num_pipes - 1], commandParts);
    command_handling(commandParts, numParts, piped_command_copy, old_pwd, Curr_Info, NULL, piped_command_copy, 0, 0);
    dup2(standard_input, 0);

    return;
}
