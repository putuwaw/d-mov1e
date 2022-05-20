#include "lib/d_mov1e.h"

int main(){
    GtkWidget *window;
    GtkWidget *layout;
    GtkWidget *image;
    GtkWidget *label;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Final Project Struktur Data");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (window), layout);

    image = gtk_image_new_from_file("src/image/sample_image.jpg");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    label = gtk_label_new("Hello World");
    gtk_layout_put(GTK_LAYOUT(layout), label, 0, 0);

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/sample_css.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    return 0;
}
