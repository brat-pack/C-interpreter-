#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NFA_Constructor.h"
#include "reader.h"

// Global counter used to give each state a unique id.
int state_count = 0;

// Create a state.
void Assign_State_Number(State* state)
{
    state->number = state_count;
    state_count++;
}

State* Create_State() {
    State* state = calloc(0, sizeof(State));
    Assign_State_Number(state);
    state->is_final = 0;
    state->edges = List_Create();
    return state;
};

Edge* Create_Edge(State* state, char c) {
    Edge* edge = calloc(NULL, sizeof(Edge));
    edge->state = state;
    edge->c = c;
    return edge;
};

// Create an edge transition between two states on the character c.
void Link_States_On_Char(State* beginState, State* endState, char c) {
    Edge* edge = Create_Edge(endState, c);
    List_Append(beginState->edges, edge);
}

// Create an epsilon edge transition between two states
void Link_States_On_Epsilon(State* beginState, State* endState) {
    Link_States_On_Char(beginState, endState, '\0');
}

// Construct an NFA out of two states.
NFA* Create_NFA(State* beginState, State* endState) {
    NFA* nfa = calloc(0, sizeof(struct NFA));
    nfa->start = beginState;
    nfa->end = endState;
    return nfa;
}

// Create the most primitive NFA. Two states connected by a single edge. Used for 'c'
NFA* Create_Primitive_NFA(char c) {
    State* beginState = Create_State();
    State* endState = Create_State();
    Link_States_On_Char(beginState, endState, c);
    NFA* nfa = Create_NFA(beginState, endState);
    return nfa;
}

// Concatenate two NFA's together to form a new NFA. Used for ex. for ab
NFA* Concatenate_NFA(struct NFA* NFA_1, struct NFA* NFA_2) {
    if (NFA_2 == NULL) return NFA_1;
    List_Destroy(NFA_1->end->edges);
    NFA_1->end->edges = NFA_2->end->edges;
    NFA* concatenated_nfa = Create_NFA(NFA_1->start, NFA_2->end);
    free(NFA_1);
    free(NFA_2);
    return concatenated_nfa;
};

// Function for the union algorithm. Creates a union between two states. Used for |
NFA* Create_Union_NFA(NFA* NFA_1, NFA* NFA_2) {
    State* start_state = Create_State();
    State* end_state = Create_State();

    Link_States_On_Epsilon(NFA_1->end, end_state);
    Link_States_On_Epsilon(NFA_2->end, end_state);
    Link_States_On_Epsilon(start_state, NFA_1->start);
    Link_States_On_Epsilon(start_state, NFA_2->start);

    free(NFA_1);
    free(NFA_2);

    return Create_NFA(start_state, end_state);
}

// Function for the kleenstar NFA. Creates a kleenstar out of a single NFA. Used for *
NFA* Create_Kleenstar_NFA(NFA* nfa) {
    State* start_state = Create_State();
    State* end_state = Create_State();

    Link_States_On_Epsilon(start_state, end_state);
    Link_States_On_Epsilon(start_state, nfa->start);
    Link_States_On_Epsilon(nfa->end, end_state);
    Link_States_On_Epsilon(nfa->end, nfa->start);

    free(nfa);

    return Create_NFA(start_state, end_state);
}

// Links a list of NFA's to a new start state and a single end state. Used for evaluate match
// multiple and create range.
NFA* Create_Parallel_NFA_From_List(List* nfa_list) {
    State* start_state = Create_State();
    State* end_state = Create_State();
    
    for (Node* node = nfa_list->first; node != NULL; node = node->next)
    {
        NFA* nfa = node->value;
        Link_States_On_Epsilon(start_state, nfa->start);
        Link_States_On_Epsilon(nfa->end, end_state);
    }
    List_ClearAndDestroy(nfa_list);
    return Create_NFA(start_state, end_state);
}

// Creates primitives from a range of characters all linking to the same
// end state and originating from the same start state. For example a-z
NFA* Create_Range_NFA(int a, int b) {
    List* nfas = List_Create();
    
    for (int i = a; i <= b; i++)
    {
        NFA* nfa = Create_Primitive_NFA(i);
        List_Append(nfas, nfa);
    }
    return Create_Parallel_NFA_From_List(nfas);
}

// TODO
NFA* Create_Plus_NFA(NFA* nfa) {
    return nfa;
}