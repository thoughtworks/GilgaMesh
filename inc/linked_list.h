#pragma once

#include <stdint.h>
#include <stdbool.h>


typedef struct
{
  uint8_t *item;
  uint8_t *previousListItem;
  uint8_t *nextListItem;
}__attribute__ ((packed)) listItem;

typedef struct
{
  uint16_t count;
  uint16_t maxSize;
  listItem *first;
  listItem *last;
}__attribute__ ((packed)) linkedList;


bool add_item_to_list(linkedList *list, uint8_t *item);
bool remove_item_from_list(linkedList *list, uint8_t *item, uint16_t itemSize);
