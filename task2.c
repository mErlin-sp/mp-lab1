#include <stdio.h>

#define LINES_PER_PAGE 45

#define MAX_WORDS 100000
#define MAX_LINE_LENGTH 10000
#define MAX_WORD_LENGTH 1000

int do_task2() {
    puts("Task2\n\n");

    FILE *fp;
    char stop_words[120][255];

    // Read stop words file
    fp = fopen("../stop_words.txt", "r");
    if (fp == NULL) {
        printf("Stop words file open error.\n");
        return 0;
    }

    int i = 0;
    sw_read_loop:
    if (fscanf(fp, "%s", stop_words[i]) == 1) {
        i++;
        goto sw_read_loop;
    }
    fclose(fp);


    static char dictionary[MAX_WORDS][3][MAX_WORD_LENGTH] = {};
    int unique_words = 0;

    // Read text file
    fp = fopen("../pride-and-prejudice.txt", "r");
    if (fp == NULL) {
        printf("Text file open error.\n");
        return 0;
    }

    i = 0; // current page
    char line[MAX_LINE_LENGTH]; // current line

    line_loop:
    if (fgets(line, sizeof line, fp) == NULL) { // if line == NULL
        goto end_loop; // end
    }

    char *word_p = NULL;
    word_p = strtok(line, " ");
    i++;
    goto word1_loop;

    word_loop:
    word_p = strtok(NULL, " ");

    word1_loop:
    if (word_p == NULL) { // if no words left in this line
        goto line_loop; // go to next line
    }

    //check word for non-alphabetic symbols
    int j = 0;

    char new_word[MAX_WORD_LENGTH] = "";
    na_loop:
    if (j < strlen(word_p)) {
        if (isalpha(word_p[j])) {
            new_word[strlen(new_word)] = tolower(word_p[j]); //assign symbol to new word
        }
        j++;
        goto na_loop;
    }
    if (strlen(new_word) > 0) {
        new_word[strlen(new_word)] = '\0';
        strcpy(word_p, new_word); //replace old word
    } else {
        goto word_loop;
    }

    // check if word is stop word
    j = 0;
    sw_loop:
    if (j < 120) {
        if (strcmp(word_p, stop_words[j]) == 0) {
            goto word_loop;
        }
        j++;
        goto sw_loop;
    }

    // check if word exists in words arr
    j = 0;
    dict_loop:
    if (j < unique_words) {
        if (strcmp(word_p, dictionary[j][0]) == 0) { // if word already exists
//            printf("Word already exists %d times.\n", dictionary[j][1][0]);
            char *ptr = NULL;
            int word_counter = strtol(dictionary[j][1], &ptr, 10);
            sprintf(dictionary[j][1], "%d", word_counter + 1); // increase word counter
            snprintf(dictionary[j][2], MAX_WORD_LENGTH, "%s, %d", dictionary[j][2],
                     ((i / LINES_PER_PAGE) + 1)); // add page number
            goto word_loop;
        }

        j++;
        goto dict_loop;
    }

    //if word does not exist in words arr
//    printf("New word \"%s\". Unique words: %d.\n", word, unique_words);
    strcpy(dictionary[unique_words][0], word_p); //add it to words arr
    sprintf(dictionary[unique_words][1], "%d", 1); // set word counter to 1
    sprintf(dictionary[unique_words][2], "%d", (i / LINES_PER_PAGE) + 1); // add first page
    unique_words++; // increase unique words counter
    goto word_loop;

    end_loop:
    fclose(fp);

    //Remove words with count > 100
    i = 0;
    remove_loop:
    if (i < unique_words) {
        char *ptr = NULL;
        int word_counter = strtol(dictionary[j][1], &ptr, 10);
        if (word_counter > 100) {
            j = i;
            remove1_loop:
            if (j < (unique_words - 1)) {
                strcpy(dictionary[j][0], dictionary[j + 1][0]);
                strcpy(dictionary[j][1], dictionary[j + 1][1]);
                strcpy(dictionary[j][2], dictionary[j + 1][2]);
                j++;
                goto remove1_loop;
            }
//            unique_words--;
        }
        i++;
        goto remove_loop;
    }

    //Bubble sort words array
    i = 0;
    sort_loop:
    if (i < unique_words - 1) {
        j = 0;
        sort1_loop:
        if (j < (unique_words - i - 1)) {
            if (strcmp(dictionary[j][0], dictionary[j + 1][0]) > 0) { // swap words
                char temp_w[MAX_WORD_LENGTH], temp_c[MAX_WORD_LENGTH], temp_s[MAX_WORD_LENGTH];
                strcpy(temp_w, dictionary[j][0]);
                strcpy(temp_c, dictionary[j][1]);
                strcpy(temp_s, dictionary[j][2]);

                strcpy(dictionary[j][0], dictionary[j + 1][0]);
                strcpy(dictionary[j][1], dictionary[j + 1][1]);
                strcpy(dictionary[j][2], dictionary[j + 1][2]);

                strcpy(dictionary[j + 1][0], temp_w);
                strcpy(dictionary[j + 1][1], temp_c);
                strcpy(dictionary[j + 1][2], temp_s);
            }

            j++;
            goto sort1_loop;
        }

        i++;
        goto sort_loop;
    }

    //Print words
    i = 0;
    print_loop:
    if (i < unique_words) {
        printf("%s - %s\n", dictionary[i][0], dictionary[i][2]);
        i++;
        goto print_loop;
    }

    return 0;
}