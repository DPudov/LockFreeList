#include "menu.h"
#include "reader.h"
#include "types.h"

int menu(void)
{
    int menu = -1;
    node_t *head = NULL;
    do
    {
        printf("Which operation do you want to execute?\n"
            "1. Append new node\n"
            "2. Search for node\n"
            "3. Print sorted list\n"
            "4. Delete first node\n"
            "5. Test lock-free list\n"
            "0. Exit\n");

        menu = read_option();
        switch (menu)
        {
            case OPTION_EXIT:
                //do finish
                return SUCCESS;

            case OPTION_APPEND:
                break;

            case OPTION_SORTED:
                break;

            case OPTION_DELETE:
                break;

            case OPTION_LOCKFR:
                break;

            case OPTION_SEARCH:
                break;
        }
    }
    while (menu != 0);
    return SUCCESS;
}
