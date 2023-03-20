#ifndef CD_H
#define CD_H

#include <stdlib.h>
#include <stdio.h>
#include "Location.h"

void cd(char **foreground_command, directoryInfo *currInfo, char *oldpwd);

#endif