#include <gtk/gtk.h>
#include "sqlite3.h"
#include "../includes/access.h"
#include "../includes/interface.h"
#include "../database/database.h"

static void on_add_new_trader(GtkButton* button, gpointer user_data){
    User_Data_Model* user_model = (User_Data_Model *) user_data;
    GtkEntryBuffer* email_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->email));
    const char* raw_email_data = gtk_entry_buffer_get_text(email_entry_buffer);

    GtkEntryBuffer* passw_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->password));
    const char* raw_passw_data = gtk_entry_buffer_get_text(passw_entry_buffer);

    g_print("email passed: '%s'", raw_email_data);
    sqlite3* _fetch_db_config = initialise_database_file("_trading_.db");
    int add_new_trader = add_nonexisting_user(_fetch_db_config, raw_email_data, raw_passw_data);
    if (add_new_trader) g_print("registration successful");
}

void add_new_trader(GtkApplication* app, gpointer user_data){
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), _engine_name);
    gtk_window_set_default_size(GTK_WINDOW (window), _screen_width, _screen_height);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_widget_set_vexpand(box, true);
    gtk_widget_set_hexpand(box, true);

    GtkWidget* ntrader_grid = gtk_grid_new();
    gtk_widget_set_margin_top(ntrader_grid, 270);
    gtk_widget_set_valign(ntrader_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(ntrader_grid, GTK_ALIGN_CENTER);

    GtkWidget* email_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID (ntrader_grid), email_entry, 3,3,1,1);
    gtk_entry_set_placeholder_text(GTK_ENTRY (email_entry), "email address");
    gtk_widget_set_size_request(email_entry, 200, 40);

    GtkWidget* passw_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID (ntrader_grid), passw_entry, 3,4,1,1);
    gtk_entry_set_placeholder_text(GTK_ENTRY (passw_entry), "secure password");
    gtk_widget_set_size_request(passw_entry, 200, 40);
    gtk_entry_set_visibility(GTK_ENTRY(passw_entry), false);
    gtk_widget_set_margin_top(passw_entry, 5);

    User_Data_Model *user_model = g_new(User_Data_Model, 1);
    user_model->email = email_entry;
    user_model->password = passw_entry;

    GtkWidget* submit_button = gtk_button_new_with_label("register account");
    gtk_grid_attach(GTK_GRID (ntrader_grid), submit_button, 3,7,1,1);
    gtk_widget_set_margin_top(submit_button, 5);

    gtk_box_append(GTK_BOX (box), ntrader_grid);
    gtk_window_set_child(GTK_WINDOW (window), box);

    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_add_new_trader), user_model);

    gtk_window_present(GTK_WINDOW (window));
}