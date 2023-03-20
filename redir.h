#ifndef REDIR_H
#define REDIR_H

#include <stdio.h>
#include "Location.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
#include "history.h"
#include "discover.h"
#include "general_command.h"
#include "pinfo.h"

void redirect(char **foreground_command, int number, char *echo_req, char *old_pwd, directoryInfo *currInfo, char *commands);

#endif