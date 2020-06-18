#include <gtk/gtk.h>
#include <string.h>

#include "BoardOperations.h"
#include "../View/EditorMenu.h"
#include "../View/MainWindow.h"
#include "../Model/Board.h"
#include "../Model/ActionList.h"

extern int currentTile;
extern Board board;
extern GtkWidget * mainGrid;
int size = 32;

gboolean hasChanges = FALSE;

//MARK: - File methods
gboolean createNewBoard(int sizeX, int sizeY){

    if(board != NULL) board = boardDestroy(board);

    board = boardInit(sizeX, sizeY);
    if(board == NULL) return FALSE;

    GtkWidget *gridParent = gtk_widget_get_parent(mainGrid);
    if(!GTK_IS_EVENT_BOX(gridParent)) showErrorDialog("Wystąpił nieoczekiwany błąd!");

    gtk_widget_set_sensitive(gridParent, TRUE);
    clearActionList();
    updateBoard();

    return TRUE;
}

gboolean openBoardFromFile(char *filepath){
    if(filepath == NULL) return FALSE;

    if(board != NULL) board = boardDestroy(board);

    board = loadBoard(filepath);
    if(board == NULL) return FALSE;

    setBoardFilepath(board, filepath);
    GtkWidget *gridParent = gtk_widget_get_parent(mainGrid);
    if(!GTK_IS_EVENT_BOX(gridParent)) showErrorDialog("Wystąpił nieoczekiwany błąd!");

    gtk_widget_set_sensitive(gridParent, TRUE);
    clearActionList();
    updateBoard();

    return TRUE;
}

gboolean saveBoardToFile(char *filepath){
    //Save board to filepath / earlier filepath if not present
    if(filepath == NULL && (filepath = getBoardFilepath(board)) == NULL) return FALSE;
    char *fileCopy = malloc(sizeof(char)*strlen(filepath));
    fileCopy = strcpy(fileCopy, filepath);
    setBoardFilepath(board, "\0");
    if(!saveBoard(board, fileCopy)) {
      return FALSE;
    }
    setBoardFilepath(board, fileCopy);
    free(fileCopy);
    return TRUE;
}

//MARK: - Edit methods

gboolean undoBoardState(){

    Board previousAction = undoActionFromList();
    if(previousAction == NULL) return FALSE;

    board = boardDestroy(board);
    board = previousAction;

    updateEditButtons(!isCurrActionFirst(), TRUE);

    return TRUE;
}

gboolean redoBoardState(){

    Board nextAction = redoActionFromList();
    if(nextAction == NULL) return FALSE;
    board = boardDestroy(board);
    board = nextAction;

    updateEditButtons(TRUE, !isCurrActionLast());

    return TRUE;
}

gboolean addStateToList(){
    if(!addActionToList(board)) return FALSE;

    if(!hasChanges) changeToBoardTitle(TRUE);
    hasChanges = TRUE;

    updateEditButtons(TRUE, FALSE);

    return TRUE;
}

void changeToBoardTitle(gboolean edited){
    gchar *boardTitle = getBoardFilename(board);
    hasChanges = edited;

    if(boardTitle == NULL){
        if(edited) setMainWindowTitle("Bez nazwy - edytowany");
        else setMainWindowTitle("Bez nazwy");
    }else{
        gchar edit[] = " - edytowany";
        gchar *windowTitle;
        windowTitle = edited ? strcat(boardTitle, edit) : boardTitle;
        setMainWindowTitle(windowTitle);
    }
}

//MARK: - Board area methods

gboolean fillBoard(GtkWidget * sender, GdkEvent * event, gpointer data){

    double mouseX, mouseY;
    if(event->type == GDK_BUTTON_PRESS){
        mouseX = event->button.x;
        mouseY = event->button.y;
    }else if(event->type == GDK_MOTION_NOTIFY && (event->motion.state & GDK_BUTTON1_MASK)){
        mouseX = event->motion.x;
        mouseY = event->motion.y;
    }else return FALSE;


    GtkAllocation* alloc = g_new(GtkAllocation, 1);

    GtkWidget *firstChild = gtk_grid_get_child_at(GTK_GRID(mainGrid), 0, 0);
    gtk_widget_get_allocation(firstChild, alloc);

    int posX = (int)mouseX/(alloc->width+gtk_grid_get_row_spacing(GTK_GRID(mainGrid)));
    int posY = (int)mouseY/(alloc->height+gtk_grid_get_column_spacing(GTK_GRID(mainGrid)));

    if(posX < 0 || posX >= getBoardSizeX(board) || posY < 0 || posY >= getBoardSizeY(board)) return FALSE;

    setBoardValueAt(board, posX, posY, currentTile);
    GtkWidget *cellImage = gtk_grid_get_child_at(GTK_GRID(mainGrid), posX, posY);

    switch (currentTile) {
        case BRICK:
            changeImageFile(cellImage, "Tiles/brick.png");
            break;
        case WALK:
            changeImageFile(cellImage, "Tiles/walkable.png");
            break;
        case PLAYER:
            changeImageFile(cellImage, "Tiles/player_walk.png");
            break;
        case BOX:
            changeImageFile(cellImage, "Tiles/box.png");
            break;
        case BUTTON:
            changeImageFile(cellImage, "Tiles/button.png");
            break;
        default:
            changeImageFile(cellImage, "Tiles/void.jpg");
            break;
    }
    return TRUE;
}

//MARK: - Helper methods
void updateBoard(){
    gtk_container_foreach(GTK_CONTAINER(mainGrid), removeWidget, NULL);

    for(int i = 0; i < getBoardSizeY(board); i++){
        for(int j = 0; j < getBoardSizeX(board); j++){

            GtkWidget *cellImage = gtk_image_new();

            switch (getBoardValueAt(board, j, i))  {
                   case BRICK:
                       changeImageFile(cellImage, "Tiles/brick.png");
                       break;
                   case WALK:
                       changeImageFile(cellImage, "Tiles/walkable.png");
                       break;
                   case PLAYER:
                       changeImageFile(cellImage, "Tiles/player_walk.png");
                       break;
                   case BOX:
                       changeImageFile(cellImage, "Tiles/box.png");
                       break;
                   case BUTTON:
                       changeImageFile(cellImage, "Tiles/button.png");
                       break;
                   default:
                       changeImageFile(cellImage, "Tiles/void.jpg");
                       break;
               }
            gtk_grid_attach(GTK_GRID(mainGrid), cellImage, j, i, 1, 1);
        }
    }
    gtk_widget_show_all(mainGrid);
}

void removeWidget(GtkWidget * widget, gpointer data){
    gtk_widget_destroy(widget);
}

void changeImageFile(GtkWidget * image, char * file){
    GdkPixbuf *pixbuf;
    pixbuf = gdk_pixbuf_new_from_file_at_size(file, size, -1, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

gboolean isBoardEdited(){
    return hasChanges;
}
