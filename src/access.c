#include <gtk/gtk.h>
#include "sqlite3.h"
#include "../includes/access.h"
#include "../database/database.h"

static void on_add_new_trader(GtkApplication* app, gpointer user_data){
    User_Data_Model* user_model = (User_Data_Model *) user_data;
    const GtkEntryBuffer* email_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->email));
    const gchar* raw_email_data = gtk_entry_buffer_get_text(email_entry_buffer);

    const GtkEntryBuffer* passw_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->password));
    const gchar* raw_passw_data = gtk_entry_buffer_get_text(passw_entry_buffer);

    g_print("email passed: '%s'", raw_email_data);
    sqlite3* _fetch_db_config = initialise_database_file("_trading_.db");
    int add_new_trader = add_nonexisting_user(_fetch_db_config, raw_email_data, raw_passw_data);
    if (add_new_trader) g_print("registration successful");
}

void add_new_trader(GtkButton button, gpointer user_data){}