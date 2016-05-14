//
// Created by Moro on 12/05/16.
//
#include <stdio.h>
#include <stdlib.h>


struct Transition;

int StateCount = 1;

struct Edge {
    char c;
};


struct State {
    int is_final;
    int number;
};


struct NFA {
    struct State* head;
    struct Edge* tail;
    int c;
};


struct State* CreateState() {
    struct State* s = malloc(sizeof(struct State));
    *s = {StateCount, 0};
    StateCount++;
    return s;
}


struct Transition OrTransition(struct Transition* left_nfa, char *regex) {
    struct Transition t1 = EpsilonTransition(CreateState());
    left_nfa->tail = &t1;



    //t1.head = CreateState();


    //left_nfa->tail = t1;
}


void DefineRegex (char *regex) {
    struct State* OutsideEdge = NULL;

    for (int i = 0; i < sizeof(regex); i++) {
        char c = regex[i];

    }
}


void CreateNFA(char *regexs[]) {
    int StateCount = 0;

    for (int i = 0; i < (sizeof(regexs) / sizeof(regexs[0])); i++) {

    }
}