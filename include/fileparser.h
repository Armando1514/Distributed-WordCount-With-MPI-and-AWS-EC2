/*
 * fileparser.h
 * it offers the functions related the operations on file
 * - list_of_file_path(),  reads all the files into the folder "sample" and return a linked list with filenames.
 * - get_total_file_size(), returns the sum of all the files sizes (bytes).
 * Author: Armando Ferrara
*/

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

// where are the file to read
#define FILEPATH "sample"

#define PATHMODE "/" //unix like OS

struct node {
    char* data;
    long size;
    struct node* next;
};

/* get_total_file_sizes:
 * get the total file sizes in bytes */
long get_total_file_sizes();

/* get_header_list()
 * return the head of the list
 */
struct node* get_header_list();

struct node* list_of_file_path();

/* free_the_list_of_files
 * deallocate all the list structure
 */

void free_the_list_of_files();

#endif /* FILEPARSER_H_ */
