//
// Created by hachiko on 18/08/16.
//

#include "PrettyPrinter.h"
#include <string.h>
#include <stdio.h>
void Print_state(State* state);


int list_contains(State* state, List* list){
    int count = list->count;
    Node* current_node = list->first;
    while(count > 0){
        if (current_node->value == state){
            return 1;
        }
        current_node->next;
        count--;
    }
    return 0;
}

List* get_connecting_states(State* state){
    List* connecting_states = List_Create();
    List* state_edges_list = state->edges;

    int count = state_edges_list->count;
    Node* node = state_edges_list->first;

    while(count > 0){
        Edge* edge = node->value;
        List_Append(connecting_states,edge->state);
        node = node->next;
        count--;
    }
    return connecting_states;
}

List* get_unique_states(NFA* nfa){
    List* unchecked_states = List_Create();
    List* checked_states = List_Create();
    List_Append(unchecked_states, nfa->start);

    while(unchecked_states->count > 0){
        State* current_state = unchecked_states->first->value;
        List_Remove_First(unchecked_states);
        if(!list_contains(current_state, checked_states)){
            List_Append(checked_states, current_state);
            List* connected_states = get_connecting_states(current_state);
            List_Concatenate(unchecked_states, connected_states);
        }
    }
    return checked_states;
}

void PrettyPrint_NFA(NFA* nfa){
    List* states = get_unique_states(nfa);
    List_Iterate(states, Print_state);
}

void Print_state(State* state){
    int n = state->number;
    char str[12];
    snprintf(str, 12, "%d", n);
}




