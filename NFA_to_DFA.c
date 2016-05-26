//
// Created by Moro on 23/05/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"
#include "main.h"

int check_for_closure(void* edge) {
    struct Edge* edge1 = edge;
    if (edge1->c == '\0') {
        return 1;
    }
    return 0;
}

struct GenericList* filter(struct GenericList* list, int (*f)(void*)) {
    if (list != NULL) {
        void* head = list->head;
        struct GenericList* tail = list->tail;
        if (f(list)) {
            return make_generic_list(list, filter(tail, f));
        } else {
            return filter(tail,f);
        }
    } else {
        return NULL;
    }
}

struct GenericList* generic_map(struct GenericList* list, void* (*f)(void*)) {
    if (list != NULL) {
        return make_generic_list(f(list->head), generic_map(list->tail, f));
    }
    return NULL;
}

struct GenericList* find_closures(struct GenericList* edges) {
    struct GenericList* ls = filter(edges, check_for_closure);

    return filter(edges, check_for_closure);
}

int count_list(struct List* edges){
    if (edges != NULL) {
        return 1 + count_list(edges->tail);
    }
    return 0;
}

void* get_state_from_edge(void* edge){
    struct Edge* edge1 = edge;
    void* returnvalue = edge1->state;
    return returnvalue;
}

struct StateList* find_all_closures(struct State* state){
    struct StateList* checked = NULL;
    struct GenericList* not_checked = make_generic_list(state, NULL);

    while (not_checked != NULL) {
        struct State* state_to_check_for_closures = not_checked->head;
        struct GenericList* edges_to_check = converter(state_to_check_for_closures->edges);
        struct GenericList* closures = find_closures(edges_to_check);
        struct GenericList* states_reachable_by_epsilon = generic_map(closures, get_state_from_edge);
        clear_list(edges_to_check);
        clear_list(closures);
        Concatenate(not_checked, states_reachable_by_epsilon);
        clear_list(states_reachable_by_epsilon);

        checked = state_list_append(checked, not_checked->head);
        not_checked = not_checked -> tail;
    }

}

int main(int argc, char* argv[]) {
    struct NFA* s = evaluate("[a-cd]p*|lol");

    printf("%d", count_list(s->start->edges));

    struct List* ls = find_closures(s->start->edges);
    printf("%d", count_list(ls));

    return 0;
}