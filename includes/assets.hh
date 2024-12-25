#ifndef ASSETS_H
#define ASSETS_H

#include <gtk/gtk.h>
#include "interface.hh"

static void on_sidebar_activated(GtkListBox *listbox, GtkListBoxRow *listboxrow, GtkNotebook *notebook);
void asset_screen(GtkApplication *app, gpointer user_data);

#endif