#include "Location.h"
#include "cd_spl.h"

directoryInfo *InitializeShell()
{
    directoryInfo *Info = (directoryInfo *)malloc(sizeof(directoryInfo));
    Info->currentPath = (char *)malloc(sizeof(char) * 1000);
    Info->HomePath = (char *)malloc(sizeof(char) * 1000);
    Info->prevPath = (char *)malloc(sizeof(char)*1000);
}

void update_state(directoryInfo *currInfo)
{
    getcwd(currInfo->currentPath, 1000);
}

char *Shell_Location(directoryInfo *currInfo, char *finalprompt)
{
    // Define a copy of the final prompt
    char *truly_final = (char *)malloc(sizeof(char) * 1000);
    strcat(truly_final, finalprompt);
    strcat(truly_final, currInfo->currentPath);

    return truly_final;
}

char *Obtain_Rel(directoryInfo *currInfo)
{
    char *Path = (char *)malloc(sizeof(char) * 1000);

    if (iscddotdot == 1)
    {
        strcpy(Path, currInfo->HomePath);
        return Path;
    }

    char temp[1];
    temp[0] = '~';
    strcpy(Path, temp);

    int i = 0;

    // Test if current path is the sub-folder of home
    for (i = 0; i < strlen(currInfo->HomePath); i++)
    {
        // Check if = or not
        if (currInfo->currentPath[i] != currInfo->HomePath[i])
        {
            strcpy(Path, currInfo->currentPath); // If Both Do Not Match, Then Return Absolute path
            return Path;
        }
    }

    // Obtain the relative Path
    char *temp_path = currInfo->currentPath + strlen(currInfo->HomePath);

    strcpy(Path + 1, temp_path); // Here this means that if Hello/Kesav is home directory and current directory is Hello/Kesav/Venna, then temp_path contains /Venna
    return Path;
}
