//
// Created by Moro on 23/05/16.
//

#ifndef C_INTERPRETER_LISTS_H
#define C_INTERPRETER_LISTS_H


struct GenericList {
    void* head;
    struct GenericList* tail;
};

void iterate_generic_list(struct GenericList* list, void (*f)(void*));
struct GenericList* Concatenate(struct GenericList* ls1, struct GenericList* ls2);
struct GenericList* make_generic_list(void* h, struct GenericList* tail);
struct GenericList* generic_list_append(struct GenericList* list, void* value);
struct StateList* make_state_list(struct State*, struct StateList*);
struct StateList* state_list_append(struct StateList*, struct State*);
struct List* make_list(struct Edge* edge, struct List* tail);
struct NFAList* make_nfa_list(struct NFA* nfa, struct NFAList* tail);
struct List* list_append(struct List* list, struct Edge* edge);
struct NFAList* list_nfa_append(struct NFAList* list, struct NFA* nfa);
void clear_list(struct GenericList* list);
struct GenericList* converter(struct List* list);

#endif //C_INTERPRETER_LISTS_H
