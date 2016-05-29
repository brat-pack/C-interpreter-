//
// Created by Moro on 23/05/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NFA_Constructor.h"
#include "lists.h"

// Create a new list item
struct List* list_create(void* h, struct List* tail) {
    struct List* list = malloc(sizeof(struct List));
    list->head = h;
    list->tail = tail;
    return list;
}

// Clear an entire list, but not the content.
void list_clear(struct List* list){
    if(list != NULL){
        list_clear(list->tail);
        free(list);
    }
}

// Get the number of elements in the list.
int list_length(struct List* list) {
    if (list != NULL) {
        return 1 + list_length(list->tail);
    }
    return 0;
}

// Append a value to an existing list
struct List* list_append(struct List* list, void* value) {
    if (list != NULL) {
        if (list->tail != NULL) {
            list_append(list->tail, value);
        } else {
            list->tail = list_create(value, NULL);
        }
    } else {
        list = list_create(value, NULL);
    }
    return list;
}

// Checks whether the element val is present in the list
int list_contains(struct List* list, void* val) {
    if(list != NULL){
        if(list->head == val){
            return 1;
        } else {
            return list_contains(list->tail, val);
        }
    } return 0;
}

// Concatentates two lists. List ls2 is becomes the tail of ls1.
struct List* list_concatenate(struct List* ls1, struct List* ls2) {
    struct List* first = ls1;
    while (ls1->tail != NULL){
        ls1 = ls1->tail;
    }
    ls1->tail = ls2;
    return first;
}



// Iterates function
void list_iterate(struct List* list, void (*f)(void*)) {
    if(list != NULL){
        f(list->head);
        list_iterate(list->tail, f);
    }
}

// Map function
struct List* list_map(struct List* list, void* (*f)(void*)) {
    if (list != NULL) {
        return list_create(f(list->head), list_map(list->tail, f));
    }
    return NULL;
}

// Fold function
void* list_fold(struct List* list, void* (*f)(void*, void*),void* acc) {
    if(list != NULL){
        return f(list->head, list_fold(list->tail,f, acc));
    } return acc;
}

// Filter function
struct List* list_filter(struct List* list, int (*f)(void*)) {
    if (list != NULL) {
        void* head = list->head;
        struct List* tail = list->tail;
        if (f(head)) {
            return list_create(list->head, list_filter(tail, f));
        } else {
            return list_filter(tail,f);
        }
    } else {
        return NULL;
    }
}

// TODO: Legacy code, to be removed.
struct List* list_converter(struct List* list) {
    return list;
}




