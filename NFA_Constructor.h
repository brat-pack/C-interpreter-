//
// Created by Moro on 23/05/16.
//

#ifndef C_INTERPRETER_MAIN_H
#define C_INTERPRETER_MAIN_H

int state_count;
typedef char* Expression;
enum bool {true = 1, false = 0};
struct NFA* evaluate(Expression);

struct State {
    struct List* edges; // List with all edges connecting to other states
    int number; // Unique state ID.
    int is_final; // Bool that checks whether the state is a final state
};

struct Edge {
    struct State* state; // State we connect to.
    char c; // Character for this transition.
};

struct NFA {
    struct State* start; // State that the NFA starts with.
    struct State* end; // State that the NFA ends with.
};

#endif //C_INTERPRETER_MAIN_H
