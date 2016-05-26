//
// Created by Moro on 13/05/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "reader.h"
#include "lists.h"

int state_count = 0;

struct State* make_state() {
    struct State* state = malloc(sizeof(struct State));
    state->number = state_count;
    state_count++;
    state->is_final = 0;
    state->edges = NULL;
    return state;
};

struct Edge* make_edge(struct State* state, char c) {
    struct Edge* edge = malloc(sizeof(struct Edge));
    edge->state = state;
    edge->c = c;
    return edge;
};

void link_states(struct State* s1, struct State* s2, char c) {
    struct Edge* new_edge = make_edge(s2, c);
    s1->edges = list_append(s1->edges, new_edge);
}

void link_states_epsilon(struct State* s1, struct State* s2) {
    link_states(s1, s2, '\0');
}

struct NFA* make_nfa(struct State* s1, struct State* s2) {
    struct NFA* nfa = malloc(sizeof(struct NFA));
    nfa->start = s1;
    nfa->end = s2;
    return nfa;
}

struct NFA* make_primitive(char c) {
    struct State* s1 = make_state();
    struct State* s2 = make_state();
    link_states(s1, s2, c);
    struct NFA* nfa = make_nfa(s1, s2);
    return nfa;
}

struct NFA* concatenate_nfa(struct NFA* nfa1, struct NFA* nfa2) {
    if (nfa2 == NULL) return nfa1;
    free(nfa1->end->edges);
    nfa1->end->edges = nfa2->start->edges;
    free(nfa2->start);
    struct NFA* concatenated_nfa = make_nfa(nfa1->start, nfa2->end);
    free(nfa1);
    free(nfa2);
    return concatenated_nfa;
};

struct NFA* make_union(struct NFA* nfa1, struct NFA* nfa2) {
    struct State* start_state = make_state();
    struct State* end_state = make_state();

    link_states_epsilon(nfa1->end, end_state);
    link_states_epsilon(nfa2->end, end_state);
    link_states_epsilon(start_state, nfa1->start);
    link_states_epsilon(start_stat
    e, nfa2->start);

    free(nfa1);
    free(nfa2);

    return make_nfa(start_state, end_state);
}

struct NFA* kleenstar_nfa(struct NFA* nfa) {
    struct State* start_state = make_state();
    struct State* end_state = make_state();

    link_states_epsilon(start_state, end_state);
    link_states_epsilon(start_state, nfa->start);
    link_states_epsilon(nfa->end, end_state);
    link_states_epsilon(nfa->end, nfa->start);

    free(nfa);

    return make_nfa(start_state, end_state);
}

struct NFA* make_match_multiple(struct NFAList* nfas, int list_length) {
    struct State* start_state = make_state();
    struct State* end_state = make_state();
    struct NFAList* ls = nfas;

    for (int i = 0; i < list_length; i++) {
        struct NFA* nfa = nfas->head;

        link_states_epsilon(start_state, nfa->start);
        link_states_epsilon(nfa->end, end_state);
        struct NFAList* x = nfas;
        free(x->head);
        nfas = nfas->tail;
    }
    return make_nfa(start_state, end_state);
}

struct NFA* create_range(int a, int b) {
    struct NFAList* ls = NULL;
    int i = 0;

    for (a; a <= b; a++) {
        struct NFA* nfa = make_primitive(a);
        ls = list_nfa_append(ls, nfa);
        i++;
    }
    return make_match_multiple(ls, i);
}

struct NFA* evaluate_multiple_choice(Expression exp) {
    struct NFAList* ls = NULL;
    int i = 0;

    while (peek(exp) != '\0') {
        i++;
        if (next(exp) == '-') {
            char c1 = consume(&exp);
            exp++;
            char c2 = consume(&exp);
            ls = list_nfa_append(ls, create_range(c1, c2));
        } else {
            ls = list_nfa_append(ls, make_primitive(consume(&exp)));
        }
    }
    return make_match_multiple(ls, i);
}

struct NFA* evaluate (Expression exp) {
    char* newexp = malloc(strlen(exp));
    strcpy(newexp, exp);

    // Check for union
    for (int i = 0; i < strlen(exp); i++) {
        if (peek(newexp) == '|') {
            char* str = make_substring(exp, 0, i);
            char* str2 = make_substring(exp, i+1, strlen(exp));
            return make_union(evaluate(str), evaluate(str2));
        } else {
            newexp++;
        }
    }
    newexp = malloc(strlen(exp));
    strcpy(newexp, exp);

    // Check for brackets
    if (peek(exp) == '(') {
        char* newexp = malloc(strlen(exp));
        strcpy(newexp, exp);
        int bracket_count = 0;
        int i = 0;
        while ((peek(newexp) != ')' && peek(newexp) != '\0') || bracket_count > 1) {
            if (peek(newexp) == '(') bracket_count++;
            if (peek(newexp) == ')') bracket_count--;
            newexp++;
            i++;
        }

        char* str = make_substring(exp, 1, i-1);

        if (next(newexp) == '*') {
            char* str2 = make_substring(exp, i+2, strlen(exp));
            return concatenate_nfa(kleenstar_nfa(evaluate(str)), evaluate(str2));
        }

        if (next(newexp) != '\0') {
            char* str2 = make_substring(exp, i+1, strlen(exp));
            return concatenate_nfa(evaluate(str), evaluate(str2));
        }
        return evaluate(str);
    }

    if (peek(exp) == '[') {
        char* newexp = malloc(strlen(exp));
        strcpy(newexp, exp);
        int i = 0;
        while (peek(newexp) != ']' && peek(newexp) != '\0') {
            newexp++;
            i++;
        }

        char* str = make_substring(exp, 1, i);
        if (next(newexp) == '*') {
            char* str2 = make_substring(exp, i+2, strlen(exp));
            return concatenate_nfa(kleenstar_nfa(evaluate_multiple_choice(str)), evaluate(str2));
        }


        if (next(newexp) != '\0') {
            char* str2 = make_substring(exp, i+1, strlen(exp));
            return concatenate_nfa(evaluate_multiple_choice(str), evaluate(str2));
        }

        return evaluate_multiple_choice(str);
    }

    if (peek(exp) == '\0') return NULL;

    if (next(exp) == '-') {
        char c1 = consume(&exp);
        exp++;
        char c2 = consume(&exp);
        if (next(exp) == '*') {
            return concatenate_nfa(kleenstar_nfa(create_range(c1, c2)), evaluate(exp));
        }
        return concatenate_nfa(create_range(c1, c2), evaluate(exp));
    }

    if (next(exp) == '*') {
        return  concatenate_nfa(kleenstar_nfa(make_primitive(consume(&exp))), evaluate(exp));
    }

    return  concatenate_nfa(make_primitive(consume(&exp)), evaluate(exp));

}
/*
int main(int argc, char* argv[]) {
    struct NFA* s = evaluate("[a-cd]p*|lol");
    printf("hi");
    return 0;
}*/