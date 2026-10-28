#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/* * Represents a single node in the linked list.
 * It contains a void pointer to the data and a pointer to the next node.
 */
typedef struct LinkedListNode{
    void* data;
    struct LinkedListNode* next;
} LinkedListNode;

/*
 * Represents the linked list as a whole.
 * It contains a pointer to the first node (head) and a count of items.
 */
typedef struct{
    LinkedListNode* head;
    int count;
} LinkedList;

/* Creates and returns a new, empty linked list. */
LinkedList* createLinkedList();

/* Inserts a new piece of data at the beginning (head) of the list. */
void insertFirst(LinkedList* list, void* data);

/* Removes and returns the first piece of data from the list. Returns NULL if empty. */
void* removeFirst(LinkedList* list);

/* * Frees all memory used by the list, including the nodes and the data they hold.
 * It requires a function pointer to know how to free the specific data type.
 */
void freeLinkedList(LinkedList* list, void (*freeData)(void*));


#endif


