//
// Created by Moro on 23/05/16.
//

#ifndef C_INTERPRETER_MAIN_H
#define C_INTERPRETER_MAIN_H

struct State;
struct Edge;
struct List;
typedef char* Expression;
struct NFA* evaluate(Expression);

struct State {
    struct List* edges;
    int number;
    int is_final;
};

struct Edge {
    struct State* state;
    char c;
};

struct NFA {
    struct State* start;
    struct State* end;
};

struct List {
    struct Edge* head;
    struct List* tail;
};

struct StateList {
    struct State* head;
    struct StateList* tail;
};

struct NFAList {
    struct NFA* head;
    struct NFAList* tail;
};
#endif //C_INTERPRETER_MAIN_H
