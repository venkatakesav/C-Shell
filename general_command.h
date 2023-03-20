#ifndef GENERAL_H
#define GENERAL_H
#include "Location.h"

int executeGeneralOperations(char **foreground_processes, char **background_processes, directoryInfo *currInfo, char *commands, int num_parts, char *rawCommand, int num_background);
void jobs();
void sigtstp_handler(int signum);
void sig(char *C[]);
void Shiftforeground(char *Command[]);
void Shiftbackground(char *Command[]);
void sigint_handler(int signum);
#endif