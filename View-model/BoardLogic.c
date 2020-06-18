#include <stdbool.h>
#include <stdlib.h>
#include "BoardLogic.h"
#include "../Model/Board.h"
#include "../Model/ActionList.h"

#include "../View/MainWindow.h"
#include "../View/GameGrid.h"
#include "../View/GameToolbar.h"

Board mainBoard, playableCopy;

struct pos{
    int x,y;
};

struct pos player, start;
GSList *buttonTiles = NULL;

int moves = 0, pushes = 0;
gboolean isGameFinished = TRUE;

//MARK: - Init
void initLogic(char *filePath){
    if(mainBoard != NULL) boardDestroy(mainBoard);
    if(playableCopy != NULL) boardDestroy(playableCopy);

    mainBoard = loadBoard(filePath);
    if(verifyBoard(mainBoard)){
      showErrorDialog("Wybranej planszy nie da się ukończyć. Proszę wybrać inną.");
      return;
    }
    playableCopy = copyBoard(mainBoard);
    addActionToList(playableCopy);

    while(buttonTiles != NULL){
        GSList *link = buttonTiles;
        buttonTiles = g_slist_remove_link(buttonTiles, link);
        free(link->data);
        g_slist_free(link);
    }
    isGameFinished = FALSE;
    moves = 0;
    pushes = 0;
    setBoardHiMoves(playableCopy, 0);
    setBoardHiPushes(playableCopy, 0);

    prepareBoard(mainBoard);
    loadBoardGrid(playableCopy);
}

void prepareBoard(Board b){

    for(int y = 0; y < getBoardSizeY(b); y++){
        for(int x = 0; x < getBoardSizeX(b); x++){
            int tile = getBoardValueAt(b, x, y);
            if(tile == PLAYER){
                start.x = x;
                start.y = y;
                player = start;
            }else if(tile == BUTTON){
                Pos * buttonPos = malloc(sizeof(Pos));
                buttonPos->x = x;
                buttonPos->y = y;
                buttonTiles = g_slist_prepend(buttonTiles, buttonPos);
            }
        }
    }

}

void revertBoard(){

    boardDestroy(playableCopy);
    playableCopy = copyBoard(mainBoard);

    clearActionList();
    addActionToList(playableCopy);

    setBoardValueAt(playableCopy, player.x, player.y, WALK);
    player.x = start.x;
    player.y = start.y;

    isGameFinished = FALSE;
    moves = 0;
    pushes = 0;
    setBoardHiMoves(playableCopy, 0);
    setBoardHiPushes(playableCopy, 0);

    loadBoardGrid(mainBoard);
}

//MARK: - Player movement
void movePlayer(int dx, int dy, GtkWidget *boardGrid){
    if(isGameFinished) return;
    if(player.x + dx < 0 || player.x + dx >= getBoardSizeX(mainBoard)) return;
    if(player.y + dy < 0 || player.y + dy >= getBoardSizeY(mainBoard)) return;

    int newPlayerX = player.x+dx, newPlayerY = player.y+dy;
    bool canMove = false, movedBox = false;
    switch (getBoardValueAt(playableCopy, newPlayerX, newPlayerY)) {
        case BOX:;
            int nextTile = getBoardValueAt(playableCopy, newPlayerX+dx, newPlayerY+dy);
            if(nextTile == WALK || nextTile == BUTTON){
                canMove = true;
                setBoardValueAt(playableCopy, newPlayerX+dx, newPlayerY+dy, BOX);
                setBoardValueAt(playableCopy, newPlayerX, newPlayerY, WALK);
                updateGridTile(newPlayerX+dx, newPlayerY+dy, BOX);
                movedBox = true;

                pushes++;
                setBoardHiPushes(playableCopy, pushes);
            }
            break;
        case WALK:
        case BUTTON:
        case PLAYER:
            canMove = true;
            break;
        default:
            break;
    }
    if(canMove){

        moves++;
        setBoardHiMoves(playableCopy, moves);

        setBoardValueAt(playableCopy, newPlayerX, newPlayerY, PLAYER);
        setBoardValueAt(playableCopy, player.x, player.y, getBoardValueAt(mainBoard, player.x, player.y) == BUTTON ? BUTTON : WALK);
        updateGridTile(newPlayerX, newPlayerY, PLAYER);
        updateGridTile(player.x, player.y, getBoardValueAt(playableCopy, player.x, player.y));

        player.x = newPlayerX;
        player.y = newPlayerY;
        if(movedBox && isPuzzleSolved()) puzzleSolved();

        if(!addActionToList(playableCopy)){
            showErrorDialog("Błąd pamięci!");
        }
        updateEditButtonsState(TRUE, FALSE);
    }
}

//MARK: - Puzzle solved
gboolean isPuzzleSolved(){

    for(GSList *it = buttonTiles; it != NULL; it = g_slist_next(it)){
        Pos *buttonPos = (Pos *)it->data;
        if(getBoardValueAt(playableCopy, buttonPos->x, buttonPos->y) != BOX) return FALSE;
    }

    return TRUE;
}

void puzzleSolved(){
    int hiMoves = getBoardHiMoves(mainBoard);
    int hiPushes = getBoardHiPushes(mainBoard);

    isGameFinished = TRUE;

    bool willSaveBoard = false;
    if(moves < hiMoves){
        setBoardHiMoves(mainBoard, moves);
        willSaveBoard = true;
    }
    if(pushes < hiPushes){
        setBoardHiPushes(mainBoard, pushes);
        willSaveBoard = true;
    }
    if(willSaveBoard) saveBoard(mainBoard, getBoardFilepath(mainBoard));

    GtkWidget *winDialog = gtk_message_dialog_new(GTK_WINDOW(getMainWindow()), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "Poziom ukończony!");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(winDialog), "Twój wynik to %d ruchów (rekord %d) i %d pchnięć (rekord %d).\nCo chcesz teraz zrobić?", moves, hiMoves, pushes, hiPushes);
    gtk_dialog_add_button(GTK_DIALOG(winDialog), "Restart", GTK_RESPONSE_REJECT);
    gtk_dialog_add_button(GTK_DIALOG(winDialog), "Zamknij grę", GTK_RESPONSE_CANCEL);
    gtk_dialog_add_button(GTK_DIALOG(winDialog), "Inna plansza", GTK_RESPONSE_ACCEPT);

    int result = gtk_dialog_run(GTK_DIALOG(winDialog));
    gtk_widget_destroy(winDialog);

    if(result == GTK_RESPONSE_CANCEL) gtk_main_quit();
    else if(result == GTK_RESPONSE_ACCEPT) loadNewBoard(NULL, NULL);
    else if(result == GTK_RESPONSE_REJECT) resetBoard(NULL, NULL);


}
//MARK: - Edit/undo methods
gboolean undoLastAction(){
    Board lastAction = undoActionFromList();
    if(lastAction == NULL) return FALSE;

    playableCopy = boardDestroy(playableCopy);
    playableCopy = lastAction;

    moves = getBoardHiMoves(playableCopy);
    pushes = getBoardHiPushes(playableCopy);

    updateEditButtonsState(!isCurrActionFirst(), TRUE);
    updateBoardGrid(playableCopy);

    updatePlayerPos();
    return TRUE;
}

gboolean redoLastAction(){
    Board nextAction = redoActionFromList();
    if(nextAction == NULL) return FALSE;

    playableCopy = boardDestroy(playableCopy);
    playableCopy = nextAction;

    moves = getBoardHiMoves(playableCopy);
    pushes = getBoardHiPushes(playableCopy);

    updateEditButtonsState(TRUE, !isCurrActionLast());
    updateBoardGrid(playableCopy);

    updatePlayerPos();

    return TRUE;
}

//MARK: - Saving and loading game state
gboolean saveGameState(char *filePath){
    if(!saveBoard(playableCopy, filePath)){
        return FALSE;
    }
    return TRUE;
}

gboolean loadGameState(char *filePath){
    Board loadedBoard = loadBoard(filePath);
    if(loadedBoard == NULL) return FALSE;

    initLogic(getBoardFilepath(loadedBoard));
    playableCopy = loadedBoard;

    struct pos oldStart = start;

    prepareBoard(playableCopy);

    start = oldStart;

    moves = getBoardHiMoves(playableCopy);
    pushes = getBoardHiPushes(playableCopy);

    loadBoardGrid(playableCopy);
    return TRUE;
}

//MARK: - Helper methods
Board getCurrentBoard(){
    return playableCopy;
}

Board getMainBoard(){
    return mainBoard;
}

void updatePlayerPos(){
    if(getBoardValueAt(playableCopy, player.x-1, player.y)  == PLAYER){
        player.x--;
    }
    else if(getBoardValueAt(playableCopy, player.x+1, player.y)  == PLAYER){
        player.x++;
    }
    else if(getBoardValueAt(playableCopy, player.x, player.y-1)  == PLAYER){
        player.y--;
    }
    else if(getBoardValueAt(playableCopy, player.x, player.y+1)  == PLAYER){
        player.y++;
    }
}
