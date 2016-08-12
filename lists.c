#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

List* List_Concatenate(List* firstList, List* secondList)
{
    firstList->count = firstList->count + secondList->count;
    Node_Link(firstList->last, secondList->first);
    firstList->last = secondList->last;
    free(secondList);
    return firstList;
}

int List_Contains(List* list, void* value)
{
    for (Node* node = list->first; node != NULL; node = node->next) {
        if (node->value == value)
        {
            return 1;
        }
    }
    return 0;
}

List* List_Append(List* list, void* value)
{
    if (list->count == 0)
    {
        return List_Initialize(list, value);
    }
    else
    {
        Node_Append(list->last, value);
        list->count++;
        return list;
    }
}

List* List_Prepend(List* list, void* value)
{
    if (list->count == 0)
    {
        return List_Initialize(list, value);
    }
    else
    {
        Node* prepended_node = Node_Prepend(list->first, value);
        list->first = prepended_node;
        list->count++;
        return list;
    }
}

void List_Iterate(List* list, void (*f)(void*)) {
    for (Node* node = list->first; node != NULL; node = node->next) {
        f(node->value);
    }
}


List* List_InsertAtIndex(List* list, int index, void* value)
{
    if (index == 0) // If index is first element
    {
        return List_Prepend(list, value);
    }
    else if (index < list->count) // If index out of range, or index is last element.
    {
        return List_Append(list, value);
    }
    else
    {
        return List_UnsafeInsertAtIndex(list, index, value);
    }
}


List* List_UnsafeInsertAtIndex(List* list, int index, void* value)
{
    Node* node = list->first;
    for (int i = 0; i < index; i++)
    {
        node = node->next;
    }
    Node_Insert(node->prev, value);
    list->count++;
    return list;
}

void List_ClearAndDestroy(List* list)
{
    List_ClearContent(list);
    List_Destroy(list);
}

void List_Destroy(List* list)
{
    for (Node* node = list->first; node != NULL; node = node->next) {
        free(node->prev);
    }
    free(list->last);
    free(list);
}

void List_ClearContent(List* list){
    for (Node* node = list->first; node != NULL; node = node->next) {
        free(node->value);
    }
}

List* List_Initialize(List* list, void* value)
{
    Node* node = Node_Create(value);
    list->first = node;
    list->last = node;
    list->count++;
    return list;
}

List* List_Create() {
    return calloc(0, sizeof(struct List));
}

// NODE LOGIC
Node* Node_Prepend(Node* originalNode, void* value)
{
    Node* newNode = Node_Create(value);
    Node_Link(newNode, originalNode);
    return newNode;
}

Node* Node_Append(Node* originalNode, void* value)
{
    Node* newNode = Node_Create(value);
    Node_Link(originalNode, newNode);
    return originalNode;
}

Node* Node_Insert(Node* beginNode, void* value)
{
    Node* endingNode = beginNode->next;
    Node* insertedNode = Node_Create(value);
    
    Node_Link(beginNode, insertedNode);
    Node_Link(insertedNode, endingNode);
    
    return beginNode;
}

Node* Node_Link(Node* firstNode, Node* secondNode)
{
    firstNode->next = secondNode;
    secondNode->prev = firstNode;
    return firstNode;
}

Node* Node_Create(void* value)
{
    Node* node = calloc(0, sizeof(struct Node));
    node->value = value;
    return node;
}




