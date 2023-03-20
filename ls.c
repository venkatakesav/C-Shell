#include "ls.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define RESET_COLOR "\e[m"
#define MAKE_GREEN "\e[32m"
#define MAKE_BLUE "\e[36m"

char *Username(uid_t userId)
{
    struct passwd *pws = (struct passwd *)malloc(sizeof(struct passwd));
    pws = getpwuid(userId);
    if (!pws)
    {
        char *tempStr = (char *)malloc(sizeof(char) * 100);
        sprintf(tempStr, "%d", userId);
        return tempStr;
    }
    return pws->pw_name;
}

char *GroupName(gid_t groupId)
{
    struct group *grp = (struct group *)malloc(sizeof(struct group));
    grp = getgrgid(groupId);
    if (!grp)
    {
        char *tempStr = (char *)malloc(sizeof(char) * 100);
        sprintf(tempStr, "%d", groupId);
        return tempStr;
    }
    return grp->gr_name;
}

void change_flags(bool *isAPresent, bool *isLPresent, int number_of_arguments, char **foreground_command)
{
    for (int i = 1; i < number_of_arguments; i++)
    {
        if (foreground_command[i][0] == '-')
        {
            if (strcmp(foreground_command[i], "-a") == 0 || strcmp(foreground_command[i], "a") == 0)
            {
                *isAPresent = true;
                // printf("A is Present\n");
            }
            else if (strcmp(foreground_command[i], "-l") == 0 || strcmp(foreground_command[i], "l") == 0)
            {
                *isLPresent = true;
                // printf("L is Present\n");
            }
            else if (strcmp(foreground_command[i], "-al") == 0 || strcmp(foreground_command[1], "-la") == 0)
            {
                *isAPresent = true;
                *isLPresent = true;
                // printf("A and L are Present\n");
            }
            else
            {
                printf("Invalid Flags\n");
            }
        }
    }
}

void print_directory(struct dirent *File_stream, bool isAPresent, bool isLPresent, directoryInfo *currInfo)
{
    if (isLPresent == false)
    {
        // printf("%s\n", File_stream->d_name);

        struct stat buffer;
        char tempo[1000];
        char *path_type = getcwd(tempo, 1000);
        strcat(path_type, "/");
        strcat(path_type, File_stream->d_name);
        if (stat(path_type, &buffer) != -1)
        {
            if (S_ISDIR(buffer.st_mode))
            {
                printf(MAKE_BLUE "%s\n" RESET_COLOR, File_stream->d_name);
            }
            else if (buffer.st_mode & S_IXUSR)
            {
                printf(MAKE_GREEN "%s\n" RESET_COLOR, File_stream->d_name);
            }
            else
            {
                printf("%s\n", File_stream->d_name);
            }
        }
    }

    else
    {
        struct stat statObject;
        stat(File_stream->d_name, &statObject);

        // char First_Part[1000] = {"\0"};
        // if(statObject.st_mode){ //If It is directory then put 'd' as first letter
        //     printf("%d", stat);
        // }

        char *First_Part = calloc(1000, sizeof(char));

        // printf("%o\n", statObject.st_mode);

        // strcpy(First_Part, "");

        if (S_ISDIR(statObject.st_mode) != 0)
        { // Check the operation -> returns non zero if directory
            strcpy(First_Part, "d");
        }
        else
        {
            strcpy(First_Part, "-");
        }

        // printf("%o\n", S_IXUSR);

        // Here Logic is if you and the values individually in the octal system you get 0 then it does not
        // Have that permission
        // For Instance
        // In Octal statObject.st_mode == 100664
        // S_IXUSR == 100 -> on anding '0' -> No execute permission for user

        if (statObject.st_mode & S_IRUSR == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "r");
        }

        if (statObject.st_mode & S_IWUSR == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "w");
        }

        if (statObject.st_mode & S_IXUSR == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "x");
        }

        if (statObject.st_mode & S_IRGRP == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "r");
        }

        if (statObject.st_mode & S_IWGRP == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "w");
        }

        if (statObject.st_mode & S_IXGRP == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "x");
        }

        if (statObject.st_mode & S_IROTH == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "r");
        }

        if (statObject.st_mode & S_IWOTH == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "w");
        }

        if (statObject.st_mode & S_IXOTH == 0)
        {
            strcat(First_Part, "-");
        }
        else
        {
            strcat(First_Part, "x");
        }

        char *time_modified = ctime(&statObject.st_mtim); // Time Modified is changed

        time_modified = time_modified + 4; // Remove the name of the day
        time_modified[strlen(time_modified) - 1] = ' ';

        // First_Part = First_Part + 1;

        // printf("%s", time_modified);

        printf("%s %d %s %s %s ", First_Part, (int)statObject.st_nlink, Username(statObject.st_uid), GroupName(statObject.st_gid), time_modified);
        struct stat buffer;

        char tempo[1000];
        char *path_type = getcwd(tempo, 1000);
        strcat(path_type, "/");
        strcat(path_type, File_stream->d_name);

        if (stat(path_type, &buffer) != -1)
        {
            if (S_ISDIR(buffer.st_mode))
            {
                printf(MAKE_BLUE "%s\n" RESET_COLOR, File_stream->d_name);
            }
            else if (buffer.st_mode & S_IXUSR)
            {
                printf(MAKE_GREEN "%s\n" RESET_COLOR, File_stream->d_name);
            }
            else
            {
                printf("%s\n", File_stream->d_name);
            }
        }
    }
}

void print_fileStructure(char **foreground_command, directoryInfo *currInfo, bool isAPresent, bool isLPresent, bool isDirectory, int i)
{
    char *Directory = (char *)malloc(sizeof(char) * 1000);
    if (isDirectory == false && i == -1)
    {
        DIR *directory_input = opendir(currInfo->currentPath);

        struct dirent *File_stream = readdir(directory_input); // This generates the stream of files
        while (File_stream != NULL)
        {

            if (isAPresent == false)
            {
                if (File_stream->d_name[0] == '.')
                {
                    File_stream = readdir(directory_input);
                    continue;
                }
                else
                {
                    print_directory(File_stream, isAPresent, isLPresent, currInfo);
                    File_stream = readdir(directory_input);
                }
            }
            else
            {
                print_directory(File_stream, isAPresent, isLPresent, currInfo);
                File_stream = readdir(directory_input);
            }
        }
    }
    else if (isDirectory == true)
    {
        // Check whether given path is relative or absolute
        if (strcmp(foreground_command[i], "~") == 0)
        {
            // This means implement the home directory
            DIR *directory_input = opendir(currInfo->HomePath);

            struct dirent *File_stream = readdir(directory_input); // This generates the stream of files
            while (File_stream != NULL)
            {
                if (isAPresent == false)
                {
                    if (File_stream->d_name[0] == '.')
                    {
                        File_stream = readdir(directory_input);
                        continue;
                    }
                    else
                    {
                        print_directory(File_stream, isAPresent, isLPresent, currInfo);
                        File_stream = readdir(directory_input);
                    }
                }
                else
                {
                    print_directory(File_stream, isAPresent, isLPresent, currInfo);
                    File_stream = readdir(directory_input);
                }
            }
        }
        else
        {
            // Find out whether Absolute path or relative path
            char *temp = malloc(sizeof(char) * 1000);

            int isAbsolutePath = 0;
            for (int j = 0; j < strlen(foreground_command[i]); j++)
            {
                if (foreground_command[i][0] == '~')
                {
                    if (foreground_command[i][1] == '/')
                    {
                        strcpy(temp, currInfo->HomePath);
                        strcat(temp, foreground_command[i] + 1);
                        isAbsolutePath = 1;
                    }
                }
            }

            DIR *directory_input;
            if (isAbsolutePath != 1)
            {
                // Either Absolute Path or relative Path
                directory_input = opendir(foreground_command[i]);
            }
            else
            {
                directory_input = opendir(temp);
            }

            if (directory_input == NULL)
            {
                perror(foreground_command[i]);
                return;
            }

            struct dirent *File_stream = readdir(directory_input); // This generates the stream of files
            while (File_stream != NULL)
            {
                if (isAPresent == false)
                {
                    if (File_stream->d_name[0] == '.')
                    {
                        File_stream = readdir(directory_input);
                        continue;
                    }
                    else
                    {
                        print_directory(File_stream, isAPresent, isLPresent, currInfo);
                        File_stream = readdir(directory_input);
                    }
                }
                else
                {
                    print_directory(File_stream, isAPresent, isLPresent, currInfo);
                    File_stream = readdir(directory_input);
                }
            }
        }
    }
    else
    {
        return;
    }
}

int ls(char **foreground_command, directoryInfo *currInfo, int number_of_arguments)
{
    bool isAPresent = false;
    bool isLPresent = false;

    // The current Directory is in currInfo->currentPath

    // Here Foreground_Command is containing foreground_command[0] = "ls", if the flags exist
    // Here there is a small problem, we remove spaces between ls -a l so this technically should be an invalid command
    // But If We just calculate the number of -a and -l flags then we obtain that it is valid, to counter this count
    // The number of -'s first

    // The Point with these flags is that they can be present in any part that is there can be a directory
    // coming first and it wouldn't matter

    // For example ls directory -l -a

    change_flags(&isAPresent, &isLPresent, number_of_arguments, foreground_command);

    bool contains_directory = false;

    // First Figure out if the command contains a directory or not
    for (int i = 1; i < number_of_arguments; i++)
    {
        if (strcmp(foreground_command[i], "-a") == 0 || strcmp(foreground_command[i], "-l") == 0 || strcmp(foreground_command[i], "-al") == 0 || strcmp(foreground_command[i], "-la") == 0)
        {
            continue;
        }
        else
        {
            contains_directory = true;
            print_fileStructure(foreground_command, currInfo, isAPresent, isLPresent, contains_directory, i);
        }
    }

    if (contains_directory == false)
    {
        print_fileStructure(foreground_command, currInfo, isAPresent, isLPresent, contains_directory, -1);
    }
}