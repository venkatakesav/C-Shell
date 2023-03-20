#include "cd.h"
#include "cd_spl.h"

void cd(char **foreground_command, directoryInfo *currInfo, char *oldpwd)
{
    if(foreground_command[1] == NULL){
        return;
    }

    if (foreground_command[1][0] == '.')
    {
        if (foreground_command[1][1] == '.')
        {
            // Consider the relative path of the directory ->
            if (strcmp(Obtain_Rel(currInfo), "~") == 0)
            {
                printf("%s\n", currInfo->HomePath);
            } 
            else{
                //Say Relative Path of the given file is something like 
                //This, ~/venkata/kesav
                //Navigate to the topmost directory
                char *ParentDir = (char *)malloc(sizeof(char)*1000);
                char *RelParentDir = (char *)malloc(sizeof(char)*1000);
                char *final = (char *)malloc(sizeof(char)*1000);
                int i; 
                for(i = strlen(Obtain_Rel(currInfo)); Obtain_Rel(currInfo)[i] != '/'; i--);
                strncpy(ParentDir, Obtain_Rel(currInfo), i);
                // printf("%s\n", ParentDir);
                if(strcmp(ParentDir, "~") == 0){
                    chdir(currInfo->HomePath);
                }
                else{
                    strcpy(RelParentDir, ParentDir+1);
                    strcpy(final, currInfo->HomePath);
                    strcat(final, RelParentDir);


                    // printf("%s\n", final);
                    int x = chdir(final);
                    if(x < 0){perror(ParentDir + 2);}
                }
                // printf("%d", i);
            }
        }
    }
    else if (strcmp(foreground_command[1], "-") == 0)
    {
        int x = chdir(currInfo->prevPath);
        if(x < 0){printf("Illegal Command\n");}
        printf("%s\n", currInfo->prevPath);
    }
    else if (foreground_command[1][0] == '~')
    {
        char *directory = (char *)malloc(sizeof(char) * 1000);
        strcpy(directory, currInfo->HomePath);
        if (foreground_command[1][1] == '/')
        {
            strcat(directory, foreground_command[1] + 1);
            int x = chdir(directory);
            if(x < 0){perror(directory);}
        }
        else
        {
            int x = chdir(currInfo->HomePath);
            if(x < 0){perror(foreground_command[1]);}
        }
    }
    else
    {
        int x = chdir(foreground_command[1]);
        if(x < 0){
            perror(foreground_command[1]);
        }
    }

    strcpy(currInfo->prevPath, currInfo->currentPath);
    update_state(currInfo);
    return;
}