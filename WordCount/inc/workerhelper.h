/*
 * workerhelper.h
 *
 *  Created on: 23 mag 2019
 *      Author: cristian
 */

#ifndef WORKERHELPER_H_
#define WORKERHELPER_H_

/* whichIsMyPortion: assign the correct number of file to analyze */
struct node * whichIsMyPortion(long start, long chunkSize);

void buildfrequencieshash(FILE* fp) ;

#endif /* WORKERHELPER_H_ */
