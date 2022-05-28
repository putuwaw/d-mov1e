#include "dashboard_informasi.h"

void display_dashboard_informasi(){
    // WIDGET
    GtkWidget *windowDashInformasi;
    GtkWidget *layoutDashInformasi;
    GtkWidget *bgDashInformasi;

    // BUTTON
    GtkWidget *buttonAbout;
    GtkWidget *buttonInfoQueue;
    GtkWidget *buttonHighTransaction;
    GtkWidget *buttonTransactionHistory;

    GtkWidget *buttonToDash;
    GtkWidget *labelTitle;

    // ICON
    GdkPixbuf *iconDashInformasi;

    gtk_init(NULL, NULL);

    // WINDOW
    windowDashInformasi = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowDashInformasi), "Informasi");
    gtk_window_set_default_size(GTK_WINDOW(windowDashInformasi), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowDashInformasi), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutDashInformasi = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowDashInformasi), layoutDashInformasi);

    // ICON
    iconDashInformasi = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowDashInformasi), iconDashInformasi);

    // BACKGROUND
    bgDashInformasi = gtk_image_new_from_file("src/image/dashboard_informasi.png");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), bgDashInformasi, 0, 0);

     // label
    labelTitle = gtk_label_new("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), labelTitle, 570, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");


    // BUTTON
    buttonToDash = gtk_button_new_with_label("Dashboard");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), buttonToDash, 50,550);
    gtk_widget_set_size_request(buttonToDash, 540, 70);

    buttonInfoQueue = gtk_button_new_with_label("Info Antrian");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), buttonInfoQueue, 50,150);
    gtk_widget_set_size_request(buttonInfoQueue, 540, 70);


    buttonHighTransaction = gtk_button_new_with_label("Pembeli Terbaik");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), buttonHighTransaction, 50,350);
    gtk_widget_set_size_request(buttonHighTransaction, 550, 70);


    buttonAbout = gtk_button_new_with_label("Tentang Aplikasi");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), buttonAbout, 50,450);
    gtk_widget_set_size_request(buttonAbout, 540, 70);


    buttonTransactionHistory = gtk_button_new_with_label("Riwayat Transaksi");
    gtk_layout_put(GTK_LAYOUT(layoutDashInformasi), buttonTransactionHistory, 50,250);
    gtk_widget_set_size_request(buttonTransactionHistory, 550, 70);


    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_informasi.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowDashInformasi);

    // SIGNAL
    g_signal_connect(windowDashInformasi, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonToDash, "clicked", G_CALLBACK(handle_display_dashboard), windowDashInformasi);

    g_signal_connect(buttonHighTransaction, "clicked", G_CALLBACK(handle_display_informasi_pembeli), windowDashInformasi);
    g_signal_connect(buttonInfoQueue, "clicked", G_CALLBACK(handle_display_informasi_antrian), windowDashInformasi);
    g_signal_connect(buttonAbout, "clicked", G_CALLBACK(handle_display_informasi_aplikasi), windowDashInformasi);
    g_signal_connect(buttonTransactionHistory, "clicked", G_CALLBACK(handle_display_informasi_transaksi), windowDashInformasi);

    gtk_main();
}

void handle_display_dashboard_informasi(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_dashboard_informasi();
}
