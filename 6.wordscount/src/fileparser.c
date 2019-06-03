/*
 * fileparser.c
 * it offers the functions related the operations on file
 * - list_of_file_path(),  reads all the files into the folder "sample" and return a linked list with filenames.
 * - get_total_file_size(), returns the sum of all the files sizes (bytes).
 * Author: Armando Ferrara
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "../include/fileparser.h"

// head for the list file names (internal).
struct node* head = NULL;

//insert link at the first location
void __insert_path_in_list(char* data)
{

    //create a link
    struct node* link = (struct node*)malloc(sizeof(struct node));

    link->data = strdup(data);

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}

/* __getFileSize:
 * get the file size in bytes
 * of one specific file by name.*/
long __get_file_size(const char* filename)
{
    long size;
    FILE* f;

    f = fopen(filename, "r");
    if (f == NULL)
        return -1;
    // jump to the end of the file
    fseek(f, 0, SEEK_END);
    // tell you where the file position is, does it means size.
    size = ftell(f);
    fclose(f);
    return size;
}

/* get_total_file_sizes:
 * get the total file sizes in bytes */
long get_total_file_sizes()
{
    long total_size = 0;
    long single_size = 0;
    struct node* ptr = (struct node*)list_of_file_path();
    char* path_string;

    while (ptr != NULL) {
        path_string = ptr->data;
        single_size = __get_file_size(path_string);
        total_size = total_size + single_size;
        ptr->size = single_size;
        ptr = ptr->next;
    }
    return total_size;
}

/* __calculate_path_string:
 * return the  location path for a name file */
char* __calculate_path_string(char* name_file)
{
    char* result = malloc(strlen(PATHMODE) + strlen(FILEPATH) + strlen(PATHMODE) + strlen(name_file) + 2);

    strcpy(result, "..");
    strcat(result, PATHMODE);
    strcat(result, FILEPATH);
    strcat(result, PATHMODE);
    strcat(result, name_file);
    return result;
}

/* __list_of_file_path():
 * list of all the name of files */
struct node* list_of_file_path()
{
    struct dirent* de; // Pointer for directory entry

    char* result = malloc(strlen(FILEPATH) + strlen(PATHMODE) + 1);

    strcpy(result, "..");
    strcat(result, PATHMODE);
    strcat(result, FILEPATH);
    // opendir() returns a pointer of DIR type.
    DIR* dr = opendir(result);

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        perror("Error, is not possible to open the current directory");
        return NULL;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir() (simple read the name of the file)
    while ((de = readdir(dr)) != NULL) {
        if ((strcmp(de->d_name, ".")) && (strcmp(de->d_name, ".."))) {
            __insert_path_in_list(__calculate_path_string(de->d_name));
        }
    }
    closedir(dr);
    return head;
}



struct node* get_header_list()
{
    return head;
}
