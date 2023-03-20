#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

void pinfo(directoryInfo *currInfo, char **foreground_process)
{

    long long int memory;
    long long int first, second;

    int pid;
    char c;

    char *pid_ifgiven = malloc(sizeof(char) * 1000);
    pid_ifgiven = foreground_process[1];

    if (pid_ifgiven == NULL)
    {
        FILE *file = fopen("/proc/self/stat", "r");
        fscanf(file, "%d %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lld",
               &pid, &c, &first, &second, &memory);
        printf("pid : %d\n", pid);
        if (first == second)
            printf("process status : %c+\n", c);
        else
            printf("process status : %c\n", c);
        printf("memory : %lld\n", memory);
        // file = fopen("/proc/self/exe", "r");
        char dir[1000] = {'\0'};
        int len = readlink("/proc/self/exe", dir, 999);
        int l = strlen(currInfo->HomePath);
        if (strlen(dir) < l)
            printf("Executable path: %s\n", dir);
        else
            printf("Executable path: ~%s\n", &dir[l]);
    }

    else
    {
        char *temp_path = malloc(sizeof(char) * 1000);
        strcpy(temp_path, "/proc/");
        strcat(temp_path, pid_ifgiven);
        strcat(temp_path, "/stat");
        // printf("%s\n", temp_path);
        FILE *file = fopen(temp_path, "r");
        fscanf(file, "%d %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lld",
               &pid, &c, &first, &second, &memory);
        printf("pid : %d\n", pid);
        if (first == second)
            printf("process status : %c+\n", c);
        else
            printf("process status : %c\n", c);
        printf("memory : %lld\n", memory);
        // file = fopen("/proc/self/exe", "r");
        char dir[1000] = {'\0'};
        int len = readlink("/proc/self/exe", dir, 999);
        int l = strlen(currInfo->HomePath);
        if (strlen(dir) < l)
            printf("Executable path: %s\n", dir);
        else
            printf("Executable path: ~%s\n", &dir[l]);
    }
}
