//
// Created by Moro on 23/05/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char peek(char* str) {
    return str[0];
}

char next(char* str) {
    str++;
    char c = *str;
    return c;
}

char consume(char** str) {
    char* f = *str;
    f++;
    char c = **str;
    *str = f;
    return c;
}

int eat(char** str, char c) {
    char* x = *str;
    x++;
    if (str[0][0] == c) {
        *str = x;
        return c;
    }
    *str = x;
    return 0;
}

char* make_substring(char* str, int b, int e) {
    char* new_string = malloc(sizeof(int) * (e - b) + 1);
    int i;
    for (i = 0; i < (e-b); i++) {
        new_string[i] = str[b + i];
    }
    new_string[i+1] = '\0';
    return new_string;
}