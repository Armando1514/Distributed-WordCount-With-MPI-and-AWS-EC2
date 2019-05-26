/*
 * workerhelper.c
 * The worker helper, manage all the file i the package, assign the
 * right number of file to read, the right position for start the reading
 * and manage all the issue related in a parallel context related to the reading
 * from files.
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <workerhelper.h>
#include <fileparser.h>
#include <hashmap.h>

// maximum word length
#define MAXWORDLENGTH 500
// minimum for considering a sequence of character as a word
#define MINWORDLENGTH 2

/*
 * __which_is_my_portion(long start, long chunkSize):
 * assign the correct number of file to analyze to
 * the worker.
 */

struct node* __which_is_my_portion(long start, long chunk_size)
{
    // get the list of files into the package
    struct node* current = get_header_list();
    struct node* head_list = get_header_list();
    struct node* next;
    char* path_string;

    long temp_size = current->size;
    // the file to analyze is the first
    if (temp_size > start) {
        next = current->next;
        path_string = current->data;

        current = next;
    }
    else {
        temp_size = 0;
        //scroll until you don't reach the file to analyze
        while (current != NULL && temp_size < start) {

            next = current->next;

            path_string = current->data;

            temp_size = temp_size + current->size;

            // i don't need to analyze this file, i'm able to delete it.
            if (temp_size < start) {
                free(current->data);
                free(current);
                head_list = next;
            }

            current = next;
        }
    }
    //remove the bytes yet analyzed
    temp_size = temp_size - start;

    //we can need more than one file, if we need of more bytes.
    while (current != NULL && temp_size < chunk_size) {

        temp_size = temp_size + current->size;
        next = current->next;
        current = next;
    }

    //remove the other elements in the list after the current node
    if (current != NULL) {

        next = current->next;

        current->data = NULL;
        current = next;
    }

    return get_header_list();
}

/*
 * build_frequencies_hash(FILE* fp, long word_to_count, long word_counted):
 * count the occurrences of each word into the specified file.
 */
long __build_frequencies_hash(FILE* fp, long word_to_count, long word_counted)
{

    int ch;
    char str[MAXWORDLENGTH];
    memset(str, 0, MAXWORDLENGTH);
    int i = 0;

    //if the last slave read your word, ignore the word until the next non alphanumeric value
    if (ftell(fp) != 0) {

        if (isalnum(ch = fgetc(fp))) {
            word_counted++;

            // read until there are character to read
            while (isalnum(ch = fgetc(fp))) {

                word_counted++;
            }
        }
    }

    while (word_to_count > word_counted && (ch = fgetc(fp)) != EOF) {
        word_counted++;

        // if is not a space and is not a punctuation and is alphanumeric
        if (isalnum(ch)) {
            // convert the words to uppercase
            if (ch > 96 && ch < 123)
                ch -= 32;

            str[i] = ch;
            i++;

            if (i > MAXWORDLENGTH) {
                memset(str, 0, 500);
                i = 0;

                //ignore the word
                while (isalnum(ch = fgetc(fp))) {

                    word_counted++;
                }
            }
        }
        else {

            // a word is composed min by two characters
            if (i > MINWORDLENGTH)
                //insert the word and increment the value
                insert_or_increment(str);

            //reset the string buffer
            memset(str, 0, i + 1);
            i = 0;
        }
    }

    //insert the last word
    if (ch == EOF && i > MINWORDLENGTH) {

        insert_or_increment(str);
        memset(str, 0, i + 1);

        i = 0;
    }
    //if the reading is stopped during the scan of a word, finish to read it
    if (i > 0 && isalnum(ch)) {

        // read until there are character to read
        while (isalnum(ch = fgetc(fp))) {
            if (ch > 96 && ch < 123)
                ch -= 32;
            str[i] = ch;
            if (i > MAXWORDLENGTH) {
                memset(str, 0, 500);
                i = 0;

                //ignore the word
                while (isalnum(ch = fgetc(fp))) {

                    word_counted++;
                }
            }
            i++;
            // for statistic use
            word_counted++;
        }

        insert_or_increment(str);
    }

    return word_counted;
}

/*
 * calculate_word_frequencies(long start, long chunk_size):
 * calculate the word frequencies for each file.
 */
void calculate_word_frequencies(long start, long chunk_size)
{
    FILE* fp; /* reference to the file */

    struct node* my_portion = __which_is_my_portion(start, chunk_size); /* my portion file to read */

    int i = 0;

    long word_counted = 0;

    while (my_portion != NULL && my_portion->data != NULL && word_counted < chunk_size) {
        fp = fopen(my_portion->data, "r");

        if (i == 0) {
            fseek(fp, start, SEEK_SET);
            i++;
        }

        //start to count the word in the file
        word_counted = __build_frequencies_hash(fp, chunk_size, word_counted);

        fclose(fp);

        my_portion = my_portion->next;
    }
}

/* calculate_total_number_of_bytes():
 * calculates the total number of bytes.
 */
long calculate_total_number_of_bytes()
{
       // get the total number of bytes by the sum of all the byte size from files
	       return get_total_file_sizes();

}

/*
 * deallocate_the_lists():
 * free the space used by hashmap and
 * list files.
 */

void deallocate_the_lists()
{
	free_the_list_of_files();

	free_hashmap();
}

/* report_execution(long execution_time)
 * report in a file the total execution time
 * and each word - occurrences values.
 */

void report(long execution_time)
{
	printf("START REPORT:\n");
	report_hash_elements();
	printf("END REPORT:\n");

}
