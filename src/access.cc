#include <gtk/gtk.h>
#include "sqlite3.h"
#include "../includes/access.hh"
#include "../includes/interface.hh"
#include "../database/database.hh"
#include "../security/safe.hh"

#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>
#include <string.h>


#define ERROR_MSG(msg) \
    printf("Error in file %s, line %d: %s\n", __FILE__, __LINE__, msg)

static void on_add_new_trader(GtkButton *button, gpointer user_data)
{
    User_Data_Model *user_model = (User_Data_Model *)user_data;
    GtkEntryBuffer *email_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->email));
    const char *raw_email_data = gtk_entry_buffer_get_text(email_entry_buffer);

    GtkEntryBuffer *passw_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(user_model->password));
    const char *raw_passw_data = gtk_entry_buffer_get_text(passw_entry_buffer);

    if (sodium_init() < 0)
        printf("%s\n", "cannot initialise sodium");
    unsigned char key[crypto_secretbox_KEYBYTES];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char ciphertext[crypto_secretbox_MACBYTES + strlen(raw_passw_data) + 1];

    g_print("email passed: '%s'\n", raw_email_data);
    sqlite3 *db;
    int rc = sqlite3_open(_database_name, &db);
    if (rc != SQLITE_OK)
        printf("%s\n", sqlite3_errmsg(db));
    sqlite3_stmt *stmt;
    int result_stmt;
    rc = sqlite3_prepare_v2(db, _add_new_trader, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        printf("%s\n", "insert did not work");
    sqlite3_errmsg(db);
    sqlite3_bind_text(stmt, 1, raw_email_data, -1, SQLITE_STATIC);
    int encrypted_data = encrypt_password(raw_passw_data, ciphertext, nonce, key);
    static char buffer[30];
    snprintf(buffer, sizeof(buffer), "%d", encrypted_data);
    sqlite3_bind_text(stmt, 2, (const char *)buffer, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("%s\n", "could not step properly");
        sqlite3_errmsg(db);
    }
    sqlite3_finalize(stmt);
}

void add_new_trader(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), _engine_name);
    gtk_window_set_default_size(GTK_WINDOW(window), _screen_width, _screen_height);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(box, true);
    gtk_widget_set_hexpand(box, true);

    GtkWidget *ntrader_grid = gtk_grid_new();
    gtk_widget_set_margin_top(ntrader_grid, 100);
    gtk_widget_set_valign(ntrader_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(ntrader_grid, GTK_ALIGN_CENTER);

    GtkWidget *logo_image = gtk_image_new_from_file("logo.png");
    gtk_grid_attach(GTK_GRID(ntrader_grid), logo_image, 3, 1, 1, 1);
    gtk_image_set_pixel_size(GTK_IMAGE(logo_image), 100);
    gtk_widget_set_margin_bottom(logo_image, 20);

    GtkWidget *register_label = gtk_label_new("Trading Terminal");
    gtk_grid_attach(GTK_GRID(ntrader_grid), register_label, 3, 2, 1, 1);
    gtk_widget_set_margin_bottom(register_label, 20);
    gtk_widget_add_css_class(register_label, "body-text");

    GtkWidget *email_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(ntrader_grid), email_entry, 3, 3, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "email address");
    gtk_widget_set_size_request(email_entry, 200, 40);
    gtk_widget_add_css_class(email_entry, "body-text");

    GtkWidget *passw_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(ntrader_grid), passw_entry, 3, 4, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(passw_entry), "secure password");
    gtk_widget_set_size_request(passw_entry, 200, 40);
    gtk_entry_set_visibility(GTK_ENTRY(passw_entry), false);
    gtk_widget_set_margin_top(passw_entry, 5);
    gtk_widget_add_css_class(passw_entry, "body-text");

    User_Data_Model *user_model = g_new(User_Data_Model, 1);
    user_model->email = email_entry;
    user_model->password = passw_entry;

    GtkWidget *submit_button = gtk_button_new_with_label("Request terminal access");
    gtk_grid_attach(GTK_GRID(ntrader_grid), submit_button, 3, 7, 1, 1);
    gtk_widget_set_margin_top(submit_button, 5);
    gtk_widget_add_css_class(submit_button, "core");

    gtk_box_append(GTK_BOX(box), ntrader_grid);
    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css_style_path = "style.css";
    GFile *style_file = g_file_new_for_path(css_style_path);
    gtk_css_provider_load_from_file(provider, style_file);

    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_add_new_trader), user_model);

    gtk_window_present(GTK_WINDOW(window));
}