//
// Created by Moro on 23/05/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"
#include "NFA_Constructor.h"

int check_for_closure(void* edge) {
    struct Edge* edge1 = edge;
    if (edge1->c == '\0') {
        return 1;
    }
    return 0;
}


struct GenericList* find_closures(struct GenericList* edge_list) {
    return filter(edge_list, check_for_closure);
}

int count_list(struct List* edges){
    if (edges != NULL) {
        return 1 + count_list(edges->tail);
    }
    return 0;
}

void* get_state_from_edge(void* edge){
    struct Edge* edge1 = edge;
    struct State* returnvalue = edge1->state;
    return returnvalue;
}

void* get_edge_from_state(void* state){

}

struct GenericList* find_all_closures(struct State* state){
    struct GenericList* checked = NULL;
    struct GenericList* not_checked = make_generic_list(state, NULL);

    while (not_checked != NULL) {
        struct State* state_to_check_for_closures = not_checked->head;
        struct GenericList* edges_to_check = converter(state_to_check_for_closures->edges); // Type: Edge list
        struct GenericList* closures = find_closures(edges_to_check); // Type: Edge list
        struct GenericList* states_reachable_by_epsilon = generic_map(closures, get_state_from_edge); // Type: State list
        clear_list(edges_to_check);
        clear_list(closures);
        Concatenate(not_checked, states_reachable_by_epsilon);
        checked = generic_list_append(checked, not_checked->head);
        not_checked = not_checked -> tail;
    }
    return checked;
}

void print_state_id(void* state) {
    struct State* s = state;
    printf("\tid:%d\n", s->number);
}

void* get_state_if_not_in_list(void* head, void* list){
    struct Edge* edge = head;
    if(!Contains(list, edge->state)){
        return generic_list_append(list, edge->state);
    } else {
        return list;
    }
}

struct GenericList* get_connecting_states(struct State* state) {
    struct GenericList* genlist = converter(state->edges);
    return generic_map(genlist, get_state_from_edge);
}

struct GenericList* get_all_states(struct State* state, struct GenericList* total_states){
    struct GenericList* connecting_states = get_connecting_states(state);

    if(!(Contains(total_states, state))) {
        if (total_states == NULL) total_states = generic_list_append(total_states, state);
        else generic_list_append(total_states, state);

        while (connecting_states != NULL) {
            if (!Contains(total_states, connecting_states->head)) {
                get_all_states(connecting_states->head, total_states);
            }
            connecting_states = connecting_states->tail;
        }
    }

    return total_states;
}

/*
int main(int argc, char* argv[]) {
    struct NFA* s = evaluate("(a|b)*abb");
    s->end->is_final = 1;

    printf("%d states \n", state_count);
    printf("%d total states \n", generic_list_length(get_all_states(s->start, NULL)));

    printf("");
    struct GenericList* ls = find_all_closures(s->start);
    //iterate_generic_list(ls, print_state_id);

    //iterate_generic_list(ls, print_state_id);


    return 0;
}*/