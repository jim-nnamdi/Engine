#include <gtk/gtk.h>
#include <cairo.h>
#include "../includes/assets.h"
#include <stdio.h>
#include <math.h>

static void on_sidebar_activated(GtkListBox *listbox, GtkListBoxRow *listboxrow, GtkNotebook *notebook)
{
    int page_num = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(listboxrow));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page_num);
}

void asset_screen(GtkApplication *app, gpointer user_data)
{
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const gchar *css_style_path = "style.css";
    GFile *css_style_file = g_file_new_for_path(css_style_path);
    gtk_css_provider_load_from_file(css_provider, css_style_file);

    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "assets");
    gtk_window_set_default_size(GTK_WINDOW(window), _screen_width, _screen_height);

    GtkWidget *asset_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(window), asset_pane);

    GtkWidget *listbox_sidebar = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox_sidebar), GTK_SELECTION_SINGLE);
    gtk_paned_set_start_child(GTK_PANED(asset_pane), listbox_sidebar);
    gtk_widget_add_css_class(listbox_sidebar, "sidebar");

    const char *items[] = {"Apple", "Microsoft", "Nvidia", "Tesla", "Facebook"};
    for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
    {
        GtkWidget *label = gtk_label_new(items[i]);
        gtk_list_box_insert(GTK_LIST_BOX(listbox_sidebar), label, i);
        gtk_widget_add_css_class(label, "lbl");
    }

    GtkWidget *notebook = gtk_notebook_new();
    gtk_paned_set_end_child(GTK_PANED(asset_pane), notebook);
    for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
    {
        GtkWidget *label = gtk_label_new(items[i]);
        GtkWidget *content = gtk_label_new(items[i]);
        gtk_notebook_append_page(notebook, content, label);
    }
    g_signal_connect(listbox_sidebar, "row-activated", G_CALLBACK(on_sidebar_activated), notebook);
    gtk_window_present(GTK_WINDOW(window));
}
