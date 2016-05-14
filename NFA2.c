//
// Created by Moro on 13/05/16.
//
#include <stdio.h>
#include <stdlib.h>

struct State {
    struct State** tails;
    int is_final;;
};


void clear_state(struct State* n) {
    for (int i = 0; i < 256; i++) {
        if (n->tails[i] != NULL) {
            clear_state(n->tails[i]);
        }
    }
    free(n->tails);
    free(n);
}


struct State* create_state(int is_final) {
    struct State* s = malloc(sizeof(struct State));
    for (int i = 0; i < 256; i++) {
        s->tails[i] = NULL;
    }
    s->is_final = is_final;
    return s;
}



// Instead of a for loop, this needs to be recursive or something. Going all the way to the end and recursing back.
void create_NFA(char regex[]) {
    struct State* root_state = malloc(sizeof(struct State));

    for (int i = 0; i < sizeof(regex) / sizeof(char); i++) {
        struct State* s = create_state(0);
    }
}

void parse_regexs(int regexs_a, char* regexs_v[]) {
    for (int i = 0; i < regexs_a; i++) {
        create_NFA(regexs_v[i]);
    }
}