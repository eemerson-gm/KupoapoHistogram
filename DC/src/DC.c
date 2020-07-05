/*
    FILE: DC.c
    PROJECT: Assignment 5 - Histogram
    FIRST-VERSION: 2020-04-16
    PROGRAMMER: Eric Emerson
    DESCRIPTION:
	The function of this file is to consume data being placed into a buffer by the two data producers and print a histogram on the screen.
*/

#include "../../inc/Header.h"

//Defines the global variables.
int shmid;
int pid1;
int pid2;
struct shmstruct *shmmap;
int letter_count[('U' - 'A')];

/*
    FUNCTION: handle_alarm_read
    DESCRIPTION: Reads from the shared memory buffer when called by the alarm.
    PARAMETERS: int s: The signal id being passed.
    RETURNS: N/A
*/
void handle_alarm_read(int s){

    //Loops through each character in the map.
    while(shmmap->read_index != shmmap->write_index){

        //Checks if the character is valid.
        if (shmmap->letters[shmmap->read_index] >= 'A'){

            //Adds to the letter count.
            letter_count[shmmap->letters[shmmap->read_index] - 'A'] += 1;

        }

        //Increments the read index.
        shmmap->read_index++;

        //Wraps the read index.
        shmmap->read_index = wrap_max(shmmap->read_index, 256);

    }

    //Sets the alarm signal.
    signal(SIGALRM, handle_alarm_read);
    alarm(DATA_CONSUMER_ALARM_READ);

}

/*
    FUNCTION: print_histogram
    DESCRIPTION: Prints the number of each letter as a histogram.
    PARAMETERS: N/A
    RETURNS: N/A
*/
void print_histogram(){

    //Clears the screen.
    system("clear");

    //Loops through each letter count.
    for(int n = 0; n < ('U' - 'A'); n++){

        //Gets a temporary count.
        int temp_count = letter_count[n];

        //Gets a histogram string.
        char histogram[DATA_MEMORY_MAX] = "";

        //Adds 100 characters to the string.
        while(temp_count >= 100){
            temp_count -= 100;
            strcat(histogram, "*");
        }

        //Adds 10 characters to the string.
        while(temp_count >= 10){
            temp_count -= 10;
            strcat(histogram, "+");
        }

        //Adds 1 characters to the string.
        while(temp_count >= 1){
            temp_count -= 1;
            strcat(histogram, "-");
        }

        //Prints the histogram count.
        printf("%c-%03d %s\n", n + 'A', letter_count[n], histogram);
        fflush(stdout);

        //Clears the memory.
        for(int c = 0; c < DATA_MEMORY_STRING; c++){
            histogram[c] = '\0';
        }

    }

}

/*
    FUNCTION: handle_exit
    DESCRIPTION: Reads a final time from the buffer and prints the histograms and an exit message.
    PARAMETERS: int s: The signal id being passed.
    RETURNS: N/A
*/
void handle_exit(int s){

    //Reads the buffer a final time.
    alarm(0);
    handle_alarm_read(0);

    //Prints a final histogram.
    print_histogram();

    //Prints the exit message.
    printf("Shazam !!");

    //Exits the program.
    kill(pid1, SIGINT);
    kill(pid2, SIGINT);
    exit(0);

}

int main(int argc, char* argv[]){

    //Recieves the shared memory id.
    shmid = atoi(argv[1]);
    pid1 = atoi(argv[2]);
    pid2 = atoi(argv[3]);

    //Defines the shared memory map.
    shmmap = (struct shmstruct *)shmat(shmid, NULL, 0);

    //Resets the letter count.
    for(int c = 0; c < ('U' - 'A'); c++){
        letter_count[c] = 0;
    }

    //Sets the exit signal.
    signal(SIGINT, handle_exit);

    //Sets the reading alarm signal.
    signal(SIGALRM, handle_alarm_read);
    alarm(DATA_CONSUMER_ALARM_READ);

    //Starts a timer.
    time_t time_start = clock();

    //Loops infinetly.
    while(1){

        //Checks if the timer is up.
        if(((clock() - time_start)/CLOCKS_PER_SEC) >= DATA_CONSUMER_SLEEP_PRINT){

            //Resets the time.
            time_start = clock();

            //Prints historgrams.
            print_histogram();

        }

    }

}
