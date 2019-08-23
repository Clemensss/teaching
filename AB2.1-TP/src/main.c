#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SEG 10 //time limit for the tests
#define DASH "--------------------------"

void child(char str[]);
void father(char str_pid[], char arg[], char file[]);

int main(int argc, char *argv[])
{
    //check if there is an argument
    if (argv[1] == NULL)
    {
        printf("Error: No arguments found.\nPlease insert a valid argument.\n");
        exit(-1);
    }

    //check if the argument is valid
    if (strcasecmp(argv[1], "cpu") != 0 && strcasecmp(argv[1], "cpu-mem") != 0)
    {
        printf("Error: Invalid argument.\nPlease insert a valid argument.\n");
        exit(-1);
    }

    //creates child process
    int pid = fork();

    //converts pid into a string
    char str_pid[20];
    sprintf(str_pid, "%d", pid);

    //problem occoured exiting with -1
    if (pid < 0)
    {
        perror("Error: ");
        exit(-1);
    }

    //it's the child process
    else if (pid == 0)
    {
        child(argv[1]);
    }

    //it's the father process
    else
    {
        father(str_pid, argv[1], argv[2]);
    }

    perror("Status");
    printf("tchau\n");
    exit(0);
}

/*  This function's purpose is to print into bash and into a file 
 * the defined process's memory and cpu usage.
 *
 * It writes into the string bash_cmd the bash command:
 *  $ ps -e -o pid,rss,pcpu | grep $pid | awk '{ printf "%-10s%-5s\n", $2, $3 }'
 *  
 *  Which shows all the current processes' pids, resident size 
 * memories and their cpu usage, respectively. It then "greps" the 
 * process defined by the variable str_pid, and prints out the last 
 * 2 "ps" outputs: The memory and cpu usage;
 * 
 *  It only writes into a file if there is an argument in argv[2]
 */
void father(char str_pid[], char arg[], char file[])
{
    char bash_cmd[256], aux[20];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    FILE *filep;
    int n;

    //uses sprintf to print the command into the string
    sprintf(bash_cmd, "ps -e -o pid,rss,pcpu | grep %s", str_pid);
    strcat(bash_cmd, "| awk '{ printf \"%-10s%-5s\\n\", $2, $3 }'");

    if (file != NULL)
    {
        //adds this command to print output into the file
        sprintf(aux, " >> %s", file);
        strcat(bash_cmd, aux);

        //adds eye candy to file
        filep = fopen(file, "a");
        fprintf(filep, "%s\n", asctime(tm));
        fprintf(filep, "%s test\nPID: %s\n\n%-10s%-5s\n", arg, str_pid, "MEM(KB)", "CPU(%)");
        fclose(filep);
    }
    else
    {
        printf("%s test\nPID: %s\n\n%-10s%-5s\n", arg, str_pid, "MEM(KB)", "CPU(%)");
    }

    //ten second loop
    int i;
    for (i = 0; i < SEG; i++)
    {
        system(bash_cmd);
        sleep(1);
    }

    //writes into the str the command to kill the child process
    strcpy(bash_cmd, "kill ");
    strcat(bash_cmd, str_pid);
    system(bash_cmd);

    if (file != NULL)
    {
        filep = fopen(file, "a");
        fprintf(filep, "\n%s\n", DASH);
        fclose(filep);
    }
}

/*
 *  This function is used to force test cpu and ram power.
 *  Uses argv input to define which loop to run.
 */
void child(char str[])
{
    //infinite loop for cpu test
    if (strcasecmp(str, "cpu") == 0)
    {
        for (;;)
        {
        }
    }

    //infinite loop for cpu and memory test
    else if (strcasecmp(str, "cpu-mem") == 0)
    {
        int i;
        for (i = 0;; i++)
        {
            //reduces the number of loop interations that allocate memory
            if (i % 15 == 0)
            {
                malloc(1); //allocates 1 Byte
            }
        }
    }
}