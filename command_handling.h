#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H

#include <stdio.h>
#include "Location.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "history.h"
#include "discover.h"
#include "general_command.h"
#include "pinfo.h"
#include "redir.h"

void command_handling(char **foreground_command, int number, char *echo_req, char *old_pwd, directoryInfo *currInfo, char **backgroundProcesses, char *commands, int num_parts, int num_background);

#endif