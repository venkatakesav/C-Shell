#include "Location.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#ifndef DISCOVER_H
#define DISCOVER_H

int Obtain_Directory(char **foreground_command, int *record, int number, char *Directory);
void discover_all(directoryInfo *currInfo, char **foreground_command, int number);

#endif