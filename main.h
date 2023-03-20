#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include "Location.h"


int dealingwithBackground(char *input, char **commands);
int dealingwithSpaces(char *input, char **commands);
// A function to split the strings wherever \n and \t are involved
int dealingwithSemiColons(char *input, char **commands);


#endif