#include "Location.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#ifndef LS_H
#define LS_H

int ls(char **foreground_command, directoryInfo* currInfo, int number_of_arguments);

#endif