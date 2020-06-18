#ifndef MainEditorToolbar_h
#define MainEditorToolbar_h

#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *initToolbar(void);
void setCurrentTile(GtkWidget * widget, gpointer tile);

#endif /* MainEditorToolbar_h */
