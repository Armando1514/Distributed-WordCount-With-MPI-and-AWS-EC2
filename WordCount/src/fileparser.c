/*
 * fileparser.c
 * Function userful for get the total file size, and the list of
 * the file names.
 * Author: Armando Ferrara
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fileparser.h>
#include <hashmap.h>



// list of file names (internal).
struct node *head = NULL;
// names of file to analyse for each processor.




struct node* getfilenamelist()
{
	return head;
}

void setHeader(struct node *ref)
{
	head = ref;
}

//insert link at the first location
void  insertFirst(char *data) {

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));

   link->data = strdup(data);

   //point it to old first node
   link->next = head;

   //point first to new first node
   head = link;



}



//return the number of file
int numberOfFile() {
   struct node *ptr = head;
   int i = 0;
   //start from the beginning
   while(ptr != NULL) {
      i++;
      ptr = ptr->next;
   }

  return i;
}

// freeAllTheList deallocate all the list structure
void freeAllTheLists(short mode)
{

   struct node *np;
   struct node *nptmp;
   if(mode == 1)
   {
	   np = head;

	   while(np != NULL)
	   {
		   nptmp = np->next;
		   free(np->data);
		   free(np);
		   np = nptmp;
	   }
   }

}





/* getFileSize: get the file size in bytes  of one specific file, by name.*/
long getFileSize(const char* filename)
{
    long size;
    FILE *f;

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

/* getTotalFileSizes: get the total file sizes in bytes */
long getTotalFileSizes()
{
	long totalSize = 0;
    long singleSize = 0;
	struct node *ptr = listOfFileNames();

	while(ptr != NULL)
	{
		char *pathString = ptr->data;
		singleSize = getFileSize(pathString);
		totalSize = totalSize + singleSize;
        ptr->size = singleSize;
		ptr = ptr->next;

	}
	return totalSize;
}

/* calculateResultString: return the path of the location path for a name file */
char * calculatePathString(char *nameFile)
{
	char *result = malloc(strlen(PATHMODE)+strlen(FILEPATH) + strlen(PATHMODE) + strlen(nameFile) + 2);

        strcpy(result, ".");
	    strcat(result, PATHMODE);
		strcat(result, FILEPATH);
		strcat(result, PATHMODE);
		strcat(result, nameFile);
		return result;
}


/* listOfFileNames: list of all the name of files */
struct  node* listOfFileNames()
{
    struct dirent *de;  // Pointer for directory entry

	char *result = malloc(strlen(FILEPATH) + strlen(PATHMODE)+ 1);

    strcpy(result, ".");
    strcat(result, PATHMODE);
    strcat(result, FILEPATH);

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(result);

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Error, is not possible to open the current directory" );
        return NULL;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir() (simple read the name of the file)
    while ((de = readdir(dr)) != NULL)
    {
    		if( (strcmp(de->d_name, ".")) && (strcmp(de->d_name, "..")) )
    		{
    				insertFirst(calculatePathString(de->d_name));
    		}

    }
    closedir(dr);
    return head;

}
