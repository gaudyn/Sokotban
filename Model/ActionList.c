#include "ActionList.h"
#include <gtk/gtk.h>
#include "Board.h"

GList *actionList = NULL, *currentAction = NULL;

//MARK: - Add to list
gboolean addActionToList(Board b){
    if(currentAction != NULL && g_list_next(currentAction) != NULL) clearActionListFromCurr();
    Board copy = copyBoard(b);
    if(copy == NULL) return FALSE;

    actionList = g_list_append(currentAction, copy);
    currentAction = currentAction == NULL ? actionList : g_list_next(currentAction);
    return TRUE;
}

//MARK: - Move back/forward in list
Board undoActionFromList(){
    if(isCurrActionFirst()) return NULL;

    currentAction = g_list_previous(currentAction);
    return copyBoard(currentAction->data);
}
Board redoActionFromList(){
    if(isCurrActionLast()) return FALSE;

    currentAction = g_list_next(currentAction);
    return copyBoard(currentAction->data);
}

//MARK: - Position in list
gboolean isCurrActionLast(){
    return g_list_next(currentAction) == NULL ? TRUE : FALSE;
}

gboolean isCurrActionFirst(){
    return g_list_previous(currentAction) == NULL ? TRUE : FALSE;
}

//MARK: - Clearing
void clearActionList(){
    currentAction = NULL;
    GList *it = actionList;
    while(it != NULL){
        GList *next = g_list_next(it);
        actionList = g_list_remove_link(actionList, it);
        boardDestroy(it->data);
        it = next;
    }
    actionList = NULL;
}

void clearActionListFromCurr(){
    GList *it = g_list_next(currentAction);
    while(it != NULL){
        GList *next = g_list_next(it);
        actionList = g_list_remove_link(actionList, it);
        it->data = boardDestroy(it->data);
        g_list_free(it);
        it = next;
    }
}
