#include "history.h"
int no_lines(directoryInfo *currInfo);

void printHistory(directoryInfo *currInfo)
{
    char *temp = (char *)malloc(sizeof(char) * 100);
    strcpy(temp, currInfo->HomePath);
    strcat(temp, "/History");

    int history = open(temp, O_RDWR, S_IRUSR);

    if (no_lines(currInfo) <= 10)
    {
        // printf("Number of lines is less than or equal to 10\n");
        char *buffer = (char *)malloc(sizeof(char));
        while (read(history, buffer, 1) != 0)
        {
            printf("%s", buffer);
        }
    }
    else if (no_lines(currInfo) > 10 && no_lines(currInfo) < 20)
    {
        // printf("Number of lines is greater than 10 and less than 20\n");
        char *buffer = (char *)malloc(sizeof(char));
        int limit = no_lines(currInfo) - 10;
        int no_lines = 0;
        while (read(history, buffer, 1) != 0)
        {
            if (no_lines >= limit)
            {
                printf("%s", buffer);
            }
            if (strcmp(buffer, "\n") == 0)
            {
                no_lines++;
            }
        }
    }
    else
    {
        // printf("Number of lines is greater than or equal to 20\n");
        char *buffer = (char *)malloc(sizeof(char));
        int no_lines = 0;
        while (read(history, buffer, 1) != 0)
        {
            if (no_lines >= 10)
            {
                printf("%s", buffer);
            }
            if (strcmp(buffer, "\n") == 0)
            {
                no_lines++;
            }
        }
    }
}

int no_lines(directoryInfo *currInfo)
{
    char *temp = (char *)malloc(sizeof(char) * 100);
    strcpy(temp, currInfo->HomePath);
    strcat(temp, "/History");

    int no_lines = 0;

    int history = open(temp, O_RDWR, S_IRUSR);

    char *buffer = (char *)malloc(sizeof(char));
    while (read(history, buffer, 1) != 0)
    {
        if (strcmp(buffer, "\n") == 0)
        {
            no_lines++;
        }
    }

    return no_lines;
}

void writeToHistory(directoryInfo *currInfo, char *command_line, int *num_commands, char **Read_Array)
{
    // Here if there is no file called history then, define one
    char *temp = (char *)malloc(sizeof(char) * 100);
    char *temp_final = (char *)malloc(sizeof(char) * 100);

    strcpy(temp, currInfo->HomePath);
    strcpy(temp_final, temp);

    strcat(temp, "/History");
    strcat(temp_final, "/History_final");

    int history_file = open(temp, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (no_lines(currInfo) < 20)
    {
        lseek(history_file, 0, SEEK_END);
        write(history_file, command_line, strlen(command_line));

        // printf("num_commands->%d\n", *num_commands);
    }
    else
    {
        char *buffer_final = (char *)malloc(sizeof(char) * 1000);

        read(history_file, buffer_final, 1000);

        int num_parts = 0;
        char delimiters[] = "\n";
        // Use a copy so as to not modify the original input
        char *copy = (char *)malloc(sizeof(char) * 1000);
        strcpy(copy, buffer_final);

        char *part = strtok(copy, delimiters);

        for (int i = 0; i < 20; i++)
        {
            strcpy(Read_Array[i], part);
            strcat(Read_Array[i], "\n");
            part = strtok(NULL, delimiters);
            num_parts = i + 1;
        }

        for (int i = 0; i < 19; i++)
        {
            strcpy(Read_Array[i], Read_Array[i + 1]);
        }
        strcpy(Read_Array[19], command_line);

        int history_file_final = open(temp_final, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        for (int i = 0; i < 20; i++)
        {
            lseek(history_file_final, 0, SEEK_END);
            write(history_file_final, Read_Array[i], strlen(Read_Array[i]));
        }

        rename(temp_final, temp);
    }

    close(history_file);
}