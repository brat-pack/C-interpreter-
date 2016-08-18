//
// Created by hachiko on 18/08/16.
//

#include "PrettyPrinter.h"
#include <string.h>
#include <stdio.h>

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




