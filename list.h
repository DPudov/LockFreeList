#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct node node_t;
struct node
{
    void *data;
    node_t *next;
};

typedef struct
{
    node_t *head;
    node_t *tail;
    int size;
} lock_free_list_t;

int init_list(lock_free_list_t **list);

void free_list(lock_free_list_t *list);

void insert(node_t **head, node_t *elem, node_t *before);

node_t *find(node_t *head, const void *data,
    int (*comparator)(const void *, const void *));

void *pop_front(node_t **head);

node_t *reverse(node_t *head);

void front_back_split(node_t *head, node_t **back);

node_t *sorted_merge(node_t **head_a, node_t **head_b,
    int (*comparator)(const void *, const void *));

node_t *sort(node_t *head, int (*comparator)(const void *, const void *));

int compare_nodes(const void *, const void *);

int compare_and_swap(node_t **addr_node, node_t *old_node, node_t *new_node);

typedef struct
{
    int key;
    unsigned int number;
    char *lab_name;
    size_t lab_name_allocated;
    char *lab_desc;
    size_t lab_desc_allocated;
} data_t;

data_t *create_data(unsigned int number, char *lab_name,
    size_t lab_name_allocated, char *lab_desc, size_t lab_desc_allocated);

node_t *create_element(int key, data_t *d);

void print_list(int num, node_t *head);

void print_data(data_t *data);

#endif
