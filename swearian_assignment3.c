#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Holds data for one movie, links next movie in the list.
struct movie{
    char *title;
    char languages[5][21];
    int year;
    int numLanguages;
    double rating;
    struct movie *next;
};

// Creates movie struct from one line of the CSV file.
struct movie *createMovie(char *line){
    struct movie *currMovie = malloc(sizeof(struct movie));
    char *saveptr;

    // Title
    char *token = strtok_r(line, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    // Languages
    token = strtok_r(NULL, ",", &saveptr);

    char *langList = token + 1;
    char *closeBracket = strchr(langList, ']');

    if (closeBracket != NULL){
        *closeBracket = '\0';
    }

    currMovie->numLanguages = 0;
    
    char *langSave;
    char *lang = strtok_r(langList, ";", &langSave);

    while (lang != NULL){
        strcpy(currMovie->languages[currMovie->numLanguages], lang);
        currMovie->numLanguages++;
        lang = strtok_r(NULL, ";", &langSave);
    }

    //Rating
    token = strtok_r(NULL, ",\n", &saveptr);
    currMovie->rating = strtof(token, NULL);

    currMovie->next = NULL;
    return currMovie;
}

// Reads CSV file, returns linked list of movies.
struct movie *processFile(char *filePath, int *count){
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len =0;
    struct movie *head = NULL;
    struct movie *tail = NULL;
    *count = 0;

    getline(&currLine, &len, movieFile);

    while (getline(&currLine, &len, movieFile) != -1){
        currLine[strcspn(currLine, "\n")] = '\0';
        struct movie *newNode = createMovie(currLine);

        if (head == NULL){
            head = newNode;
            tail = newNode;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
        (*count)++;
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

// Movies released in a specific year.
void moviesByYear(struct movie *list){
    int year;
    int found = 0;

    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    while (list != NULL){
        if (list->year == year){
            printf("%s\n", list->title);
            found = 1;
        }
        list = list->next;
    }
    if (!found){
        printf("No data about movies released in the year %d\n", year);
    }
}

// Highest rated movie for each year.
void bestByYear(struct movie *list){
    for (int year = 1900; year <= 2021; year++){
        struct movie *best = NULL;
        struct movie *curr = list;
        while (curr != NULL){
            if (curr->year == year && (best == NULL || curr->rating > best->rating)){
                best = curr;
            }
            curr = curr->next;
        }
        if (best != NULL){
            printf("%d %.1f %s\n", best->year, best->rating, best->title);
        }
    }
}


// Movies released in a specific language.
void moviesByLanguage(struct movie *list){
    char language[21];
    int found = 0;

    printf("Enter the language for which you want to see movies: ");
    scanf("%s", language);

    while (list != NULL){
        for (int i = 0; i < list->numLanguages; i++){
            if (strcmp(list->languages[i], language) == 0){
                printf("%d %s\n", list->year, list->title);
                found = 1;
                break;
            }
        }
        list = list->next;
    }
    if (!found){
        printf("No data about movies released in %s\n", language);
    }
}

void printMenu(void){
    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
}

int main(int argc, char **argv){
    if (argc < 2){
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }
    int count = 0;
    struct movie *list = processFile(argv[1], &count);
    printf("Processed file %s and parsed data for %d movies\n", argv[1], count);

    int choice = 0;

    while (choice != 4){
        printMenu();
        scanf("%d", &choice);

        switch (choice){
            case 1:
                moviesByYear(list);
                break;
            case 2:
                bestByYear(list);
                break;
            case 3:
                moviesByLanguage(list);
                break;
            case 4:
                break;
            default:
                printf("You entered an incorrect choice. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}
