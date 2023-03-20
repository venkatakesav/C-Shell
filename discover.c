#include "discover.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int file_found = 0;

int isDirectory(char *directory)
{
    DIR *check = opendir(directory);
    if (check == NULL)
    {
        return -1;
    }
    return 1;
}

void ModifyRelative(char **directory, directoryInfo *currInfo)
{
    *directory = *directory + strlen(currInfo->HomePath) + 1;
}

int Obtain_Directory(char **foreground_command, int *record, int number, char *Directory)
{
    int x = -1;
    for (int i = 0; i < number; i++)
    {
        if (record[i] == 0)
        {
            x = i;
        }
    }

    if (x != -1)
    {
        strcpy(Directory, foreground_command[x]);
        return 1;
    }
    else
    {
        return -1;
    }
}

void Search_Dir(char *Path, int flag_d, int flag_f, directoryInfo *currInfo)
{
    // Check for Relative or absolute path -> if absolute convert to Relative with a .
    // For example is Kesav/New is the relative path
    // Then ./Kesav/New should be printed

    char **File_list = (char **)malloc(sizeof(char *) * 100);
    for (int i = 0; i < 100; i++)
    {
        File_list[i] = (char *)malloc(sizeof(char) * 1000);
    }

    struct dirent *DIR_INF;
    DIR *dir;

    if (strlen(Path) == 0)
    {
        // printf("Path is .!!!\n");
        strcpy(Path, ".");
        dir = opendir(Path);
    }
    else
    {
        dir = opendir(Path);
        if (dir == NULL)
        {
            // perror(Path);
        }
        else
        {
            // printf("Opening Successful!! -> %s\n", Path);
        }
    }

    if (dir == NULL)
    {
        return;
    }

    char *temp_path = (char *)malloc(sizeof(char) * 1000);

    while ((DIR_INF = readdir(dir)) != NULL)
    {

        if (DIR_INF->d_name[0] != '.')
        {
            if (flag_d == 0 && flag_f == 0 || flag_d == 1 && flag_f == 1)
            {
                printf("%s/%s\n", Path, DIR_INF->d_name);
                strcpy(temp_path, Path);
                strcat(temp_path, "/");
                strcat(temp_path, DIR_INF->d_name);
                Search_Dir(temp_path, flag_d, flag_f, currInfo);
            }
            else if (flag_d == 1)
            {
                strcpy(temp_path, Path);
                strcat(temp_path, "/");
                strcat(temp_path, DIR_INF->d_name);
                if (isDirectory(temp_path) == 1)
                {
                    printf("%s/%s\n", Path, DIR_INF->d_name);
                    strcpy(temp_path, Path);
                    strcat(temp_path, "/");
                    strcat(temp_path, DIR_INF->d_name);
                    Search_Dir(temp_path, flag_d, flag_f, currInfo);
                }
            }
            else if (flag_f == 1)
            {
                strcpy(temp_path, Path);
                strcat(temp_path, "/");
                strcat(temp_path, DIR_INF->d_name);
                if (isDirectory(temp_path) == -1)
                {
                    printf("%s/%s\n", Path, DIR_INF->d_name);
                }
                strcpy(temp_path, Path);
                strcat(temp_path, "/");
                strcat(temp_path, DIR_INF->d_name);
                Search_Dir(temp_path, flag_d, flag_f, currInfo);
            }
        }
    }
}

void Find_Dir(char *Path, directoryInfo *currInfo, char *file_name)
{
    // Check for Relative or absolute path -> if absolute convert to Relative with a .
    // For example is Kesav/New is the relative path
    // Then ./Kesav/New should be printed

    char *File_list = (char *)malloc(sizeof(char *) * 1000);
    strcpy(File_list, file_name);
    File_list = File_list + 1;
    File_list[strlen(File_list) - 1] = '\0';

    struct dirent *DIR_INF;
    DIR *dir;

    if (strlen(Path) == 0)
    {
        // printf("Path is .!!!\n");
        strcpy(Path, ".");
        dir = opendir(Path);
    }
    else
    {
        dir = opendir(Path);
        if (dir == NULL)
        {
            // perror(Path);
        }
        else
        {
            // printf("Opening Successful!! -> %s\n", Path);
        }
    }

    if (dir == NULL)
    {
        return;
    }

    char *temp_path = (char *)malloc(sizeof(char) * 1000);

    while ((DIR_INF = readdir(dir)) != NULL)
    {

        if (DIR_INF->d_name[0] != '.')
        {
            // printf("%s/%s\n", Path, DIR_INF->d_name);
            strcpy(temp_path, Path);
            strcat(temp_path, "/");
            strcat(temp_path, DIR_INF->d_name);
            if (strcmp(DIR_INF->d_name, File_list) == 0)
            {
                printf("File Found with Path -> %s\n", temp_path);
                file_found = 1;
                return;
            }
            Find_Dir(temp_path, currInfo, file_name);
        }
    }

    // printf("Not Found\n");
}

void discover_all(directoryInfo *currInfo, char **foreground_command, int number)
{
    int flag_d = 0;
    int flag_f = 0;

    int *Record_checked = (int *)malloc(sizeof(char) * number);
    Record_checked[0] = 1;

    for (int i = 1; i < number; i++)
    {
        // For Flags
        if (strcmp(foreground_command[i], "-d") == 0)
        {
            flag_d = 1;
            Record_checked[i] = 1;
        }
        if (strcmp(foreground_command[i], "-f") == 0)
        {
            flag_f = 1;
            Record_checked[i] = 1;
        }
    }

    char *target_directory = (char *)malloc(sizeof(char) * 1000);
    char *Path = (char *)malloc(sizeof(char) * 1000);

    // To find the Search file
    char *file_search = (char *)malloc(sizeof(char) * 1000);
    int isFile = 0;

    for (int i = 1; i < number; i++)
    {
        if (foreground_command[i][0] == '"')
        {
            strcpy(file_search, foreground_command[i]);
            isFile = 1;
            Record_checked[i] = 1;
            // printf("The file name is %s\n", foreground_command[i]);
        }
    }

    int flagisDirectory = 0;

    // To Find the directory
    for (int i = 1; i < number; i++)
    {
        if (strcmp(foreground_command[i], ".") == 0)
        {
            Record_checked[i] = 1;
            strcpy(target_directory, currInfo->currentPath);
            ModifyRelative(&target_directory, currInfo);
            // printf("The target directory is -> %s\n", target_directory);
            flagisDirectory = 1;
        }
        else if (strcmp(foreground_command[i], "..") == 0)
        {
            Record_checked[i] = 1;
            // Say Relative Path of the given file is something like
            // This, ~/venkata/kesav
            // Navigate to the topmost directory
            int x;
            for (int j = 0; j < strlen(currInfo->currentPath); j++)
            {
                if (foreground_command[i][j] == '/')
                {
                    x = j;
                }
            }

            strcpy(target_directory, foreground_command[i]);
            // printf("%s\n", target_directory);

            // ModifyRelative(&target_directory, currInfo);
            // printf("The target directory is -> %s\n", target_directory);

            flagisDirectory = 1;
        }
        else if (strcmp(foreground_command[i], "~") == 0)
        {
            Record_checked[i] = 1;
            strcpy(target_directory, currInfo->HomePath);
            flagisDirectory = 1;

            ModifyRelative(&target_directory, currInfo);
            // printf("The target directory is -> %s\n", target_directory);
        }
    }

    if (flagisDirectory == 0)
    {
        if (Obtain_Directory(foreground_command, Record_checked, number, target_directory) == -1)
        {
            strcpy(target_directory, currInfo->currentPath);
            ModifyRelative(&target_directory, currInfo);
            // printf("The target directory is -> %s\n", target_directory);
        }
        else
        {
            int isRelativePath = 0;
            int isAbsolutePath = 0;

            char *temp = malloc(sizeof(char) * 1000);

            // For Absolute Paths -> convert to Relative Paths
            if (Obtain_Directory(foreground_command, Record_checked, number, target_directory) != -1)
            {
                // printf("\n%s\n", target_directory);
                for (int i = 0; i < strlen(target_directory); i++)
                {
                    if (target_directory[0] == '~')
                    {
                        if (target_directory[1] == '/')
                        {
                            strcpy(temp, currInfo->HomePath);
                            strcat(temp, target_directory + 1);
                            isAbsolutePath = 1;
                        }
                    }
                }

                if (isAbsolutePath == 1)
                {
                    strcpy(target_directory, temp);
                    ModifyRelative(&target_directory, currInfo);
                }

                // printf("The target directory is -> %s\n", target_directory);
            }
        }
    }

    if (isFile != 1)
    {
        Search_Dir(target_directory, flag_d, flag_f, currInfo);
    }
    else
    {
        // printf("File is Present !!\n");
        Find_Dir(target_directory, currInfo, file_search);
        if (file_found == 0)
        {
            printf("File Not Found");
        }
        file_found = 0;
    }

    // printf("The Target directory is: %s\n", target_directory);
}