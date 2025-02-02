#include "lab.h"

/**
 * Create a new list with callbacks for data handling
 * 
 * @param destroy_data Function that frees list data
 * @param compare_to Function that compares list elements
 * @return list_t* Pointer to the newly allocated list or NULL if allocation failed
 */
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *))
{
    //Evaluate for null callbacks.
    if (!destroy_data || !compare_to)
    {
        return NULL;
    }
    
    // Allocate memory for the list
    list_t *list = (list_t *)malloc(sizeof(list_t));

    // Check if the list was allocated
    if (!list) {
        return NULL;
    }

    // Allocate memory for the head node
    list->head = (node_t *)malloc(sizeof(node_t));

    // Check if the head node was allocated
    if (!list->head) {
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

/**
 * Destroy the list and all associated data
 * 
 * @param list Pointer to the list that needs to be destroyed
 */
void list_destroy(list_t **list)
{
    // Validate the list
    if (!list || !(*list) || !(*list)->head) {
        return;
    }

    // Grab first node in the list
    node_t *current_node = (*list)->head->next;

    // Free all the nodes in the list
    while (current_node != (*list)->head) {
        node_t *next_node = current_node->next;

        //check for destroy data and check for null data (great tip from the code-review.)
        if ((*list)->destroy_data && current_node->data) {
            (*list)->destroy_data(current_node->data);
        }
        free(current_node);
        current_node = next_node;
    }

    // Free the head node and the list
    free((*list)->head);
    free(*list);
    *list = NULL;
}

/**
 * Adds data to the front of the list
 * 
 * @param list Pointer to an existing list
 * @param data The data to add
 * @return A pointer to the list
 */
list_t *list_add(list_t *list, void *data)
{
    // Validate Data
    if (!list || !list->head) {
        return NULL;
    }

    // Allocate new node
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    // Check if the new node was allocated
    if (!new_node) {
        return NULL;
    }

    // Set the data of the new node
    new_node->data = data;

    // Set the next and prev pointers of the new node
    new_node->next = list->head->next;
    new_node->prev = list->head;

    // Set the prev pointer of the node after the head to the new node
    list->head->next->prev = new_node;

    // Set the next pointer of the head to the new node
    list->head->next = new_node;
    list->size++;

    return list;
}

/**
 * Removes the data at the specified index. If index is invalid
 * then this function does nothing and returns NULL
 * 
 * @param list Pointer to an existing list
 * @param index The index of the data to remove
 * @return void* The data that was removed or NULL if nothing was removed
 */
void *list_remove_index(list_t *list, size_t index)
{
    if (!list || !list->head || index >= list->size) {
        return NULL;
    }

    node_t *current_node = list->head->next;

    for (size_t i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    void *data = current_node->data;

    // Remove the node from the list
    current_node->prev->next = current_node->next;
    current_node->next->prev = current_node->prev;

    // Free the node
    free(current_node);

    // Decrement the size of the list
    list->size--;

    return data;
}

/**
 * Returns the index of the data in the list. 
 * If the data is not in the list then -1 is returned
 * 
 * @param list Pointer to an existing list
 * @param data The data to search for
 * @return The index of the item if found or -1 if not
 */
int list_indexof(list_t *list, void *data)
{
    // Check for valid list
    if (!list || !list->head) {
        return -1;
    }

    // Start at first data node
    node_t *current_node = list->head->next;
    int index = 0;

    while (current_node != list->head) {
        if (list->compare_to(current_node->data, data) == 0) {
            return index;
        }
        current_node = current_node->next;
        index++;
    }

    return -1;
}
