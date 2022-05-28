#include "dashboard_informasi_aplikasi.h"

void display_informasi_aplikasi(){
    GtkWidget *windowInfoAplikasi;
    GtkWidget *layoutInfoAplikasi;
    GtkWidget *bgInfoAplikasi;

    GtkWidget *buttonToInformasi;


    // ICON
    GdkPixbuf *iconInfoAplikasi;

    gtk_init(NULL, NULL);

    // WINDOW
    windowInfoAplikasi = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowInfoAplikasi), "Informasi Aplikasi");
    gtk_window_set_default_size(GTK_WINDOW(windowInfoAplikasi), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowInfoAplikasi), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutInfoAplikasi = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowInfoAplikasi), layoutInfoAplikasi);

    // ICON
    iconInfoAplikasi = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowInfoAplikasi), iconInfoAplikasi);

    // BACKGROUND
    bgInfoAplikasi = gtk_image_new_from_file("src/image/dashboard_informasi_aplikasi.png");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAplikasi), bgInfoAplikasi, 0, 0);

    // BUTTON
    buttonToInformasi = gtk_button_new_with_label("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAplikasi), buttonToInformasi, 50,660);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_informasi_aplikasi.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowInfoAplikasi);

    // SIGNAL
    g_signal_connect(windowInfoAplikasi, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonToInformasi, "clicked", G_CALLBACK(handle_display_dashboard_informasi), windowInfoAplikasi);

    gtk_main();
}

void handle_display_informasi_aplikasi(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_informasi_aplikasi();
}
