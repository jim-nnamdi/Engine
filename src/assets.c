#include <gtk/gtk.h>
#include <cairo.h>
#include "../includes/assets.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 200
#define POINTS 50

void generate_trend_data(double *data, int size)
{
    srand(time(NULL)); // Seed for random number generation
    data[0] = 1000.0;  // Starting value
    for (int i = 1; i < size; i++)
    {
        double change = ((rand() % 200) - 100) * 0.5; // Random change in range [-50, 50]
        data[i] = data[i - 1] + change;
        if (data[i] < 0)
            data[i] = 0; // Prevent negative values
    }
}

/* Draw the crypto trend graph */
static void
on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
    double *data = (double *)user_data;
    double x_spacing = (double)(width - 100) / (POINTS - 1); // Spacing between points
    double max_value = 0;

    /* Find maximum value for scaling */
    for (int i = 0; i < POINTS; i++)
    {
        if (data[i] > max_value)
        {
            max_value = data[i];
        }
    }

    /* Draw background */
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Dark gray
    cairo_paint(cr);

    /* Draw grid */
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8); // Slightly lighter gray for grid
    cairo_set_line_width(cr, 0.5);

    // Vertical grid lines
    for (int i = 50; i <= width - 50; i += 50)
    {
        cairo_move_to(cr, i, 50);
        cairo_line_to(cr, i, height - 50);
    }

    // Horizontal grid lines
    for (int i = 50; i <= height - 50; i += 50)
    {
        cairo_move_to(cr, 50, i);
        cairo_line_to(cr, width - 50, i);
    }
    cairo_stroke(cr);

    /* Draw axes */
    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); // Light gray
    cairo_set_line_width(cr, 1.0);

    // X-axis
    cairo_move_to(cr, 50, height - 50);
    cairo_line_to(cr, width - 50, height - 50);
    cairo_stroke(cr);

    // Y-axis
    cairo_move_to(cr, 50, height - 50);
    cairo_line_to(cr, 50, 50);
    cairo_stroke(cr);

    /* Draw labels on axes */
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // White
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.0);

    // Y-axis labels
    for (int i = 0; i <= 5; i++)
    {
        double value = max_value * i / 5;
        char label[16];
        snprintf(label, sizeof(label), "%.0f", value);

        cairo_move_to(cr, 10, height - 50 - (i * (height - 100) / 5));
        cairo_show_text(cr, label);
    }

    // X-axis labels
    for (int i = 0; i < POINTS; i += 5)
    {
        char label[16];
        snprintf(label, sizeof(label), "%d", i);

        cairo_move_to(cr, 50 + i * x_spacing, height - 30);
        cairo_show_text(cr, label);
    }

    /* Draw trend line */
    cairo_set_source_rgb(cr, 0.2, 0.7, 0.9); // Cyan
    cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, 50, height - 50 - (data[0] / max_value) * (height - 100));
    for (int i = 1; i < POINTS; i++)
    {
        double x = 50 + i * x_spacing;
        double y = height - 50 - (data[i] / max_value) * (height - 100);
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);
}

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
        // GtkWidget *content = gtk_label_new(items[i]);
        GtkWidget *window;
        GtkWidget *drawing_area;

        /* Generate trend data */
        static double trend_data[POINTS];
        generate_trend_data(trend_data, POINTS);

        /* Create a new window */
        window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "Crypto Trend Graph with Netty Background 📈");
        gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);

        /* Create a drawing area */
        drawing_area = gtk_drawing_area_new();
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), on_draw, trend_data, NULL);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), drawing_area, label);
        // gtk_notebook_append_page(GTK_NOTEBOOK(notebook), gtk_label_new("Testing"), label);
    }
    g_signal_connect(listbox_sidebar, "row-activated", G_CALLBACK(on_sidebar_activated), notebook);
    gtk_window_present(GTK_WINDOW(window));
}
