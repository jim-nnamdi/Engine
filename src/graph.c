#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH   800
#define HEIGHT  600
#define POINTS  50

/* Generate random fluctuation data */
void generate_trend_data(double *data, int size) {
    srand(time(NULL)); // Seed for random number generation
    data[0] = 1000.0;  // Starting value
    for (int i = 1; i < size; i++) {
        double change = ((rand() % 200) - 100) * 0.5; // Random change in range [-50, 50]
        data[i] = data[i - 1] + change;
        if (data[i] < 0) data[i] = 0; // Prevent negative values
    }
}

/* Draw the crypto trend graph */
static void
on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    double *data = (double *)user_data;
    double x_spacing = (double)(width - 100) / (POINTS - 1); // Spacing between points
    double max_value = 0;

    /* Find maximum value for scaling */
    for (int i = 0; i < POINTS; i++) {
        if (data[i] > max_value) {
            max_value = data[i];
        }
    }

    /* Draw background */
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); // Dark gray
    cairo_paint(cr);

    /* Draw grid */
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2); // Slightly lighter gray for grid
    cairo_set_line_width(cr, 0.5);

    // Vertical grid lines
    for (int i = 50; i <= width - 50; i += 50) {
        cairo_move_to(cr, i, 50);
        cairo_line_to(cr, i, height - 50);
    }

    // Horizontal grid lines
    for (int i = 50; i <= height - 50; i += 50) {
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
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.0);

    // Y-axis labels
    for (int i = 0; i <= 5; i++) {
        double value = max_value * i / 5;
        char label[16];
        snprintf(label, sizeof(label), "%.0f", value);

        cairo_move_to(cr, 10, height - 50 - (i * (height - 100) / 5));
        cairo_show_text(cr, label);
    }

    // X-axis labels
    for (int i = 0; i < POINTS; i += 5) {
        char label[16];
        snprintf(label, sizeof(label), "%d", i);

        cairo_move_to(cr, 50 + i * x_spacing, height - 30);
        cairo_show_text(cr, label);
    }

    /* Draw trend line */
    cairo_set_source_rgb(cr, 0.2, 0.7, 0.9); // Cyan
    cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, 50, height - 50 - (data[0] / max_value) * (height - 100));
    for (int i = 1; i < POINTS; i++) {
        double x = 50 + i * x_spacing;
        double y = height - 50 - (data[i] / max_value) * (height - 100);
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
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
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.crypto", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
