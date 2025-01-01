#include <gtk/gtk.h>
#include <cairo.h>

#define POINTS 50

void graph(GtkDrawingArea *area, cairo_t* cr, int width, int height, gpointer userdata){
    double* data = (double *)userdata;
    double xspace = (double)(width - 100)/(POINTS - 1);
    double maxscale = 0;
    for (int i =0; i < POINTS; ++i)
        if (data[i] > maxscale) maxscale = data[i];

    // background
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // grid
    cairo_set_source_rgb(cr, 0.8, 0.8,0.8);
    cairo_set_line_width(cr, 0.5);

    // up to down (vertical grid lines)
    for(int i = 50; i < width - 50; i += 50){
        cairo_move_to(cr, i, 50); 
        cairo_line_to(cr, i, height - 50);
    }

    // left to right (horizontal grid lines)
    for(int i = 50; i < height - 50; i += 50){
        cairo_move_to(cr, 50, i);
        cairo_line_to(cr, width - 50, i);
    }
    cairo_stroke(cr);
}