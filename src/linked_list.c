#include <linked_list.h>
#include <string.h>
#include <stdlib.h>


bool add_item_to_list(linkedList *list, uint8_t *item) {
  if (list->count >= list->maxSize) return false;

  listItem *newItem = malloc(sizeof(listItem));
  memset(newItem, 0, sizeof(listItem));
  newItem->item = item;

  if (list->count == 0) {
    list->first = newItem;
  } else {
    list->last->nextListItem = (uint8_t *) newItem;
    newItem->previousListItem = (uint8_t *) list->last;
  }
  list->last = newItem;
  list->count++;
  return true;
}

void remove_and_free_list_item(linkedList *list, listItem *itemToRemove) {
  listItem *previousItem = (listItem *) itemToRemove->previousListItem;
  listItem *nextItem = (listItem *) itemToRemove->nextListItem;

  if (previousItem != NULL) {
    previousItem->nextListItem = (uint8_t *) nextItem;
  } else {
    list->first = nextItem;
  }
  if (nextItem != NULL) {
    nextItem->previousListItem = (uint8_t *) previousItem;
  } else {
    list->last = previousItem;
  }

  free(itemToRemove);
  list->count--;
}

bool remove_item_from_list(linkedList *list, uint8_t *item, uint16_t itemSize) {
  listItem *itemToCompare = list->first;

  while(itemToCompare != NULL) {
    if (memcmp(itemToCompare->item, item, itemSize) == 0) {
      remove_and_free_list_item(list, itemToCompare);
      return true;
    }
    itemToCompare = (listItem *) itemToCompare->nextListItem;
  }
  return false;
}
