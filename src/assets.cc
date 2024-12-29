#include <gtk/gtk.h>
#include "../includes/assets.hh"

static void on_sidebar_activated(GtkListBox *listbox, GtkListBoxRow *listboxrow, GtkNotebook *notebook) 
{
    int page_num = gtk_list_box_row_get_index(listboxrow);
    gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_num);
}

void asset_screen(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "assets");
    gtk_window_set_default_size(GTK_WINDOW(window), _screen_width, _screen_height);

    GtkWidget* asset_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(window), asset_pane);

    GtkWidget* listbox_sidebar = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox_sidebar), GTK_SELECTION_SINGLE);
    gtk_paned_set_start_child(GTK_PANED(asset_pane), listbox_sidebar);

    const char* items[] = {"Apple", "Microsoft", "Nvidia", "Tesla", "Facebook"};
    for (int i = 0; i < sizeof(items)/sizeof(items[0]); i++){
        GtkWidget* label = gtk_label_new(items[i]);
        gtk_list_box_insert(GTK_LIST_BOX(listbox_sidebar), label, i);
    }

    GtkWidget* notebook = gtk_notebook_new();
    gtk_paned_set_end_child(GTK_PANED(asset_pane), notebook);
    for (int z = 0; z < sizeof(items)/sizeof(items[0]); z++){
        char charbuffer[20];
        snprintf(charbuffer, sizeof(charbuffer), "Asset %d", z);
        GtkWidget* label = gtk_label_new(charbuffer);
        GtkWidget* content = gtk_label_new(charbuffer);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook),content, label);
    }
    g_signal_connect(listbox_sidebar, "row-activated",G_CALLBACK(on_sidebar_activated), notebook);
    gtk_window_present(GTK_WINDOW(window));
}
