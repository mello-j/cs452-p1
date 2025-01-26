#include "lab.h"

static const int INVALID_INDEX = -1;

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *))
{
    // Allocate memory for the list
    list_t *list = malloc(sizeof(list_t));

    // Check if the list was allocated
    if (!list)
    {
        return NULL;
    }

    // Allocate memory for the head node
    list->head = malloc(sizeof(node_t));

    // Check if the head node was allocated
    if (!list->head)
    {
        free(list);
        return NULL;
    }

    // Set the head node to point to itself
    list->head->next = list->head;
    list->head->prev = list->head;
    list->head->data = NULL;

    // Set the size of the list to 0
    list->size = 0;

    // Set the function pointers
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;

    return list;
}

void list_destroy(list_t **list)
{
    if (!list || !(*list))
    {
        return;
    }

    list_t *currentList = *list;
    node_t *currentNode = currentList->head->next;

    // Free all the nodes in the list
    while (currentNode != currentList->head)
    {
        currentList->destroy_data(currentNode->data);
        node_t *tempNode = currentNode;
        currentNode = currentNode->next;
        free(tempNode);
    }

    // free the head node and the list
    free(currentList->head);
    free(currentList);
    *list = NULL;
}

list_t *list_add(list_t *list, void *data)
{
    // Validate Data
    if (!list || !list->head)
    {
        return NULL;
    }

    // allocate new node
    node_t *newNode = malloc(sizeof(node_t));

    // set the data of the new node
    newNode->data = data;

    // set the next and prev pointers of the new node
    newNode->next = list->head->next;
    newNode->prev = list->head;

    // set the prev pointer of the node after the head to the new node
    list->head->next->prev = newNode;

    // set the next pointer of the head to the new node
    list->head->next = newNode;
    list->size++;

    return list;
}

void *list_remove_index(list_t *list, size_t index)
{
    if (!list || !list->head || index >= list->size)
    {
        return NULL;
    }

    node_t *currentNode = list->head->next;

    for (size_t i = 0; i < index; i++)
    {
        currentNode = currentNode->next;
    }

    void *data = currentNode->data;
    currentNode->prev->next = currentNode->next;
    currentNode->next->prev = currentNode->prev;
    free(currentNode);
    list->size--;

    return data;
}

int list_indexof(list_t *list, void *data)
{

    // Check for valid list
    if (!list || !list->head)
    {
        return INVALID_INDEX;
    }

    // Grab the head of the list
    node_t *currentNode = list->head->next;
    int index = 0;

    while (currentNode != list->head)
    {
        if (list->compare_to(currentNode->data, data) == 0)
        {
            return index;
        }
        currentNode = currentNode->next;
        index++;
    }

    return INVALID_INDEX;
}