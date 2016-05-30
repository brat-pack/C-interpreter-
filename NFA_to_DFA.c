#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"
#include "NFA_Constructor.h"

int check_for_character_transition(void* edge, char c) {
    struct Edge* edge1 = edge;
    if (edge1->c == c) {
        return 1;
    }
    return 0;
}

// Checks if an edge is an epislon.
int check_for_closure(void* edge) {
    return check_for_character_transition(edge, '\0');
}

// Returns a list of edges.
struct List* find_closures(struct List* edge_list) {
    return list_filter(edge_list, check_for_closure);
}

// Returns a state thats connected to an edge.
void* get_state_from_edge(void* edge){
    struct Edge* edge1 = edge;
    struct State* returnvalue = edge1->state;
    return returnvalue;
}

// Returns a list of states.
struct List* find_all_closures(struct State* state){
    struct List* checked = NULL; // Type: State list
    // First state is added to the not_checked list.
    struct List* not_checked = list_create(state, NULL); // Type: State list

    while (not_checked != NULL) {
        struct State* state_to_check_for_closures = not_checked->head; // Extract the current state at the head of not_checked.
        struct List* edges_to_check = state_to_check_for_closures->edges; // Type: Edge list
        struct List* closures = find_closures(edges_to_check); // Type: Edge list
        struct List* states_reachable_by_epsilon = list_map(closures, get_state_from_edge); // Type: State list
        list_clear(closures);
        list_concatenate(not_checked, states_reachable_by_epsilon);
        checked = list_append(checked, not_checked->head); // Add the checked states to the checked list.
        not_checked = not_checked -> tail; // Go to the next not_checked element.
    }
    return checked;
}

// Returns a list of connecting states.
struct List* get_connecting_states(struct State* state) {
    struct List* genlist = state->edges;
    return list_map(genlist, get_state_from_edge);
}

// Prints all states with their connecting edges.
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

// Returns a list of states.
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

struct List* calculate_all_closures(struct List* state){
    struct List* all_closures = NULL;
    while(state != NULL){
        all_closures = list_append(all_closures, find_all_closures(state->head));
        state = state->tail;
    }
    return all_closures;
}

struct List* get_closures_from_set(struct List* states) {
    struct List* returnList = NULL;

    while (states != NULL) {
        struct State* s = states->head;
        struct List* connecting_states = find_all_closures(s);
        while (connecting_states != NULL) {
            returnList = list_append(returnList, connecting_states->head);
            struct List* tail = connecting_states->tail;
            free(connecting_states);
            connecting_states = tail;
        }
        states = states->tail;
    }

    return returnList;
}

// Move function gets all states we can transition to from a set of states on character c.
struct List* Move(struct List* states, char c) {
    struct List* returnList = NULL;
    while (states != NULL) {
        struct State* state = states->head;
        struct List* edges = state->edges;
        while (edges != NULL) {
            if (check_for_character_transition(edges->head, c)) {
                returnList = list_append(returnList, get_state_from_edge(edges->head));
            }
            edges = edges->tail;
        }
        states = states->tail;
    }
    return returnList;
}

// IGNORE MAIN
int main(int argc, char* argv[]) {
    struct NFA* s = evaluate("(a|b)*abb");
    s->end->is_final = 1;

    printf("%d states \n", state_count);
    printf("%d total states \n", list_length(get_all_states(s->start, NULL)));
    printf("");
    struct List* all_closures = get_all_states(s->start, NULL);
    all_closures = calculate_all_closures(all_closures);
    struct List* dtran = get_closures_from_set(Move(all_closures->head, 'a'));
    print_states(dtran);

    return 0;
}