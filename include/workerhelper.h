/*
 * workerhelper.h
 *
 It manages all the files into the package, assign the
 right number of file to read and the right position for start the reading.
 It also resolves all the  issue related to the reading from files  in a parallel context.
 It allows the communications between the main and the lower level (file parser and hashmap)
 *  Author: Armando Ferrara
 */

#ifndef WORKERHELPER_H_
#define WORKERHELPER_H_

// maximum word length
#define MAXWORDLENGTH 500
// minimum for considering a sequence of character as a word, starting from value 0.
#define MINWORDLENGTH 0

/*
 * calculate_word_frequencies(long start, long chunk_size):
 * calculate the word frequencies for each file.
 */
void calculate_word_frequencies(long start, long chunk_size);

/*
 * deallocate_the_lists():
 * free the space used by hashmap and
 * list files.
 */

void deallocate_the_lists();

/* calculate_total_number_of_bytes():
 * Assign for each worker, the right number of bytes to read.
 */
long calculate_total_number_of_bytes();

/* report(long execution_time)
 * report in a file the total execution time
 * and each word - occurrences values.
 */
void report(float execution_time, short my_rank);

#endif /* WORKERHELPER_H_ */
