#include "types.h"
#include "list.h"
#include <assert.h>

void test_insert_1(void)
{
    node_t *list = malloc(sizeof(node_t));
    assert(list != NULL);

    node_t *element = malloc(sizeof(node_t));
    assert(element != NULL);

    insert(&list, element, list);
    assert(list == element);

    free(list->next);

    free(list);
}

void test_insert_2(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    list->next = element_1;

    element_1->next = element_2;

    node_t *element_insert = calloc(1, sizeof(node_t));
    assert(element_insert != NULL);

    insert(&list, element_insert, element_2);

    assert(element_1->next == element_insert);
    assert(element_insert->next == element_2);

    free(list->next->next->next);

    free(list->next->next);

    free(list->next);

    free(list);
}

void test_reverse(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    list->next = element_1;

    element_1->next = element_2;

    node_t *tmp = reverse(list);
    assert(tmp == element_2);
    assert(tmp->next = element_1);
    assert(tmp->next->next = list);

    free(tmp->next->next);

    free(tmp->next);

    free(tmp);
}

void test_front_split_0(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *element_3 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_4 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    list->next = element_1;

    element_1->next = element_2;
    element_2->next = element_3;
    element_3->next = element_4;
    element_4->next = NULL;

    node_t *new_node = NULL;
    front_back_split(list, &new_node);

    assert(list->next == element_1);
    assert(element_1->next == element_2);
    assert(element_2->next == NULL);

    assert(new_node == element_3);
    assert(new_node->next == element_4);
    assert(new_node->next->next == NULL);

    free(element_4);

    free(element_3);

    free(element_2);

    free(element_1);

    free(list);
}

void test_front_split_1(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    list->next = element_1;

    element_1->next = element_2;

    node_t *new_node = NULL;
    front_back_split(list, &new_node);

    assert(new_node == element_2);

    free(element_2);

    free(element_1);

    free(list);
}

void test_front_split_2(void)
{
    front_back_split(NULL, NULL);

    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *node = NULL;
    front_back_split(list, &node);

    assert(node == NULL);

    free(list);
}

void test_sort_wrong(void)
{
    assert(sort(NULL, compare_nodes) == NULL);
    assert(sort(NULL, NULL) == NULL);
}

void test_sort(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *element_3 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_4 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    data_t *data_0 = calloc(1, sizeof(data_t));
    assert(data_0 != NULL);

    data_t *data_1 = calloc(1, sizeof(data_t));
    assert(data_1 != NULL);

    data_t *data_2 = calloc(1, sizeof(data_t));
    assert(data_2 != NULL);

    data_t *data_3 = calloc(1, sizeof(data_t));
    assert(data_3 != NULL);

    data_t *data_4 = calloc(1, sizeof(data_t));
    assert(data_4 != NULL);

    data_0->key = 3;
    data_1->key = 1;
    data_2->key = 5;
    data_3->key = 2;
    data_4->key = 4;

    list->data = data_0;
    element_1->data = data_1;
    element_2->data = data_2;
    element_3->data = data_3;
    element_4->data = data_4;

    list->next = element_1;

    element_1->next = element_2;
    element_2->next = element_3;
    element_3->next = element_4;
    element_4->next = NULL;

    node_t *head = sort(list, compare_nodes);

    assert(head == element_1);
    assert(head->next == element_3);
    assert(head->next->next == list);
    assert(head->next->next->next == element_4);
    assert(head->next->next->next->next == element_2);

    free(data_0);
    free(data_1);
    free(data_2);
    free(data_3);
    free(data_4);

    free(element_4);
    free(element_3);
    free(element_2);
    free(element_1);
    free(list);
}

void test_find_wrong()
{
    assert(find(NULL, NULL, NULL) == NULL);
}

void test_find_correct()
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *element_3 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_4 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    data_t *data_0 = calloc(1, sizeof(data_t));
    assert(data_0 != NULL);

    data_t *data_1 = calloc(1, sizeof(data_t));
    assert(data_1 != NULL);

    data_t *data_2 = calloc(1, sizeof(data_t));
    assert(data_2 != NULL);

    data_t *data_3 = calloc(1, sizeof(data_t));
    assert(data_3 != NULL);

    data_t *data_4 = calloc(1, sizeof(data_t));
    assert(data_4 != NULL);

    data_0->key = 3;
    data_1->key = 1;
    data_2->key = 5;
    data_3->key = 2;
    data_4->key = 4;

    list->data = data_0;
    element_1->data = data_1;
    element_2->data = data_2;
    element_3->data = data_3;
    element_4->data = data_4;

    list->next = element_1;

    element_1->next = element_2;
    element_2->next = element_3;
    element_3->next = element_4;
    element_4->next = NULL;

    node_t *f = find(list, data_2, compare_nodes);

    assert(f == element_2);

    free(data_0);
    free(data_1);
    free(data_2);
    free(data_3);
    free(data_4);

    free(element_4);
    free(element_3);
    free(element_2);
    free(element_1);
    free(list);
}

void test_pop_front_wrong(void)
{
    assert(pop_front(NULL) == NULL);

    node_t *list = NULL;
    assert(pop_front(&list) == NULL);
}

void test_pop_front_correct(void)
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *element_3 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_4 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    data_t *data_0 = calloc(1, sizeof(data_t));
    assert(data_0 != NULL);

    data_t *data_1 = calloc(1, sizeof(data_t));
    assert(data_1 != NULL);

    data_t *data_2 = calloc(1, sizeof(data_t));
    assert(data_2 != NULL);

    data_t *data_3 = calloc(1, sizeof(data_t));
    assert(data_3 != NULL);

    data_t *data_4 = calloc(1, sizeof(data_t));
    assert(data_4 != NULL);

    data_0->key = 3;
    data_1->key = 1;
    data_2->key = 5;
    data_3->key = 2;
    data_4->key = 4;

    list->data = data_0;
    element_1->data = data_1;
    element_2->data = data_2;
    element_3->data = data_3;
    element_4->data = data_4;

    list->next = element_1;
    element_1->next = element_2;
    element_2->next = element_3;
    element_3->next = element_4;
    element_4->next = NULL;

    node_t *f = pop_front(&list);
    assert(list == element_1);
    free(f);

    f = pop_front(&list);
    assert(list == element_2);
    free(f);

    f = pop_front(&list);
    assert(list == element_3);
    free(f);

    f = pop_front(&list);
    assert(list == element_4);
    free(f);

    f = pop_front(&list);
    assert(list == NULL);
    free(f);
}

void test_cas()
{
    node_t *list = calloc(1, sizeof(node_t));
    assert(list != NULL);

    node_t *element_1 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_2 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *element_3 = calloc(1, sizeof(node_t));
    assert(element_1 != NULL);

    node_t *element_4 = calloc(1, sizeof(node_t));
    assert(element_2 != NULL);

    node_t *tmp = NULL;

    int rc = compare_and_swap(&tmp, tmp, element_4);
    printf("rc = %d\n", rc);
    assert(tmp == element_4);

    rc = compare_and_swap(&tmp, tmp, element_3);
    printf("rc = %d\n", rc);
    assert(tmp == element_3);

    rc = compare_and_swap(&tmp, tmp, NULL);
    printf("rc = %d\n", rc);
    assert(tmp == NULL);

    free(element_4);
    free(element_3);
    free(element_2);
    free(element_1);
    free(list);
}

int main(void)
{
    test_cas();

    test_insert_1();
    test_insert_2();

    test_reverse();

    test_front_split_0();
    test_front_split_1();
    test_front_split_2();

    test_sort_wrong();
    test_sort();

    test_find_wrong();
    test_find_correct();

    test_pop_front_wrong();
    test_pop_front_correct();

    return SUCCESS;
}
