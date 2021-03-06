/*
 * mpihelper.h : if the processor is the master,
 * it waits the words and the occurrences from the slaves (master_receiver()).
 * If the processors are the slaves, it puts all the words and the occurrences
 * in a structure where the size is choose by the developer
 * (through #define WORDONTHENETWORK) and sends it to the master (worker_sender()).
 */

#ifndef WORDCOUNT_MPIHELPER_H
#define WORDCOUNT_MPIHELPER_H

// number have to be > 2 for overhead reasons.
//How many words i want in the struct to send over the network.
#define WORDONTHENETWORK 50

/*
 * master_receiver(int num_proc)
 * receives the x struct from the slaves
 * where are contained the words compute,
 * and store in the master hasmap.
 */

void master_receiver(int num_proc);

/*
 * work_sender():
 * send the result of own hashmap
 * to the master, for do that we created
 * a new struct.
 */
void worker_sender();

#endif //WORDCOUNT_MPIHELPER_H
