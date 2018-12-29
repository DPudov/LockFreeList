#include "types.h"
#include "list.h"

int compare_and_swap(node_t **addr_node, node_t *old_node, node_t *new_node)
{
    if (*addr_node != old_node)
    {
        return FALSE;
    }

    *addr_node = new_node;
    return TRUE;
}
