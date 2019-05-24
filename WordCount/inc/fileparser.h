/*
 * fileparser.h
 * Functions useful for get the total file size, and the list of
 * the file names.
 * Author: Armando Ferrara
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_
#define FILEPATH "sample"
#define PATHMODE "/" //unix like OS

struct node {
   char *data;
   long size;
   struct node *next;
};

struct node * getHeader();

struct node* getfilenamelist();

/* getTotalFileSizes: get the total file sizes in bytes */
long getTotalFileSizes();


struct  node* listOfFileNames();


void insertFirst(char *data, short mode);

// freeAllTheList deallocate all the list structure
void freeAllTheList(short mode);

#endif /* FILEPARSER_H_ */
