#include <gtk/gtk.h>
#include <cairo.h>
#include <stdint.h>

static void sdraw(GtkDrawingArea *area, cairo_t *cs, int width, int height, gpointer sdata){
    const uint16_t points = 50;
    double *data = (double *) sdata;
    double spacing = (double)(width - 100) / (points - 1);

    // [@.. max scaling]
    double max_s = 0;
    for(int s = 0; s < width; ++s)
        if(data[s] > max_s) max_s = data[s];

    // [@.. background]
    cairo_set_source_rgb(cs, 0.1,0.1,0.1);
    cairo_paint(cs);

    // [@.. grid sbg]
    cairo_set_source_rgb(cs, 0.2,0.2,0.2);
    cairo_set_line_width(cs, 5);

    // [@.. vertical lines]
    for(int s = 50; s <= width - 50; s += 50){
        cairo_move_to(cs, s, 50);
        cairo_line_to(cs, s, height - 50);
    }

    // [@.. horizontal lines]
    for(int s = 50; s <= height - 50; s += 50) {
        cairo_move_to(cs, 50, s);
        cairo_line_to(cs, width - 50, s);
    }
    cairo_stroke(cs);

    // [@.. init axes]
    cairo_set_source_rgb(cs, 0.6,0.6,0.6);
    cairo_set_line_width(cs, 5);
    
    // [@.. y axis]
    cairo_move_to(cs, 50, height - 50);
    cairo_line_to(cs, width - 50, height - 50);

    // [@.. x axis]
    cairo_move_to(cs, 50, height - 50);
    cairo_line_to(cs, 50, 50);
    cairo_stroke(cs);

    // [@.. axes font]
    cairo_set_source_rgb(cs, 0.7,0.7,0.7);
    cairo_select_font_face(cs, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cs, 12.0);

    // [@.. y axis labels]
    for(int s = 0; s < 20; ++s){
        double val = max_s * s / 20;
        char label[16]; snprintf(label, sizeof(label), "%.0f", val);
        cairo_move_to(cs, 10, height - 50 - (s * (height - 100) / 20));
        cairo_show_text(cs, label);
    }

    // [@.. x axis labels]
    for(int s = 0; s < points; ++s){
        char label[16]; snprintf(label, sizeof(label), "%d", s);
        cairo_move_to(cs, 50 + s * spacing, height - 25);
        cairo_show_text(cs, label);
    }
}