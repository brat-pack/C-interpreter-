#ifndef C_INTERPRETER_LISTS_H
#define C_INTERPRETER_LISTS_H


struct List {
    void* head;
    struct List* tail;
};

// Generic stuff for lists
struct List* list_create(void* h, struct List* tail);
void list_clear(struct List* list);
int list_length(struct List* list);
struct List* list_append(struct List* list, void* value);
int list_contains(struct List* list, void* val);
struct List* list_concatenate(struct List* ls1, struct List* ls2);

// Functional stuff
void list_iterate(struct List* list, void (*f)(void*));
struct List* list_map(struct List* list, void* (*f)(void*));
void* list_fold(struct List* list, void* (*f)(void*, void*),void* acc);
struct List* list_filter(struct List* list, int (*f)(void*));

// Legacy functions
// TODO: Remove after dependency is gone.
struct List* list_converter(struct List* list);
#endif //C_INTERPRETER_LISTS_H
