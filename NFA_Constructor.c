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
    Edge* edge = malloc(sizeof(Edge));
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
    NFA* nfa = malloc(sizeof(struct NFA));
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

// Creates a match multiple for each subexpression. Used in []
NFA* evaluate_multiple_choice(Expression exp) { //TODO: change Expression type name to RegEx
    List* ls = List_Create();

    while (peek(exp) != '\0') {
        if (next(exp) == '-') {
            char c1 = consume(&exp);
            exp++;
            char c2 = consume(&exp);
            List_Append(ls, Create_Range_NFA(c1, c2));
        } else {
            List_Append(ls, Create_Primitive_NFA(consume(&exp)));
        }
    }
    return Create_Parallel_NFA_From_List(ls);
}

NFA* evaluate_bracketed_expression(Expression exp)
{
    char* newexp = malloc(strlen(exp));
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
        
        char* str = make_substring(exp, 1, i);
        
        if (next(newexp) == '*') {
            char* str2 = make_substring(exp, i+2, strlen(exp));
            struct NFA* nfa1 = kleenstar_nfa(evaluate(str));
            return concatenate_nfa(nfa1, evaluate(str2));
        }
        
        if (next(newexp) != '\0') {
            char* str2 = make_substring(exp, i+1, strlen(exp));
            struct NFA* nfa1 = evaluate(str);
            return concatenate_nfa(nfa1, evaluate(str2));
        }
        return evaluate(str);
}

// Main evaluation loop
struct NFA* evaluate (Expression exp) {
    char* newexp = malloc(strlen(exp));
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

        char* str = make_substring(exp, 1, i);

        if (next(newexp) == '*') {
            char* str2 = make_substring(exp, i+2, strlen(exp));
            struct NFA* nfa1 = kleenstar_nfa(evaluate(str));
            return concatenate_nfa(nfa1, evaluate(str2));
        }

        if (next(newexp) != '\0') {
            char* str2 = make_substring(exp, i+1, strlen(exp));
            struct NFA* nfa1 = evaluate(str);
            return concatenate_nfa(nfa1, evaluate(str2));
        }
        return evaluate(str);
    }
    // Check for union
    for (int i = 0; i < strlen(exp); i++) {
        if (peek(newexp) == '|') {
            char* str = make_substring(exp, 0, i);
            char* str2 = make_substring(exp, i+1, strlen(exp));
            struct NFA* nfa1 = evaluate(str);
            return make_union(nfa1, evaluate(str2));
        } else {
            newexp++;
        }
    }
    newexp = malloc(strlen(exp));
    strcpy(newexp, exp);

    // Check for multiple choice. e.g. [abc]
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
            struct NFA* nfa1 = kleenstar_nfa(evaluate_multiple_choice(str));
            return concatenate_nfa(nfa1, evaluate(str2));
        }


        if (next(newexp) != '\0') {
            char* str2 = make_substring(exp, i+1, strlen(exp));
            struct NFA* nfa1 = evaluate_multiple_choice(str);
            return concatenate_nfa(nfa1, evaluate(str2));
        }

        return evaluate_multiple_choice(str);
    }

    // Checks if end of expression.
    if (peek(exp) == '\0') return NULL;

    // Checks if expression is range.
    if (next(exp) == '-') {
        char c1 = consume(&exp);
        exp++;
        char c2 = consume(&exp);
        if (next(exp) == '*') { // If followed by kleenstar
            struct NFA* nfa1 = kleenstar_nfa(create_range(c1, c2));
            struct NFA* nfa2 = evaluate(exp);
            return concatenate_nfa(kleenstar_nfa(create_range(c1, c2)), evaluate(exp));
        }
        return concatenate_nfa(create_range(c1, c2), evaluate(exp));
    }

    // Checks if the expression is a kleenstar
    if (next(exp) == '*') {
        struct NFA* nfa1 = kleenstar_nfa(make_primitive(consume(&exp)));
        return  concatenate_nfa(nfa1, evaluate(exp));
    }


    char c = consume(&exp);
    // If all else fails, it's just a primitive.
    struct NFA* primitive = make_primitive(c);

    return concatenate_nfa(primitive, evaluate(exp));

}
