#include "list.h"
#include "types.h"

int compare_nodes(const void *a, const void *b)
{
    data_t *node_a = (data_t *) a;
    data_t *node_b = (data_t *) b;
    return node_a->key - node_b->key;
}

int init_list(lock_free_list_t **list)
{
    if (list == NULL)
    {
        return ERR_INTERNAL;
    }

    *list = (lock_free_list_t *) malloc(sizeof(lock_free_list_t));
    if (*list == NULL)
    {
        return ERR_MEMORY_ALLOCATION;
    }

    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->size = 0;
    return SUCCESS;
}

void free_list(lock_free_list_t *list)
{
    if (list == NULL)
    {
        return;
    }

    node_t *iterator = list->head;
    if (iterator == NULL)
    {
        free(list);
        return;
    }

    node_t *tmp = NULL;
    while ((tmp = iterator->next) != NULL)
    {
        free(iterator->data);
        free(iterator);
        iterator = tmp;
    }

    free(list);
}

data_t *create_data(unsigned int number, char *lab_name,
    size_t lab_name_allocated, char *lab_desc, size_t lab_desc_allocated)
{
    if (lab_name == NULL || lab_desc == NULL)
    {
        return NULL;
    }

    data_t *result = malloc(sizeof(data_t));
    if (result == NULL)
    {
        return NULL;
    }

    result->number = number;
    result->lab_name = lab_name;
    result->lab_name_allocated = lab_name_allocated;
    result->lab_desc = lab_desc;
    result->lab_desc_allocated = lab_desc_allocated;

    return result;
}

node_t *create_element(int key, data_t *d)
{
    if (d == NULL)
    {
        return NULL;
    }

    node_t *result = malloc(sizeof(node_t));
    if (result == NULL)
    {
        return NULL;
    }

    result->data = d;
    return result;
}

node_t *reverse(node_t *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    node_t *next = head->next;
    if (next == NULL)
    {
        return head;
    }

    node_t *prev = NULL;
    node_t *cur = head;
    while (cur != NULL)
    {
        node_t *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    head = prev;
    return head;
}

// Блокирующая и потоконебезопасная реализация
#ifndef UNLOCKED
void insert(node_t **head, node_t *elem, node_t *before)
{
    if (head == NULL || elem == NULL || before == NULL)
    {
        return;
    }

    if (*head == NULL)
    {
        return;
    }

    if (before == *head)
    {
        elem->next = *head;
        *head = elem;
        return;
    }

    node_t *iterator = *head;
    while (iterator->next != NULL && iterator->next != before)
    {
        iterator = iterator->next;
    }

    if (iterator->next == NULL)
    {
        return;
    }

    elem->next = before;
    iterator->next = elem;
}
#endif

#ifdef UNLOCKED
void insert(node_t **head, node_t *elem, node_t *before)
{
    if (head == NULL || elem == NULL || before == NULL)
    {
        return;
    }

    if (*head == NULL)
    {
        return;
    }

    if (before == *head)
    {
        node_t *next_expected = *head;
        node_t *old_value = elem->next;

        while (compare_and_swap(&(elem->next), old_value, next_expected) == FALSE)
        {
            ;
        }

        node_t *new_head = elem;
        node_t *old_head = *head;
        while (compare_and_swap(head, old_head, new_head) == FALSE)
        {
            ;
        }
        return;
    }

    node_t *iterator = *head;
    while (iterator->next != NULL && iterator->next != before)
    {
        iterator = iterator->next;
    }

    if (iterator->next == NULL)
    {
        return;
    }

    node_t *next_expected = before;
    node_t *old_value = elem->next;

    while (compare_and_swap(&(elem->next), old_value, next_expected) == FALSE)
    {
        ;
    }

    next_expected = elem;
    old_value = iterator->next;
    while (compare_and_swap(&(iterator->next), old_value, next_expected) == FALSE)
    {
        ;
    }
}
#endif

void *pop_front(node_t **head)
{
    if (head == NULL)
    {
        return NULL;
    }

    if (*head == NULL)
    {
        return NULL;
    }

    void *result = (*head)->data;

    node_t *tmp = (*head)->next;
    free(*head);

    *head = tmp;

    return result;
}

node_t *find(node_t *head, const void *data,
    int (*comparator)(const void *, const void *))
{
    if (head == NULL || data == NULL || comparator == NULL)
    {
        return NULL;
    }

    node_t *iterator = head;
    while (iterator != NULL)
    {
        if (comparator(data, iterator->data) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }

    return NULL;
}

void front_back_split(node_t *head, node_t **back)
{
    if (head == NULL)
    {
        return;
    }

    if (back == NULL)
    {
        return;
    }

    if (head->next == NULL)
    {
        *back = NULL;
        return;
    }

    int nodes_count = 0;
    node_t *iterator = head;
    while (iterator != NULL)
    {
        nodes_count++;
        iterator = iterator->next;
    }

    iterator = head;
    int i = 0;
    node_t *tmp = head;
    while (i < (nodes_count - 1) / 2)
    {
        if (i == (nodes_count - 1) / 2 - 1)
        {
            tmp = iterator->next;
        }

        iterator = iterator->next;
        i++;
    }

    *back = iterator->next;
    tmp->next = NULL;
}

node_t *sorted_merge(node_t **head_a, node_t **head_b,
    int (*comparator)(const void *, const void *))
{
    if (head_a == NULL || head_b == NULL || comparator == NULL)
    {
        return NULL;
    }

    node_t *result = NULL;
    if (*head_a == NULL)
    {
        result = *head_b;
        *head_b = NULL;
        return result;
    }
    else if (*head_b == NULL)
    {
        result = *head_a;
        *head_a = NULL;
        return result;
    }

    if (comparator((*head_a)->data, (*head_b)->data) <= 0)
    {
        result = *head_a;
        result->next = sorted_merge(&((*head_a)->next), head_b, comparator);
    }
    else
    {
        result = *head_b;
        result->next = sorted_merge(head_a, &((*head_b)->next), comparator);
    }

    *head_a = NULL;
    *head_b = NULL;
    return result;
}

node_t *sort(node_t *head, int (*comparator)(const void *, const void *))
{
    if (head == NULL || comparator == NULL)
    {
        return NULL;
    }

    if (head->next == NULL)
    {
        return head;
    }

    node_t *back = NULL;
    front_back_split(head, &back);

    head = sort(head, comparator);
    back = sort(back, comparator);

    node_t *result = sorted_merge(&head, &back, comparator);
    return result;
}

void print_data(data_t *data)
{
    if (data == NULL)
    {
        return;
    }

    printf("KEY: %d\n", data->key);
    printf("LAB NAME: %s\n", data->lab_name);
    printf("LAB DESCRIPTION: %s\n", data->lab_desc);
}

void print_list(int num, node_t *head)
{
    if (head == NULL)
    {
        return;
    }

    printf("LIST\n");
    node_t *iterator = head;
    int i = 0;
    while (iterator != NULL)
    {
        printf("Node %d points to address %p\n", num, (void *) iterator);
        print_data((data_t *) iterator->data);
        iterator = iterator->next;
        i++;
    }
}
