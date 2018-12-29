#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "list.h"
#include <stdio.h>

void print_addresses(lock_free_list_t *list);

void print_summary(lock_free_list_t *list);

#endif
