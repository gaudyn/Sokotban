#include <string.h>
#include <stdlib.h>

#include "GameToolbar.h"
#include "MainWindow.h"
#include "GameGrid.h"
#include "../View-model/BoardLogic.h"

GtkWidget *zoomInButton, *zoomOutButton;
GtkWidget *undoButton, *redoButton;
GtkWidget *saveGameAsButton;
GtkWidget *resetButton;

//MARK: - Init toolbar
GtkWidget *initMainToolbar() {
    GtkAccelGroup *accels = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(getMainWindow()), accels);

    GtkWidget *mainToolbar = gtk_menu_bar_new();

    //MARK: File menu
    GtkWidget *fileMenuButton = gtk_menu_item_new_with_label("Plik");
    GtkWidget *fileMenu = gtk_menu_new();

    //New game
    GtkWidget *newGameButton = gtk_menu_item_new_with_label("Nowa gra");
    g_signal_connect(G_OBJECT(newGameButton), "activate", G_CALLBACK(loadNewBoard), NULL);
    gtk_widget_add_accelerator(newGameButton, "activate", accels, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    resetButton = gtk_menu_item_new_with_label("Restart");
    gtk_widget_set_sensitive(resetButton, FALSE);
    g_signal_connect(G_OBJECT(resetButton), "activate", G_CALLBACK(resetBoard), NULL);
    gtk_widget_add_accelerator(resetButton, "activate", accels, GDK_KEY_R, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    //Load game state
    GtkWidget *loadGameButton = gtk_menu_item_new_with_label("Wczytaj stan gry...");
    g_signal_connect(G_OBJECT(loadGameButton), "activate", G_CALLBACK(loadBoardState), NULL);
    gtk_widget_add_accelerator(loadGameButton, "activate", accels, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    //Save game state
    saveGameAsButton = gtk_menu_item_new_with_label("Zapisz stan gry jako...");
    gtk_widget_set_sensitive(saveGameAsButton, FALSE);
    g_signal_connect(G_OBJECT(saveGameAsButton), "activate", G_CALLBACK(saveBoardStateAs), NULL);
    gtk_widget_add_accelerator(saveGameAsButton, "activate", accels, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newGameButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), resetButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), loadGameButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveGameAsButton);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuButton), fileMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainToolbar), fileMenuButton);

    //MARK: Edit menu
    GtkWidget *editMenuButton = gtk_menu_item_new_with_label("Edycja");
    GtkWidget *editMenu = gtk_menu_new();

    undoButton = gtk_menu_item_new_with_label("Cofnij");
    gtk_widget_set_sensitive(undoButton, FALSE);
    g_signal_connect(G_OBJECT(undoButton), "activate", G_CALLBACK(undoBoardCallback), NULL);
    gtk_widget_add_accelerator(undoButton, "activate", accels, GDK_KEY_Z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    redoButton = gtk_menu_item_new_with_label("Powtórz");
    gtk_widget_set_sensitive(redoButton, FALSE);
    g_signal_connect(G_OBJECT(redoButton), "activate", G_CALLBACK(redoBoardCallback), NULL);
    gtk_widget_add_accelerator(redoButton, "activate", accels, GDK_KEY_Z, GDK_CONTROL_MASK|GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);


    gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), undoButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), redoButton);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(editMenuButton), editMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainToolbar), editMenuButton);


    //MARK: View menu
    GtkWidget *viewMenuButton = gtk_menu_item_new_with_label("Widok");
    GtkWidget *viewMenu = gtk_menu_new();

    zoomInButton = gtk_menu_item_new_with_label("Powiększ");
    g_signal_connect(G_OBJECT(zoomInButton), "activate", G_CALLBACK(changeMainGridSize), GINT_TO_POINTER(1));
    gtk_widget_add_accelerator(zoomInButton, "activate", accels, GDK_KEY_period, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    zoomOutButton = gtk_menu_item_new_with_label("Zmniejsz");
    g_signal_connect(G_OBJECT(zoomOutButton), "activate", G_CALLBACK(changeMainGridSize), GINT_TO_POINTER(-1));
    gtk_widget_add_accelerator(zoomOutButton, "activate", accels, GDK_KEY_comma, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *normalZoomButton = gtk_menu_item_new_with_label("Standardowe powiększenie");
    g_signal_connect(G_OBJECT(normalZoomButton), "activate", G_CALLBACK(changeMainGridSize), GINT_TO_POINTER(32));

    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), zoomInButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), zoomOutButton);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu), normalZoomButton);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(viewMenuButton), viewMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(mainToolbar), viewMenuButton);
    return mainToolbar;
}

//MARK: - File menu actions
void loadNewBoard(GtkWidget *widget, gpointer data){
    GtkWidget *openFileDialog = gtk_file_chooser_dialog_new("Wybierz planszę", GTK_WINDOW(getMainWindow()), GTK_FILE_CHOOSER_ACTION_OPEN, "Anuluj", GTK_RESPONSE_CANCEL, "Otwórz", GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter *mapFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(mapFilter, "*.map");
    gtk_file_filter_set_name(mapFilter, "Mapy Sokoban");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(openFileDialog), mapFilter);

    int result = gtk_dialog_run(GTK_DIALOG(openFileDialog));
    if(result == GTK_RESPONSE_ACCEPT){
        char *filePath;
        GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(openFileDialog);
        filePath = gtk_file_chooser_get_filename(fileChooser);

        gtk_widget_set_sensitive(saveGameAsButton, TRUE);
        gtk_widget_set_sensitive(resetButton, TRUE);
        initLogic(filePath);
    }
    gtk_widget_destroy(openFileDialog);
}

void loadBoardState(GtkWidget *widget, gpointer data){
    GtkWidget *openFileDialog = gtk_file_chooser_dialog_new("Wybierz zapis gry", GTK_WINDOW(getMainWindow()), GTK_FILE_CHOOSER_ACTION_OPEN, "Anuluj", GTK_RESPONSE_CANCEL, "Otwórz", GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter *mapFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(mapFilter, "*.sav");
    gtk_file_filter_set_name(mapFilter, "Zapis Sokoban");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(openFileDialog), mapFilter);

    int result = gtk_dialog_run(GTK_DIALOG(openFileDialog));
    if(result == GTK_RESPONSE_ACCEPT){
        char *filePath;
        GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(openFileDialog);
        filePath = gtk_file_chooser_get_filename(fileChooser);

        //TODO: Check if map file has been modified after creating save

        gtk_widget_set_sensitive(saveGameAsButton, TRUE);
        gtk_widget_set_sensitive(resetButton, TRUE);
        loadGameState(filePath);
    }
    gtk_widget_destroy(openFileDialog);
}

void saveBoardStateAs(GtkWidget *widget, gpointer data){
    GtkWidget *saveFileDialog = gtk_file_chooser_dialog_new("Wybierz zapis gry", GTK_WINDOW(getMainWindow()), GTK_FILE_CHOOSER_ACTION_SAVE, "Anuluj", GTK_RESPONSE_CANCEL, "Otwórz", GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(saveFileDialog), "zapis.sav");

    GtkFileFilter *mapFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(mapFilter, "*.sav");
    gtk_file_filter_set_name(mapFilter, "Zapis Sokoban");

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(saveFileDialog), mapFilter);

    int result = gtk_dialog_run(GTK_DIALOG(saveFileDialog));
    if(result == GTK_RESPONSE_ACCEPT){
        char *filePath;
        GtkFileChooser *fileChooser = GTK_FILE_CHOOSER(saveFileDialog);
        filePath = gtk_file_chooser_get_filename(fileChooser);
        if(strstr(filePath, ".sav") == NULL){
            char extension[] = ".sav";
            char *editedFilepath = malloc(sizeof(char)*(strlen(filePath)+4));
            strcpy(editedFilepath, filePath);
            filePath = strcat(editedFilepath, extension);
        }
        saveGameState(filePath);
    }
    gtk_widget_destroy(saveFileDialog);
}

//MARK: - Edit menu actions
void undoBoardCallback(GtkWidget *widget, gpointer data){
    if(!undoLastAction()) showErrorDialog("Nie udało się cofnąć ostatniej czynności");
}

void redoBoardCallback(GtkWidget *widget, gpointer data){

    if(!redoLastAction()) showErrorDialog("Nie udało się powtórzyć ostatniej czynności");
}

void resetBoard(GtkWidget *widget, gpointer data){
    revertBoard();
}

void updateEditButtonsState(gboolean undoState, gboolean redoState){
    gtk_widget_set_sensitive(undoButton, undoState);
    gtk_widget_set_sensitive(redoButton, redoState);
}

//MARK: - View menu actions
void changeMainGridSize(GtkWidget *widget, gpointer data){
    int modifier = GPOINTER_TO_INT(data);
    int res = setTileSize(modifier, getCurrentBoard());

    gtk_widget_set_sensitive(zoomInButton, TRUE);
    gtk_widget_set_sensitive(zoomOutButton, TRUE);

    if(res == -1) gtk_widget_set_sensitive(zoomOutButton, FALSE);
    if(res == 1) gtk_widget_set_sensitive(zoomInButton, FALSE);
}
