#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns current char without consuming.
char reader_peek(char** str) {
    return *str[0];
}

// Returns char after current char without consuming.
char reader_next(char** str) {
    str++;
    char c = **str;
    return c;
}

// Consumes the current char and returns it.
char reader_consume(char** str) {
    char c = **str;
    char* f = *str;
    f++;
    *str = f;
    return c;
}

// Given a string, and two ints (a and b). Return a new string starting with character a
// and ending at character location b.
char* make_substring(char* str, int b, int e) {
    char* new_string = malloc(sizeof(int) * (e - b) + 1);
    int i;
    for (i = 0; i < (e-b); i++) {
        new_string[i] = str[b + i];
    }
    new_string[i+1] = '\0';
    return new_string;
}