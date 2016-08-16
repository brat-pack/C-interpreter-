//
// Created by Moro on 23/05/16.
//

#ifndef C_INTERPRETER_MAIN_H
#define C_INTERPRETER_MAIN_H
#include "lists.h"

int state_count;
typedef char* Expression;
enum bool {true = 1, false = 0};
struct NFA* evaluate(Expression);

typedef struct State {
    List* edges; // List with all edges connecting to other states
    int number; // Unique state ID.
    int is_final; // Bool that checks whether the state is a final state
} State;

typedef struct Edge {
    State* state; // State we connect to.
    char c; // Character for this transition.
} Edge;

typedef struct NFA {
    State* start; // State that the NFA starts with.
    State* end; // State that the NFA ends with.
} NFA;

NFA* Concatenate_NFA(struct NFA* NFA_1, struct NFA* NFA_2);
NFA* Create_Union_NFA(NFA* NFA_1, NFA* NFA_2);
NFA* Create_Kleenstar_NFA(NFA* nfa);
NFA* Create_Plus_NFA(NFA* nfa);
NFA* Create_Primitive_NFA(char c);

#endif //C_INTERPRETER_MAIN_H
