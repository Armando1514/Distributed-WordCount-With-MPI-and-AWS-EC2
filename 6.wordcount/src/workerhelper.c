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

#include "../include/hashmap.h"

#include "../include/fileparser.h"


#include "../include/workerhelper.h"

/*
 * __which_is_my_portion(long start, long chunkSize):
 * assign the correct number of file to analyze to
 * the worker.
 */

struct node* __which_is_my_portion(long * start, long chunk_size)
{
    // get the list of files into the package
    struct node* current = get_header_list();
    struct node* head_list = get_header_list();
    struct node* next;
    long tmp_start = (long)*start;
    char* path_string;

    long temp_size = current->size;
    // the file to analyze is the first
    if (temp_size > *start) {
        next = current->next;
        path_string = current->data;

        current = next;
    }
    else {
        temp_size = 0;
        //scroll until you don't reach the file to analyze
        while (current != NULL && temp_size <= tmp_start) {

            next = current->next;


            path_string = current->data;

            temp_size = temp_size + current->size;

            // i don't need to analyze this file, i'm able to delete it.
            if (temp_size < tmp_start) {
                printf("VALUE: %ld , TEMP_SIZE : %ld \n",*start,temp_size);

                // subtract the file from start
                *start = *start - current->size;

                free(current->data);
                free(current);
                head_list = next;

            }

            current = next;
        }
    }
    //remove the bytes yet analyzed
    temp_size = temp_size - tmp_start;


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

    return head_list;
}

/*
 * build_frequencies_hash(FILE* fp, long word_to_count, long word_counted):
 * count the occurrences of each word into the specified file.
 */
long __build_frequencies_hash(FILE* fp, long word_to_count, long word_counted)
{

    int ch;
    char str[MAXWORDLENGTH];

    memset(str, '\0', MAXWORDLENGTH);
    int i = 0;

    //if the last slave read your word, ignore the word until the next non alphanumeric value
    if (ftell(fp) != 0) {
        while (isalnum(ch = fgetc(fp))) {

                word_counted++;
            }
        }




    //also if the process is stopped during the reading of a character, it still finish to analyze it.
    while (((word_to_count > word_counted) || (i > 0) ) && (ch = fgetc(fp)) != EOF) {

        // if is not a space and is not a punctuation and is alphanumeric
        if (isalnum(ch)) {
            // convert the words to uppercase
            if (ch > 96 && ch < 123)
                ch -= 32;
            str[i] = ch;
            i++;

            if (i > MAXWORDLENGTH) {
                memset(str, '\0', MAXWORDLENGTH);
                i = 0;

                //ignore the word
                while (isalnum(ch = fgetc(fp))) {

                    word_counted++;
                }
            }
        }
        else {

            // a word is composed min by two characters
            if (i > MINWORDLENGTH) {

                str[i] = '\0';
                //insert the word and increment the value
                insert_or_increment(str);

            }

            //reset the string buffer
            memset(str, '\0', i );
            i = 0;
        }

        word_counted++;

    }

    //insert the last word
    if (ch == EOF && i > MINWORDLENGTH) {
        str[i] = '\0';

        insert_or_increment(str);

        memset(str, '\0', i);

        i = 0;
    }


    memset(str, '\0', i );
    return word_counted;
}

/*
 * calculate_word_frequencies(long start, long chunk_size):
 * calculate the word frequencies for each file.
 */
void calculate_word_frequencies(long start, long chunk_size)
{
    FILE* fp; /* reference to the file */

    struct node* my_portion = __which_is_my_portion(&start, chunk_size); /* my portion file to read */
    int i = 0;
    if(start < 0)
        start = 0;

    long word_counted = 0;

    while (my_portion != NULL && my_portion->data != NULL && word_counted < chunk_size) {
        printf("START:%ld CHUNK:%ld ,PATH: %s\n",start,chunk_size,my_portion->data);

        fp = fopen(my_portion->data, "r");

        if(fp==NULL)
          perror("Error in file opening at line 200 in the file workerhelper.c");

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

