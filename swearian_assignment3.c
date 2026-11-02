#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

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
int findFile(int findLargest, char *result){
    DIR *currDir = opendir(".");
    struct dirent *entry;
    struct stat fileStat;
    size_t nameLen, extLen = strlen(EXTENSION);
    int found = 0;
    off_t bestSize = 0;

    if (currDir == NULL){
        return 0;
    }

    while ((entry = readdir(currDir)) != NULL){
        nameLen = strlen(entry->d_name);

        if (strncmp(entry->d_name, PREFIX, strlen(PREFIX)) != 0){
            continue;
        }
        if (nameLen < extLen || strcmp(entry->d_name + nameLen - extLen, EXTENSION) != 0){
            continue;
        }
        if (stat(entry->d_name, &fileStat) != 0 || !S_ISREG(fileStat.st_mode)){
            continue;
        }
        if (!found || (findLargest && fileStat.st_size > bestSize) || (!findLargest && fileStat.st_size < bestSize)){
            bestSize = fileStat.st_size;
            strcpy(result, entry->d_name);
            found = 1;
        }
    }
    closedir(currDir);
    return found;
}


// Checks if file exists (fileExists).
int fileExists(char *fileName){
    struct stat fileStat;
    return stat(fileName, &fileStat) == 0;
}


// Creates new directory, writes one YYYY.txt file per year (processChosenFile).
void processChosenFile(char *fileName){
    char dirName[64], filePath[320];
    struct movie *list, *curr;
    FILE *yearFile;

    printf("Now processing the chosen file named %s\n", fileName);

    do{
        sprintf(dirName, "%s.movies.%ld", ONID, random() % 100000);
    } while (fileExists(dirName));
    
    mkdir(dirName, 0750);
    chmod(dirName, 0750);

    printf("Created directory with name %s\n", dirName);

    list = processFile(fileName);

    curr = list;
    while (curr != NULL){
        sprintf(filePath, "%s/%d.txt", dirName, curr->year);
        yearFile = fopen(filePath, "a");
        if (yearFile != NULL){
            fprintf(yearFile, "%s\n", curr->title);
            fclose(yearFile);
            chmod(filePath, 0640);
        }
        curr = curr->next;
    }
    freeList(list);
}


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
