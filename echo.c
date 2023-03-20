#include "echo.h"

void echo(char **foreground_command, int number, char *echo_req)
{
    char *delimiters = (char *)malloc(sizeof(char) * 100);
    delimiters = " \t\n";

    char *copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(copy, echo_req); // To Preserve the echo_req, that is the original command

    // Check if the last part
    //  printf("Echo -> Requirement is : %s\n", echo_req);

    // printf("Last letter is - %c\n", echo_req[strlen(echo_req)-1]);

    if (echo_req[strlen(echo_req) - 1] == '\n')
    {
        char *part = strtok(copy, delimiters);
        part = strtok(NULL, delimiters);

        printf("%s", echo_req + (part - copy));
    }
    else
    {
        char *part = strtok(copy, delimiters);
        part = strtok(NULL, delimiters);

        printf("%s\n", echo_req + (part - copy));
    }

    // Remove the part that has all the delimiters
    // char *part = strtok(copy, delimiters);
    // part = strtok(NULL, delimiters);

    // printf("%s", echo_req + (part - copy));
}