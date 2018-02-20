// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include "SortedList.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// SortedList is actually a circular doubly linked list with dummynode

/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *  The specified element will be inserted in to
 *  the specified list, which will be kept sorted
 *  in ascending order based on associated keys
 *
 * @param SortedList_t *list ... header for the list
 * @param SortedListElement_t *element ... element to be added to the list
 */
void SortedList_insert(SortedList_t* list, SortedListElement_t* element){
    if (list == NULL || list->key != NULL || element == NULL)
        return;

    SortedListElement_t* prev = list;
    SortedListElement_t* next = list->next;

    while (next != list && next->key <= element->key){
        if (opt_yield & INSERT_YIELD)
            sched_yield();
        prev = next;
        next = next->next;
    }

    if (opt_yield & INSERT_YIELD)
        sched_yield();

    element->next = next;
    element->prev = prev;
    next->prev = element;
    prev->next = element;
}

/**
 * SortedList_delete ... remove an element from a sorted list
 *
 *  The specified element will be removed from whatever
 *  list it is currently in.
 *
 *  Before doing the deletion, we check to make sure that
 *  next->prev and prev->next both point to this node
 *
 * @param SortedListElement_t *element ... element to be removed
 *
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int SortedList_delete(SortedListElement_t* element){
    if (element == NULL ||
        element->next == NULL || element->next->prev != element ||
        element->prev == NULL || element->prev->next != element)
        return 1;

    if (opt_yield & DELETE_YIELD)
        sched_yield();

    element->prev->next = element->next;
    element->next->prev = element->prev;

    return 0;
}

/**
 * SortedList_lookup ... search sorted list for a key
 *
 *  The specified list will be searched for an
 *  element with the specified key.
 *
 * @param SortedList_t *list ... header for the list
 * @param const char * key ... the desired key
 *
 * @return pointer to matching element, or NULL if none is found
 */
SortedListElement_t* SortedList_lookup(SortedList_t* list, const char* key){
    if (list == NULL || list->key != NULL)
        return NULL;

    SortedListElement_t* curr = list->next;
    while (curr != list && curr->key != key){
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        curr = curr->next;
    }
    if (curr == list)
        return NULL;
    return curr;
}

/**
 * SortedList_length ... count elements in a sorted list
 *  While enumeratign list, it checks all prev/next pointers
 *
 * @param SortedList_t *list ... header for the list
 *
 * @return int number of elements in list (excluding head)
 *     -1 if the list is corrupted
 */
int SortedList_length(SortedList_t* list){
    if (list == NULL || list->key != NULL)
        return -1;

    SortedListElement_t* curr = list->next;

    int count = 0;
    while (curr != list){
        count++;
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        
        curr = curr->next;
    }

    return count;
}

/*
int main(){ 
    // Init
    SortedList_t list;
    list.key = NULL;
    list.next = &list;
    list.prev = &list;

    SortedListElement_t element1;
    element1.key = "1";
    SortedListElement_t element2;
    element2.key = "2";
    SortedListElement_t element3;
    element3.key = "3";

    // Insert
    SortedList_insert(&list, &element2);
    SortedList_insert(&list, &element1);
    SortedList_insert(&list, &element3);

    // Print
    SortedListElement_t* tmp = list.next;
    while (tmp != &list){
        printf("%s\n", tmp->key);
        tmp = tmp->next;
    }
    printf("%d\n", SortedList_length(&list));

    // Delete
    const char* key_tmp = "1";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);
    key_tmp = "2";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);
    key_tmp = "3";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);

    // Print
    tmp = list.next;
    while (tmp != &list){
        printf("%s\n", tmp->key);
        tmp = tmp->next;
    }
    printf("%d\n", SortedList_length(&list));

    // Insert
    SortedList_insert(&list, &element3);
    SortedList_insert(&list, &element2);
    SortedList_insert(&list, &element1);

    // Print
    tmp = list.next;
    while (tmp != &list){
        printf("%s\n", tmp->key);
        tmp = tmp->next;
    }
    printf("%d\n", SortedList_length(&list));

    // Delete
    key_tmp = "2";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);
    key_tmp = "1";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);
    key_tmp = "3";
    tmp = SortedList_lookup(&list, key_tmp);
    SortedList_delete(tmp);

    // Print
    tmp = list.next;
    while (tmp != &list){
        printf("%s\n", tmp->key);
        tmp = tmp->next;
    }
    printf("%d\n", SortedList_length(&list));
} 
*/
