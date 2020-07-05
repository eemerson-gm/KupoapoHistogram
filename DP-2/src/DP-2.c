/*
    FILE: DP-2.c
    PROJECT: Assignment 5 - Histogram
    FIRST-VERSION: 2020-04-16
    PROGRAMMER: Eric Emerson
    DESCRIPTION:
	The function of this file is to produce characters from A to T into the shared memory buffer once every 1/20 of a second.
*/

#include "../../inc/Header.h"

/*
    FUNCTION: handle_exit
    DESCRIPTION: Exits the program.
    PARAMETERS: int s: The signal id being passed.
    RETURNS: N/A
*/
void handle_exit(int s){

    //Exits the process.
    exit(0);

}

int main(int argc, char* argv[]){

    //Sets a signal handeler.
    signal(SIGINT, handle_exit);

    //Recieves the shared memory id.
    int shmid = atoi(argv[1]);
    int shmpid = atoi(argv[2]);

    //Defines the shared memory map.
    struct shmstruct *shmmap;
    shmmap = (struct shmstruct *)shmat(shmid, NULL, 0);

    //Executes a new program.
    system_execute("../../DC/bin/DC", shmid, getpid(), shmpid);

    //Enters the main producer loop.
    while(1){

        //Sleeps for a little bit.
        sleep(1/20);

        //Waits until letters are read.
        if ((wrap_max(shmmap->write_index + 1, DATA_MEMORY_MAX)) != shmmap->read_index){

            //Writes a character to the shared memory.
            shmmap->letters[shmmap->write_index] = random_range('A', 'T');

            //Increments the index.
            shmmap->write_index++;

            //Warps the index position.
            shmmap->write_index = wrap_max(shmmap->write_index, DATA_MEMORY_MAX);

        }

    }

}
