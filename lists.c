//
// Created by Moro on 23/05/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "lists.h"

int generic_list_length(struct GenericList* list) {
    if (list != NULL) {
        return 1 + generic_list_length(list->tail);
    }
    return 0;
}

void* Fold(struct GenericList* list, void* (*f)(void*, void*),void* acc){
    if(list != NULL){
        return f(list->head, Fold(list->tail,f, acc));
    } return acc;
}

int Contains(struct GenericList* list, void* val){
    if(list != NULL){
        if(list->head == val){
            return 1;
        } else {
            return Contains(list->tail, val);
        }
    } return 0;
}

void iterate_generic_list(struct GenericList* list, void (*f)(void*)){
    if(list != NULL){
        f(list->head);
        iterate_generic_list(list->tail, f);
    }
}

struct GenericList* Concatenate(struct GenericList* ls1, struct GenericList* ls2) {
    struct GenericList* first = ls1;
    while (ls1->tail != NULL){
        ls1 = ls1->tail;
    }
    ls1->tail = ls2;
    return first;
}

struct GenericList* converter(struct List* list){
    if(list != NULL){
        return make_generic_list(list->head, converter(list->tail));
    }
    return NULL;
}

void clear_list(struct GenericList* list){
    if(list != NULL){
        clear_list(list->tail);
        free(list);
    }
}

struct GenericList* make_generic_list(void* h, struct GenericList* tail) {
    struct GenericList* list = malloc(sizeof(struct GenericList));
    list->head = h;
    list->tail = tail;
    return list;
}

struct GenericList* generic_list_append(struct GenericList* list, void* value) {
    if (list != NULL) {
        if (list->tail != NULL) {
            generic_list_append(list->tail, value);
        } else {
            list->tail = make_generic_list(value, NULL);
        }
    } else {
        list = make_generic_list(value, NULL);
    }
    return list;
}

struct StateList* make_state_list(struct State* state, struct StateList* tail) {
    struct StateList* state_list = malloc(sizeof(struct StateList));
    state_list->head = state;
    state_list->tail = tail;
    return state_list;
}

struct StateList* state_list_append(struct StateList* list, struct State* state) {
    if (list != NULL) {
        if (list->tail != NULL) {
            state_list_append(list->tail, state);
        } else {
            list->tail = make_state_list(state, NULL);
        }
    } else {
        list = make_state_list(state, NULL);
    }
    return list;
}

struct List* make_list(struct Edge* edge, struct List* tail) {
    struct List* list = malloc(sizeof(struct List));
    list->head = edge;
    list->tail = tail;
    return list;
}

struct NFAList* make_nfa_list(struct NFA* nfa, struct NFAList* tail) {
    struct NFAList* list = malloc(sizeof(struct NFAList));
    list->head = nfa;
    list->tail = tail;
    return list;
}

struct List* list_append(struct List* list, struct Edge* edge) {
    if (list != NULL) {
        if (list->tail != NULL) {
            list_append(list->tail, edge);
        } else {
            list->tail = make_list(edge, NULL);
        }
    } else {
        list = make_list(edge, NULL);
    }
    return list;
}

struct NFAList* list_nfa_append(struct NFAList* list, struct NFA* nfa) {
    if (list != NULL) {
        if (list->tail != NULL) {
            list_nfa_append(list->tail, nfa);
        } else {
            list->tail = make_nfa_list(nfa, NULL);
        }
    } else {
        list = make_nfa_list(nfa, NULL);
    }
    return list;
}


void state_list_iterate(struct List* list, void (*func)(struct Edge* edge)) {
    if (list != NULL) {
        func(list->head);
    }
}