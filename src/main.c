#include <stdio.h>
#include <stdlib.h>
#include "../includes/access.h"
#include "../includes/assets.h"

void activate(GtkApplication* app, gpointer user_data) {
    add_new_trader(app, user_data);
}

int main(int argc, char **argv) {
    GtkApplication* app = gtk_application_new("org.engine.com", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return status;
}
