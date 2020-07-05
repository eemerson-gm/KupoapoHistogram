/*
    FILE: DP-1.c
    PROJECT: Assignment 5 - Histogram
    FIRST-VERSION: 2020-04-16
    PROGRAMMER: Eric Emerson
    DESCRIPTION:
	The function of this file is to produce characters from A to T into the shared memory buffer in a burst of 20 every 2 seconds.
*/

#include "../../inc/Header.h"

//Defines the global variables.
int shmid;

/*
    FUNCTION: handle_exit
    DESCRIPTION: Clears the shared memory space and exits the program.
    PARAMETERS: int s: The signal id being passed.
    RETURNS: N/A
*/
void handle_exit(int s){

    //Destroys the shared memory.
    shmctl(shmid, IPC_RMID, NULL);

    //Exits the process.
    exit(0);

}

int main(void){

    //Generates a random seed.
    srand(time(0));

    //Sets a signal handeler.
    signal(SIGINT, handle_exit);

    //Generates a unique shared memory key.
    key_t shmkey = ftok(".", DATA_MEMORY_KEY);

    //Defines the shared memory id.
    shmid = shmget(shmkey, sizeof(shmstruct), 0660|IPC_CREAT);

    //Defines the shared memory map.
    struct shmstruct *shmmap;
    shmmap = (struct shmstruct *)shmat(shmid, NULL, 0);

    //Sets the default index values.
    shmmap->read_index = 0;
    shmmap->write_index = 0;

    //Executes a new program.
    system_execute("../../DP-2/bin/DP-2", shmid, getpid(), getpid());

    //Enters the main producer loop.
    while(1){

        //Sleeps for a little bit.
        sleep(2);

        //Waits until letters are read.
        if ((wrap_max(shmmap->write_index + 20, DATA_MEMORY_MAX)) < shmmap->read_index){

            //Loops for 20 characters.
            for(int c = 0; c < 20; c++){

                //Writes a character to the shared memory.
                shmmap->letters[shmmap->write_index] = random_range('A', 'T');

                //Increases the index.
                shmmap->write_index++;

                //Warps the index position.
                shmmap->write_index = wrap_max(shmmap->write_index, DATA_MEMORY_MAX);

            }

        }

    }

}
