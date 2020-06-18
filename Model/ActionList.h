#ifndef ActionList_h
#define ActionList_h

#include <stdio.h>
#include <gtk/gtk.h>
#include "Board.h"

gboolean addActionToList(Board b);

Board undoActionFromList(void);
Board redoActionFromList(void);

gboolean isCurrActionLast(void);
gboolean isCurrActionFirst(void);

void clearActionList(void);
void clearActionListFromCurr(void);

#endif /* ActionList_h */
