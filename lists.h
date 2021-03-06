#ifndef C_INTERPRETER_LISTS_H
#define C_INTERPRETER_LISTS_H

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    void* value;
} Node;

typedef struct List {
    Node* first;
    Node* last;
    int count;
} List;

// Public List Functions
List* List_Create();
void List_Destroy(List* list);
void List_ClearContent(List* list);
void List_ClearAndDestroy(List* list);
List* List_Initialize(List* list, void* value);
List* List_Append(List* list, void* value);
List* List_Prepend(List* list, void* value);
List* List_Remove_At_Index(List* list, int index);
List* List_Remove_First(List* list);
int List_Contains(List* list, void* value);

List* List_InsertAtIndex(List* list, int index, void* value);
List* List_Concatenate(List* firstList, List* secondList);
void List_Iterate(struct List* list, void (*f)(void*));



// Private List Functions
static List* List_UnsafeInsertAtIndex(List* list, int index, void* value);

// Node Functions
Node* Node_Create(void* value);
Node* Node_Prepend(Node* originalNode, void* value);
Node* Node_Append(Node* originalNode, void* value);
Node* Node_Link(Node* firstNode, Node* secondNode);
Node* Node_Insert(Node* beginNode, void* value);
void Node_ClearContent(Node* node);

#define FOREACH(ITEM, LIST) for(Node* ITEM = LIST->first; ITEM != NULL; ITEM = ITEM->next)
#endif //C_INTERPRETER_LISTS_H