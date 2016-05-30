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

struct List* find_closures(struct List* edge_list) {
    return list_filter(edge_list, check_for_closure);
}

void* get_state_from_edge(void* edge){
    struct Edge* edge1 = edge;
    struct State* returnvalue = edge1->state;
    return returnvalue;
}

struct List* find_all_closures(struct State* state){
    struct List* checked = NULL;
    struct List* not_checked = list_create(state, NULL);

    while (not_checked != NULL) {
        struct State* state_to_check_for_closures = not_checked->head;
        struct List* edges_to_check = list_converter(state_to_check_for_closures->edges); // Type: Edge list
        struct List* closures = find_closures(edges_to_check); // Type: Edge list
        struct List* states_reachable_by_epsilon = list_map(closures, get_state_from_edge); // Type: State list
        list_clear(edges_to_check);
        list_clear(closures);
        list_concatenate(not_checked, states_reachable_by_epsilon);
        checked = list_append(checked, not_checked->head);
        not_checked = not_checked -> tail;
    }
    return checked;
}

struct List* get_connecting_states(struct State* state) {
    struct List* genlist = list_converter(state->edges);
    return list_map(genlist, get_state_from_edge);
}

void print_states(struct List* states) {
    while (states != NULL) {
        struct State* state = states->head;
        printf("State id : %d\n", state->number);
        struct List* edges = state->edges;
        while (edges != NULL) {
            struct Edge* edge = edges->head;
            printf("%d: %c | ", edge->state->number, edge->c);
            edges = edges->tail;
        }
        printf("\n");
        states = states->tail;
    }
}

struct List* get_all_states(struct State* state, struct List* total_states){
    struct List* connecting_states = get_connecting_states(state);

    if(!(list_contains(total_states, state))) {
        if (total_states == NULL) total_states = list_append(total_states, state);
        else list_append(total_states, state);

        while (connecting_states != NULL) {
            if (!list_contains(total_states, connecting_states->head)) {
                get_all_states(connecting_states->head, total_states);
            }
            connecting_states = connecting_states->tail;
        }
    }

    return total_states;
}

// IGNORE MAIN
int main(int argc, char* argv[]) {
    struct NFA* s = evaluate("(a|b)*abb");
    s->end->is_final = 1;

    printf("%d states \n", state_count);
    printf("%d total states \n", list_length(get_all_states(s->start, NULL)));

    print_states(get_all_states(s->start, NULL));

    printf("");
    struct List* ls = find_all_closures(s->start);
    //iterate_generic_list(ls, print_state_id);

    //iterate_generic_list(ls, print_state_id);
    return 0;
}