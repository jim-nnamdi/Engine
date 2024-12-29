#ifndef ACCESS_H
#define ACCESS_H

#include <gtk/gtk.h>

typedef struct
{
    GtkWidget *email;
    GtkWidget *password;
} user_data_model;

typedef user_data_model User_Data_Model;

class Access
{
public:
    Access();
    Access(const Access& access);
    void add_new_trader(GtkApplication *app, gpointer user_data);
    void on_add_new_trader(GtkButton *button, gpointer user_data);
};
#endif