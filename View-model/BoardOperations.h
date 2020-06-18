#ifndef boardOperations_h
#define boardOperations_h

#include <gtk/gtk.h>
#include "../Model/Board.h"

// File methods
gboolean createNewBoard(int sizeX, int sizeY);
gboolean openBoardFromFile(char * filepath);
gboolean saveBoardToFile(char *filepath);

//Edit methods
gboolean undoBoardState(void);
gboolean redoBoardState(void);


gboolean addStateToList(void);
void changeToBoardTitle(gboolean edited);

//Board area methods
gboolean fillBoard(GtkWidget * sender, GdkEvent * event, gpointer data);

//Helper methods
void updateBoard(void);

void removeWidget(GtkWidget * widget, gpointer data);
void changeImageFile(GtkWidget * image, char * file);
gboolean isBoardEdited(void);
#endif /* boardOperations_h */
