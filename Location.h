#ifndef LOCATION_H
#define LOCATION_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct directoryInfo
{
    char *currentPath;
    char *HomePath;
    char *prevPath;
} directoryInfo;

directoryInfo *InitializeShell();
void update_state(directoryInfo *currInfo);
char *Shell_Location(directoryInfo *currInfo, char *finalprompt);
char *Obtain_Rel(directoryInfo *currInfo);

#endif