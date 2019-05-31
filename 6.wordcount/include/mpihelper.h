//
// Created by cristian on 30/05/19.
//

#ifndef WORDCOUNT_MPIHELPER_H
#define WORDCOUNT_MPIHELPER_H

// number have to be > 2 for overhead reasons.
#define WORDONTHENETWORK 50



void worker_sender();


void master_receiver(int num_proc);


#endif //WORDCOUNT_MPIHELPER_H
