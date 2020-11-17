#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int flagofAnd = 0;
FILE* f;

void initMyShell()
{

    printf("\t\t\t\tSimple shell\n");
    printf("\t\t\t\t************\n\n");

}

void printDirectory()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n %s",cwd);
}


void commandExecuter(char** command)
{
    pid_t pid = fork();

    if(pid == -1)
    {
        printf("Failed Execution!\n");
        return;
    }
    else if(pid == 0)  //Child
    {
        if(execvp(command[0], command) < 0)
        {
            printf("Command Couldn't be executed!\n");
        }
        exit(1);
    }
    else
    {
        if(flagofAnd == 0) {
            waitpid(pid, NULL, 0);
        }
        flagofAnd = 0;
    }
}

void spaceSeparator(char* input, char** args)
{
    char* delim = " ";
    char* token = strtok(input, delim);
    int i = 0;

    while(token != NULL) {

        if(strcmp(token, "&") == 0) {
            flagofAnd = 1;
            token = strtok(NULL, delim);
            //i++;
        } else {
            args[i] = token;
            token = strtok(NULL, delim);
            i++;
        }
    }
}


void commandReSetter(char** command)
{
    int i = 0;
    while(command[i] != NULL)
    {
        command[i] = NULL;
        i++;
    }
}

void logFileManager(char* input)
{
    f = fopen("/home/assem/Desktop/SimpleShell3/log.txt","a");
    fprintf(f,"%s\n",input);
    fclose(f);
}

void showSomeHelp()
{
printf("\nWelcome To My Shell\n");
printf("You can do some Unix Commands as:\n");
printf("1-pwd\n2-ls\n3-mkdir\n4-touch\n5-exit\n");

}


int main()
{
    initMyShell();
    char input[1024] = {0x0};
    char delim[] = " ";
    char* args[64] = {NULL};
    char s[1024];

    while (1) {
        commandReSetter(args);
        printf("User:~ assemsalama$ ");
        gets(input);
        logFileManager(input);
        spaceSeparator(input, args);
        if(strcmp(args[0], "cd") == 0)
        {
            if(strcmp(args[1], "..") == 0)
            {
                chdir("..");
                printf("\n%s\n", getcwd(s,1024));
            }
            else
            {
                chdir(args[1]);
                printf("\n%s\n",getcwd(s,1024));
            }
        }
        else if(strcmp(input, "exit") == 0)
        {
            printf("\nShell Ending Process...\n");
            exit(1);
        }
        else if(strcmp(input, "help") == 0)
        {
            showSomeHelp();
        }
        else
        {
            commandExecuter(args);
        }
    }



    return 0;
}
