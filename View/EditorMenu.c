#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "EditorMenu.h"
#include "../Model/Board.h"
#include "../View-model/BoardOperations.h"
#include "MainWindow.h"

static GtkWidget *dialogWindow;
static GtkWidget *widthField;
static GtkWidget *heightField;

extern GtkWidget *mainGrid;
extern Board board;

extern void addActionCallback(GtkWidget *widget, gpointer data);

GtkWidget *saveFile, *saveAsFile;
GtkWidget *undoButton, *redoButton;
GtkWidget *reduceBoard, *magnifyBoard;

GtkWidget * prepareMainWindowMenu(){

    GtkAccelGroup *accels = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(getMainWindow()), accels);

    //MARK: Menu bar
    GtkWidget *mainMenu = gtk_menu_bar_new();

    //MARK: File menu
    GtkWidget *fileMenuButton = gtk_menu_item_new_with_label("Plik");
    GtkWidget *fileMenu = gtk_menu_new();

    //New file
    GtkWidget *newFile = gtk_menu_item_new_with_label("Nowy");
    g_signal_connect(G_OBJECT(newFile), "activate", G_CALLBACK(showNewBoardDialog), NULL);
    gtk_widget_add_accelerator(newFile, "activate", accels, GDK_KEY_N,GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *openFile = gtk_menu_item_new_with_label("Otwórz...");
    g_signal_connect(G_OBJECT(openFile), "activate", G_CALLBACK(showOpenBoardDialog), NULL);
    gtk_widget_add_accelerator(openFile, "activate", accels, GDK_KEY_O,GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *divider = gtk_separator_menu_item_new();

    saveFile = gtk_menu_item_new_with_label("Zapisz");
    g_signal_connect(G_OBJECT(saveFile), "activate", G_CALLBACK(saveBoardCallback), GINT_TO_POINTER(0));
    gtk_widget_add_accelerator(saveFile, "activate", accels, GDK_KEY_S,GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    saveAsFile = gtk_menu_item_new_with_label("Zapisz jako...");
    g_signal_connect(G_OBJECT(saveAsFile), "activate", G_CALLBACK(saveBoardCallback), GINT_TO_POINTER(1));
    gtk_widget_add_accelerator(saveAsFile, "activate", accels, GDK_KEY_S,GDK_CONTROL_MASK|GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);

    setSavesEnabled(FALSE);

    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newFile);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openFile);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), divider);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveFile);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveAsFile);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuButton), fileMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), fileMenuButton);

    //MARK: Edit menu

    GtkWidget *editMenuButton = gtk_menu_item_new_with_label("Edycja");
    GtkWidget *editMenu = gtk_menu_new();

    undoButton = gtk_menu_item_new_with_label("Cofnij");
    g_signal_connect(G_OBJECT(undoButton), "activate", G_CALLBACK(undoButtonCallback), NULL);
    gtk_widget_add_accelerator(undoButton, "activate", accels, GDK_KEY_Z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    redoButton = gtk_menu_item_new_with_label("Ponów");
    g_signal_connect(G_OBJECT(redoButton), "activate", G_CALLBACK(redoButtonCallback), NULL);
    gtk_widget_add_accelerator(redoButton, "activate", accels, GDK_KEY_Z, GDK_CONTROL_MASK|GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);

    updateEditButtons(FALSE, FALSE);

    gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), undoButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), redoButton);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(editMenuButton), editMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), editMenuButton);
    //MARK: View menu
    GtkWidget *viewMenuButton = gtk_menu_item_new_with_label("Widok");
    GtkWidget *viewMenu = gtk_menu_new();

    reduceBoard = gtk_menu_item_new_with_label("Pomniejsz");
    g_signal_connect(G_OBJECT(reduceBoard), "activate", G_CALLBACK(setGridScale), GINT_TO_POINTER(-1));
    gtk_widget_add_accelerator(reduceBoard, "activate", accels, GDK_KEY_comma, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    magnifyBoard = gtk_menu_item_new_with_label("Powiększ");
    g_signal_connect(G_OBJECT(magnifyBoard), "activate", G_CALLBACK(setGridScale), GINT_TO_POINTER(1));
    gtk_widget_add_accelerator(magnifyBoard, "activate", accels, GDK_KEY_period, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *normalZoom = gtk_menu_item_new_with_label("Standarowy zoom");
    g_signal_connect(G_OBJECT(normalZoom), "activate", G_CALLBACK(setGridScale), GINT_TO_POINTER(32));


    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), magnifyBoard);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), reduceBoard);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), normalZoom);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(viewMenuButton), viewMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), viewMenuButton);

    return mainMenu;
}


//MARK: - New board dialog
void showNewBoardDialog(GtkWidget * widget, gpointer data){

    if(isBoardEdited()){
        if(showEditionAlertDialog() == GTK_RESPONSE_YES){
            saveBoardCallback(widget, data);
            return;
        }
    }
    dialogWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialogWindow), "Nowa plansza");
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialogWindow), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(dialogWindow), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(dialogWindow), GTK_WINDOW(getMainWindow()));
    gtk_window_set_position(GTK_WINDOW(dialogWindow), GTK_WIN_POS_CENTER_ON_PARENT);

    //Wprowadzanie liczb

    GtkAdjustment * widthAdjustment = gtk_adjustment_new(10, 5, 30, 1, 0, 0);
    GtkAdjustment * heightAdjustment = gtk_adjustment_new(10, 5, 20, 1, 0, 0);

    //Pole z szerokością planszy

    GtkWidget * widthLabel = gtk_label_new("Szerokość:");

    widthField = gtk_spin_button_new(widthAdjustment, 2, 0);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(widthField), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(widthField), 3);

    GtkWidget * widthBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(widthBox), widthLabel, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widthBox), widthField, FALSE, FALSE, 0);

    //Pole z wysokością planszy

    GtkWidget * heightLabel = gtk_label_new("Wysokość:");
    heightField = gtk_spin_button_new(heightAdjustment, 2, 0);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(widthField), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(heightField), 3);

    GtkWidget * heightBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(heightBox), heightLabel, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(heightBox), heightField, FALSE, FALSE, 0);

    //Przyciski

    GtkWidget * okButton = gtk_button_new_with_label("OK");
    g_signal_connect(G_OBJECT(okButton), "clicked", G_CALLBACK(newBoardCallback), NULL);

    GtkWidget * cancelButton = gtk_button_new_with_label("Anuluj");
    g_signal_connect(G_OBJECT(cancelButton), "clicked", G_CALLBACK(closeDialogWindow), NULL);

    GtkWidget * buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_set_homogeneous(GTK_BOX(buttonBox), TRUE);
    gtk_box_pack_start(GTK_BOX(buttonBox), okButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonBox), cancelButton, TRUE, TRUE, 0);

    //Główny kontener
    GtkWidget * dimensionBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_box_pack_start(GTK_BOX(dimensionBox), widthBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dimensionBox), heightBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dimensionBox), buttonBox, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(dialogWindow), dimensionBox);

    gtk_widget_show_all(dialogWindow);
}

void closeDialogWindow(GtkWidget * widget, gpointer data){
    gtk_window_close(GTK_WINDOW(dialogWindow));
    gtk_widget_destroy(dialogWindow);
}

void newBoardCallback(GtkWidget * widget, gpointer data){

    int sizeX = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widthField));
    int sizeY = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(heightField));

    if(!createNewBoard(sizeX, sizeY)) showErrorDialog("Nie udało się stworzyć nowej planszy");
    else{
        addActionCallback(widget,data);
        updateEditButtons(FALSE, FALSE);
        setSavesEnabled(TRUE);
        changeToBoardTitle(FALSE);
    }
    gtk_window_close(GTK_WINDOW(dialogWindow));
}

//MARK: - Open file dialog
void showOpenBoardDialog(GtkWidget *widget, gpointer data){

    if(isBoardEdited()){
        if(showEditionAlertDialog() == GTK_RESPONSE_YES){
            saveBoardCallback(widget, data);
            return;
        }
    }

    GtkWidget *openDialog = gtk_file_chooser_dialog_new("Otwórz plik", GTK_WINDOW(getMainWindow()), GTK_FILE_CHOOSER_ACTION_OPEN, ("_Anuluj"), GTK_RESPONSE_CANCEL, ("_Otwórz"), GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.map");
    gtk_file_filter_set_name(filter, "Mapy Sokoban");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(openDialog), filter);

    gint result = gtk_dialog_run(GTK_DIALOG(openDialog));

    if(result == GTK_RESPONSE_ACCEPT){
        char *filepath;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(openDialog);
        filepath = gtk_file_chooser_get_filename(chooser);

        if(!openBoardFromFile(filepath)) showErrorDialog("Nie udało się wczytać planszy");
        else{
            addActionCallback(widget,data);
            updateEditButtons(FALSE, FALSE);
            setSavesEnabled(TRUE);
            changeToBoardTitle(FALSE);
        }
    }
    gtk_widget_destroy(openDialog);
}

//MARK: - Save file dialog
void saveBoardWithDialog(GtkWidget *widget, gpointer data){
    GtkWidget *saveDialog = gtk_file_chooser_dialog_new("Zapisz jako", GTK_WINDOW(getMainWindow()), GTK_FILE_CHOOSER_ACTION_SAVE, ("_Anuluj"), GTK_RESPONSE_CANCEL, ("_Zapisz"), GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(saveDialog), "plansza.map");

    GtkFileFilter *mapFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(mapFilter, "*.map");
    gtk_file_filter_set_name(mapFilter, "Mapy Sokoban");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(saveDialog), mapFilter);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(saveDialog), TRUE);
    g_signal_connect(G_OBJECT(saveDialog), "confirm-overwrite", G_CALLBACK(confirmOverwriteCallback), NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(saveDialog));
    if(result == GTK_RESPONSE_ACCEPT){
        char *filepath;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(saveDialog);
        filepath = gtk_file_chooser_get_filename(chooser);
        if(strstr(filepath, ".map") == NULL){
            char extension[] = ".map";
            char *editedFilepath = malloc(sizeof(char)*(strlen(filepath)+4));
            strcpy(editedFilepath, filepath);
            filepath = strcat(editedFilepath, extension);
        }

        if(!saveBoardToFile(filepath)) showErrorDialog("Nie udało się zapisać planszy");
        else changeToBoardTitle(FALSE);

    }
    gtk_widget_destroy(GTK_WIDGET(saveDialog));
}

void saveBoardWithoutDialog(GtkWidget *widget, gpointer data){
    //Try to save the board with its filepath
    if(!saveBoardToFile(NULL)){
        printf("Otwieram okno dialogowe\n");
        //Board hasn't been saved before
        saveBoardWithDialog(widget, data);
    }else changeToBoardTitle(FALSE);

}

void saveBoardCallback(GtkWidget *widget, gpointer data){
    int showDialog = GPOINTER_TO_INT(data);
    int isBoardInvalid = verifyBoard(board);
    void (*saveBoard)(GtkWidget*, gpointer) = showDialog ? &saveBoardWithDialog : &saveBoardWithoutDialog;
    if(isBoardInvalid){
        showInvalidBoardDialog(isBoardInvalid, saveBoard);
        return;
    }else (*saveBoard)(widget, data);

}


GtkFileChooserConfirmation confirmOverwriteCallback(GtkFileChooser *chooser, gpointer data){
    return GTK_FILE_CHOOSER_CONFIRMATION_CONFIRM;
}
//MARK: - Activate save buttons

void setSavesEnabled(gboolean sensitivity){
    gtk_widget_set_sensitive(saveFile, sensitivity);
    gtk_widget_set_sensitive(saveAsFile, sensitivity);
}

//MARK: - Information dialogs
void showInvalidBoardDialog(int code, void (*saveFunc)(GtkWidget*, gpointer)){
    char *message = malloc(sizeof(char)*100);
    switch (code) {
        case PLAYER:
            strcpy(message, "Nieprawidłowa liczba graczy na planszy");
            break;
        case BOX:
            strcpy(message, "Skrzyń jest więcej od pól docelowych");
            break;
        case BUTTON:
            strcpy(message, "Skrzyń jest mniej od pól docelowych");
            break;
        default:
            return;
    }
    GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(getMainWindow()), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, "%s", strcat(message, "\nCzy na pewno chcesz zapisać planszę?"));
    int result = gtk_dialog_run(GTK_DIALOG(warningDialog));

    if(result == GTK_RESPONSE_YES) (*saveFunc)(NULL, NULL);

    gtk_widget_destroy(warningDialog);

}

//MARK: - Change grid scale
void setGridScale(GtkWidget * widget, gpointer data){
    extern int size;
    int modifier = GPOINTER_TO_INT(data);
    if (modifier == -1 && size > 25) size--;
    else if (modifier == 1 && size < 40) size++;
    else if (modifier >= 25 && modifier <= 40)size = modifier;

    gtk_widget_set_sensitive(magnifyBoard, TRUE);
    gtk_widget_set_sensitive(reduceBoard, TRUE);

    if(size <= 25)
        gtk_widget_set_sensitive(reduceBoard, FALSE);
    if(size >= 40)
        gtk_widget_set_sensitive(magnifyBoard, FALSE);

    updateBoard();
}

//MARK: - Edit menu methods

void undoButtonCallback(GtkWidget *widget, gpointer data){

    if(!undoBoardState()){
        showErrorDialog("Nie udało się cofnąć ostatniej czynności");
        return;
    }
    updateBoard();
}

void redoButtonCallback(GtkWidget *widget, gpointer data){

    if(!redoBoardState()){
        showErrorDialog("Nie udało się powtórzyć ostatniej czynności");
        return;
    }
    updateBoard();
}

void updateEditButtons(gboolean undoState, gboolean redoState){
    gtk_widget_set_sensitive(undoButton, undoState);
    gtk_widget_set_sensitive(redoButton, redoState);
}
