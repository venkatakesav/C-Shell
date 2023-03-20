#include "main.h"
#include "cd.h"
#include "echo.h"
#include "command_handling.h"
#include "cd_spl.h"
#include "history.h"
#include "discover.h"
#include "pinfo.h"
#include "piping.h"
#include <termios.h>
#include <ctype.h>
#include <signal.h>

int count = 0;
char files_common[1000][1000];

void print_directory_main(struct dirent *File_stream, directoryInfo *currInfo, char *input)
{
    struct stat buffer;
    char tempo[1000];
    char *path_type = getcwd(tempo, 1000);
    strcat(path_type, "/");
    strcat(path_type, File_stream->d_name);
    // printf("The Comparator is : %s", input);

    char *max_common = (char *)malloc(sizeof(char) * 1000);

    if (stat(path_type, &buffer) != -1)
    {
        if (S_ISDIR(buffer.st_mode))
        {
            if (File_stream->d_name[0] != '.')
            {
                if (strncmp(File_stream->d_name, input, strlen(input)) == 0)
                {
                    // printf("\n%s/", File_stream->d_name);
                    strcpy(files_common[count], File_stream->d_name);
                    strcat(files_common[count], "/");
                    count++;
                }
            }
        }
        else if (buffer.st_mode & S_IXUSR)
        {
            // printf("\n%s", File_stream->d_name);
        }
        else
        {
            // printf("%s is the name of file, %s is input\n", File_stream->d_name, input);
            if (strncmp(File_stream->d_name, input, strlen(input)) == 0)
            {
                // printf("\n%s", File_stream->d_name);
                strcpy(files_common[count], File_stream->d_name);
                count++;
            }
        }
    }
}

void Autocomplete(directoryInfo *Curr_Info, char *input, char* final_prompt)
{
    // printf("\n The Input is: %s", input);
    count = 0;

    // Check for the last space and cut that part out
    int i = 0;
    int last_index = 0;
    while (input[i] != '\0')
    {
        if (input[i] == ' ')
        {
            last_index = i;
        }
        i++;
    }

    char *incomplete_file_name = (char *)malloc(sizeof(char) * 1000);

    strcpy(incomplete_file_name, &input[last_index + 1]);
    // printf("\nThe Incomplete File name is : %s\n", incomplete_file_name);

    // printf("The Current Working Directory is : %s\n", Curr_Info->currentPath);

    DIR *directory_input = opendir(Curr_Info->currentPath);

    struct dirent *File_stream = readdir(directory_input); // This generates the stream of files
    while (File_stream != NULL)
    {
        print_directory_main(File_stream, Curr_Info, incomplete_file_name);
        File_stream = readdir(directory_input);
    }

    printf("\n");

    // for(int i = 0; i < count; i++){
    //     printf("%s\n", files_common[i]);
    // }

    if (count == 1)
    {

        int i = 0;
        int last_index = 0;
        while (input[i] != '\0')
        {
            if (input[i] == ' ')
            {
                last_index = i;
            }
            i++;
        }

        strcpy(&input[last_index + 1], files_common[0]);
        printf("%s> %s", final_prompt, input);
    }
    if (count > 1)
    {
        for (int i = 0; i < count; i++)
        {
            printf("%s\n", files_common[i]);
        }

        int j = 0;
        int common_num = 0;
        int comeOut = 0;
        for (j = 0; j < strlen(files_common[0]); j++)
        {
            if (comeOut == 1)
            {
                break;
            }
            for (int i = 0; i < count; i++)
            {
                char temp;
                temp = files_common[0][j];
                if (temp != files_common[i][j])
                {
                    common_num = j;
                    comeOut = 1;
                    break;
                }
            }
        }

        // printf("Common letters are : %d\n", common_num);

        while (input[i] != '\0')
        {
            if (input[i] == ' ')
            {
                last_index = i;
            }
            i++;
        }

        strncpy(&input[last_index + 1], files_common[0], common_num);
        printf("%s> %s", final_prompt, input);
        
    }
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int iscddotdot = 0;

int dealingwithBackground(char *input, char **commands)
{
    int num_parts = 0;
    char delimiters[] = "&";
    // Use a copy so as to not modify the original input
    char *copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(copy, input);

    char *part = strtok(copy, delimiters);

    for (int i = 0; part; i++)
    {
        commands[i] = part;
        part = strtok(NULL, delimiters);
        num_parts = i + 1;
    }

    return num_parts;
    printf("\n %d \n", num_parts);
}

int dealingwithPipes(char *input, char **piped_commands)
{
    int num_parts = 0;
    char delimiters[] = "|";
    // Use a copy so as to not modify the original input
    char *copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(copy, input);

    char *part = strtok(copy, delimiters);

    for (int i = 0; part; i++)
    {
        piped_commands[i] = part;
        part = strtok(NULL, delimiters);
        num_parts = i + 1;
    }

    return num_parts;
}

int dealingwithSpaces(char *input, char **commands)
{
    int num_parts = 0;
    char delimiters[] = " \t\n";
    // Use a copy so as to not modify the original input
    char *copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(copy, input);

    char *part = strtok(copy, delimiters);

    for (int i = 0; part; i++)
    {
        commands[i] = part;
        part = strtok(NULL, delimiters);
        num_parts = i + 1;
    }

    return num_parts;
    printf("\n %d \n", num_parts);
}

// A function to split the strings wherever \n and \t are involved
int dealingwithSemiColons(char *input, char **commands)
{
    int num_parts = 0;
    char delimiters[] = ";";
    // Use a copy so as to not modify the original input
    char *copy = (char *)malloc(sizeof(char) * 1000);
    strcpy(copy, input);

    char *part = strtok(copy, delimiters);

    for (int i = 0; part; i++)
    {
        commands[i] = part;
        part = strtok(NULL, delimiters);
        num_parts = i + 1;
    }

    return num_parts;
    printf("\n %d \n", num_parts);
}

int main()
{
    struct utsname buffer;
    uname(&buffer);

    // Here Buffer contains a struct that in turn contains the following
    // Sys_name
    // Node_name
    // Release_version
    // Hardware identifier

    // Concatenate the shell prompt
    char *prompt = (char *)malloc(sizeof(char) * 1000);

    // OperatingSystem name
    // Pointer to sysname, buffer.sysname

    char temp[1];
    // How to get current working directory

    // username@system_name:~
    char *finalprompt = (char *)malloc(sizeof(char) * 1000);
    temp[0] = '<';
    strcat(finalprompt, temp);
    gethostname(prompt, 1000); // Put the value of name of the host into prompt
    temp[0] = '@';
    strcat(prompt, temp);
    strcat(prompt, buffer.sysname);
    temp[0] = ':';
    strcat(prompt, temp);
    // Here prompt is username@system_name:
    strcat(finalprompt, prompt);
    // Here Final_prompt is <username@system_name:

    directoryInfo *Curr_Info = InitializeShell();
    // Initially home directory is same as cwd
    getcwd(Curr_Info->currentPath, 1000);
    getcwd(Curr_Info->HomePath, 1000);
    getcwd(Curr_Info->prevPath, 1000);

    // Create an array of strings -> commands are those that run in the foreground
    char **commands = (char **)malloc(sizeof(char *) * 1000);
    for (int i = 0; i < 1000; i++)
    {
        commands[i] = (char *)malloc(sizeof(char) * 1000);
    }

    // Create an array of strings -> commands are those that run in the background
    char *background_commands[1000] = {"\0"};

    char *old_pwd = (char *)malloc(sizeof(char) * 1000);

    char *temp_flush = (char *)malloc(sizeof(char) * 1000);
    strcpy(temp_flush, finalprompt);

    int numcommands = 0;

    // Maintain a Read_Array
    char **Read_Array = (char **)malloc(sizeof(char *) * 20); // Define a series of arrays
    for (int i = 0; i < 20; i++)
    {
        Read_Array[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Create a temp string

    // printf("%s ", Shell_Location(Curr_Info, finalprompt));

    // Creates a shell, that is creates shell prompt of the form
    //<username@system_name:curr_dir>

    // Maintain another previous_command
    char *prev_command = (char *)malloc(sizeof(char) * 1000);
    strcpy(prev_command, "\0");

    int NotWritten = 0;

    while (1)
    {
        // signal(SIGCHLD, die);
        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigtstp_handler);
        char *inp = malloc(sizeof(char) * 100);
        char c;
        printf("%s> ", strcat(finalprompt, Obtain_Rel(Curr_Info)));

        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                {
                    Autocomplete(Curr_Info, inp, finalprompt);
                    pt = strlen(inp);
                }
                else if (c == 4)
                {
                    exit(0);
                }
                else
                {
                    printf("%d\n", c);
                }
            }
            else
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        printf("\nInput Read: [%s]\n", inp);

        // strcpy(Curr_Info->prevPath, Curr_Info->currentPath;

        // Dealing with random spaces and tabs
        // printf("Reached\n");
        char *command_line = (char *)malloc(sizeof(char) * 1000);
        size_t size_of = 1000;
        // getline(&command_line, &size_of, stdin); // Get Line in command line
        strcpy(command_line, inp);
        strcat(command_line, "\n");

        char *command_line_cpy = (char *)malloc(sizeof(char) * 1000);
        strcpy(command_line_cpy, command_line);

        if (strcmp(command_line, "\n") == 0)
        {
            strcpy(finalprompt, temp_flush);
            continue;
        }

        if (strcmp(prev_command, command_line) == 0)
        {
            NotWritten = 1;
        }
        else
        {
            writeToHistory(Curr_Info, command_line, &numcommands, Read_Array);
        }

        int num_parts = dealingwithSemiColons(command_line, commands);
        // for (int i = 0; i < num_parts; i++)
        // {
        //     printf("%s\n", commands[i]);
        // }

        // Commands contains the commands seperated by the semi colons

        // For each command, check if they are any background processes
        for (int i = 0; i < num_parts; i++)
        {
            char *piped_commands[1000] = {"\0"};
            char *copy_commands = (char *)malloc(sizeof(char) * 1000);
            strcpy(copy_commands, commands[i]);
            // printf("The First Command is -> %s\n", copy_commands);
            // This modifies the current string
            int num_background = dealingwithBackground(commands[i], background_commands);

            // Here number of background processes are num_background - 1, as the
            // Basic Premise lies in the fact that there can be only a few background processes, but once a foreground process
            // Exists there can be no more background processes

            // Here the foreground process is background_commands[num_background - 1]
            // char **foreground_command = (char **)malloc(sizeof(char *) * 1000);
            // for (int i = 0; i < 1000; i++)
            // {
            //     foreground_command[i] = (char *)malloc(sizeof(char) * 1000);
            // }
            char *foreground_command[1000] = {"\0"};
            int number = dealingwithSpaces(background_commands[num_background - 1], foreground_command);

            int num_pipes = dealingwithPipes(background_commands[num_background - 1], piped_commands);

            if (num_pipes > 1)
            {
                piping(piped_commands, num_pipes, Curr_Info, old_pwd);
            }
            else
            {
                command_handling(foreground_command, number, background_commands[num_background - 1], old_pwd, Curr_Info, background_commands, copy_commands, num_parts, num_background); // Here we send the entire last command, for echo purposes only
            }
        }

        strcpy(finalprompt, temp_flush);
        // printf("%s\n", old_pwd);
        if (NotWritten == 0)
        {
            numcommands++;
        }
        NotWritten = 0;
        strcpy(prev_command, command_line_cpy);
    }

    return 0;
}