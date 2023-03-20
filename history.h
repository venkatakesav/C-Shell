#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "Location.h"

#ifndef HISTORY_H
#define HISTORY_H

void writeToHistory(directoryInfo *currInfo, char *command_line, int *num_commands, char** Read_Array);
void printHistory(directoryInfo *currInfo);

#endif