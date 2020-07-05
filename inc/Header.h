/*
    FILE: Header.h
    PROJECT: Assignment 5 - Histogram
    FIRST-VERSION: 2020-04-16
    PROGRAMMER: Eric Emerson
    DESCRIPTION:
	The function of this file is to contain global constants and functions used throughout the multiple project programs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

//Defines the program constants.
#define DATA_CONSUMER_ALARM_READ    2
#define DATA_CONSUMER_SLEEP_PRINT   10
#define DATA_MEMORY_MAX             256
#define DATA_MEMORY_STRING          256
#define DATA_MEMORY_KEY             65536

//Defines the shared memory struct.
struct shmstruct{
    int read_index;
    int write_index;
    char letters[DATA_MEMORY_MAX];
} shmstruct;

/*
    FUNCTION: wrap_max
    DESCRIPTION: Wraps the value back to zero after reaching the max.
    PARAMETERS: int num: The number to wrap around.
		int max: The maximum number that when equaled will wrap.
    RETURNS: int: The number either wrapped or un-wrapped.
*/
int wrap_max(int num, int max){
    if(num >= max){
        return max - num;
    }
    return num;
}

/*
    FUNCTION: system_execute
    DESCRIPTION: Executes a program with parameters.
    PARAMETERS: const char *filename: The name of the file to execute.
		int shmid: The shared memory id to pass.
		int pid1: The first program id.
		int pid2: The second program id.
    RETURNS: N/A
*/
void system_execute(const char *filename, int shmid, int pid1, int pid2){

    //Converts the shared memeory id to a string.
    char shmstr[DATA_MEMORY_STRING];
    char shmpid1[DATA_MEMORY_STRING];
    char shmpid2[DATA_MEMORY_STRING];
    char shmarg[DATA_MEMORY_STRING];
    sprintf(shmstr, " %d", shmid);
    sprintf(shmpid1, " %d", pid1);
    sprintf(shmpid2, " %d", pid2);
    strcpy(shmarg, filename);
    strcat(shmarg, shmstr);
    strcat(shmarg, shmpid1);
    strcat(shmarg, shmpid2);
    strcat(shmarg, " &");

    //Creates the program.
    system(shmarg);

}

/*
    FUNCTION: random_range
    DESCRIPTION: Selects a random number within the range.
    PARAMETERS: int min: The minimum value.
		int max: The maximum value.
    RETURNS: int: A random value within the range.
*/
int random_range(int min, int max){
    return rand() % (max + 1 - min) + min;
}
