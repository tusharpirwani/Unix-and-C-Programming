#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

/*
 * Allocates memory for a new LinkedList struct and initializes its values.
 */
LinkedList* createLinkedList()
{
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL; /* The list is initially empty */
    list->count = 0;
    return list;
}

/*
 * Creates a new node to hold the data and adds it to the front of the list.
 */
void insertFirst(LinkedList* list, void* data)

{
     /* Allocate memory for the new node */
    LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    newNode->data = data;
     /* The new node's 'next' should point to the old head of the list */
    newNode->next = list->head;

    /* The list's head should now be the new node */
    list->head = newNode;
    list->count++;
}

/*
 * Removes the node at the head of the list and returns its data.
 */
void* removeFirst(LinkedList* list)
{
    void* data = NULL;
    LinkedListNode* tempNode = NULL;
    
    /* Only proceed if the list is not empty */
    if (list->head != NULL)
    {   
         /* Temporarily store the head node */
        tempNode = list->head;
        /* Retrieve the data from that node */
        data = tempNode->data;
        /* Point the list's head to the second node in the list */
        list->head = tempNode->next;

         /* Free the memory of the old head node (but not its data) */
        free(tempNode);
        list->count--;
    }

    /* Return the data that was in the removed node */
    return data;
}

/* * Iterates through the entire list, freeing each node and the data it contains.
 * It uses a provided function pointer to free the data itself.
 */
void freeLinkedList(LinkedList* list, void (*freeData)(void*))
{
    LinkedListNode *node, *nextNode;
    node = list->head;

    /* Loop until we run out of nodes */
    while (node != NULL)
    {
        nextNode = node->next; /* Save a pointer to the next node before we free the current one */
        (*freeData)(node->data); /* Call the provided function (e.g., freeGameState) to free the specific data */
        free(node); /* Free the current node struct itself */
        node = nextNode;  /* Move to the next node */
    }

    /* Finally, free the LinkedList struct itself */
    free(list);
}