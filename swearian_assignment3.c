#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define ONID "swearian"
#define PREFIX "movies_"
#define EXTENSION ".csv"


// Holds data for one movie, links next movie in the list.
struct movie{
    char *title;
    int year;
    struct movie *next;
};

// Creates movie struct from one line of the CSV file.
struct movie *createMovie(char *line){
    struct movie *currMovie = malloc(sizeof(struct movie));
    char *saveptr, *token;

    // Title
    token = strtok_r(line, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    currMovie->next = NULL;
    return currMovie;
}

// Reads CSV file, returns linked list of movies.
struct movie *processFile(char *filePath){
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    struct movie *head = NULL;
    struct movie *tail = NULL;
    struct  movie *newNode;

    if (movieFile == NULL){
        return NULL;
    }

    getline(&currLine, &len, movieFile);

    while (getline(&currLine, &len, movieFile) != -1){
        currLine[strcspn(currLine, "\r\n")] = '\0';
        if (strlen(currLine) == 0){
            continue;
        }

        newNode = createMovie(currLine);

        if (head == NULL){
            head = newNode;
            tail = newNode;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

// Frees every movie in the list (freeList).
void freeList(struct movie *list){
    struct movie *next;
    while (list != NULL){
        next = list->next;
        free(list->title);
        free(list);
        list = next;
    }
}


// Finds largest or smallest (findFile).


// Checks if file exists (fileExists).


// Creates new directory, writes one YYYY.txt file per year (processChosenFile).


// File picker menu (pickFile).


// Main menu loop.
int main(void){
    int choice = 0;
    srandom(time(NULL));

    while (choice != 2){
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                pickFile();
                break;
            case 2:
                break;
            default:
                printf("You entered an incorrect choice. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}
