#ifndef ASSETS_H
#define ASSETS_H

#include <gtk/gtk.h>
#include "interface.hh"

class Assets
{
public:
    Assets(const Assets& assets);
    Assets(Assets&& assets);
    static void on_sidebar_activated(GtkListBox *listbox, GtkListBoxRow *listboxrow, GtkNotebook *notebook);
    void asset_screen(GtkApplication *app, gpointer user_data);
};
#endif