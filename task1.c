#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define N 25 //Count of words to print

int do_task1() {
    puts("Task1\n\n");
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

//    for (i = 0; i < 119; i++) {
//        puts(stop_words[i]);
//    }
    fclose(fp);


    static char words[100000][2][255] = {0};
    int unique_words = 0;

    // Read text file
    fp = fopen("../pride-and-prejudice.txt", "r");
    if (fp == NULL) {
        printf("Text file open error.\n");
        return 0;
    }

    int j;
    char word[255];

    read_loop:
    if (fscanf(fp, "%s", word) != 1) {
        goto end_loop;
    }

    //check word for non-alphabetic symbols
    j = 0;
    char new_word[255] = "";
    na_loop:
    if (j < strlen(word)) {
        if (isalpha(word[j])) {
            new_word[strlen(new_word)] = tolower(word[j]); //assign symbol to new word
        }
        j++;
        goto na_loop;
    }
    if (strlen(new_word) > 0) {
        new_word[strlen(new_word)] = '\0';
        strcpy(word, new_word); //replace old word
//        puts(word);
    } else {
        goto read_loop;
    }

    // check if word is stop word
    j = 0;
    sw_loop:
    if (j < 120) {
        if (strcmp(word, stop_words[j]) == 0) {
            goto read_loop;
        }
        j++;
        goto sw_loop;
    }

    // check if word exists in words arr
    j = 0;
    word_loop:
    if (j < unique_words) {
        if (strcmp(word, words[j][0]) == 0) { // if word already exists
//            printf("Word already exists %d times.\n", words[j][1][0]);
            char *ptr;
            int word_counter = strtol(words[j][1], &ptr, 10);
            sprintf(words[j][1], "%d", word_counter + 1); // increase word counter
            goto read_loop;
        }

        j++;
        goto word_loop;
    }

    //if word does not exist in words arr
//    printf("New word \"%s\". Unique words: %d.\n", word, unique_words);
    strcpy(words[unique_words][0], word); //add it to words arr
    sprintf(words[unique_words][1], "%d", 1); // set word counter to 0
    unique_words++; // increase unique words counter
    goto read_loop;

    end_loop:
    fclose(fp);

    //Bubble sort words array
    i = 0;
    sort_loop:
    if (i < unique_words - 1) {
        j = 0;
        sort1_loop:
        if (j < (unique_words - i - 1)) {

            char *ptr;
            int j_count = strtol(words[j][1], &ptr, 10);
            int jpl1_count = strtol(words[j + 1][1], &ptr, 10);
            if (j_count < jpl1_count) { // swap words
                char temp_w[255], temp_c[255];
                strcpy(temp_w, words[j][0]);
                strcpy(temp_c, words[j][1]);

                strcpy(words[j][0], words[j + 1][0]);
                strcpy(words[j][1], words[j + 1][1]);

                strcpy(words[j + 1][0], temp_w);
                strcpy(words[j + 1][1], temp_c);
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
    if (i < N) {
        printf("%s - %s\n", words[i][0], words[i][1]);
        i++;
        goto print_loop;
    }

    return 0;
}
