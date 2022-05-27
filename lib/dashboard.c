#include "dashboard.h"

void open_website(GtkWidget *widget, GtkWidget *window){
    if (g_strcmp0(gtk_widget_get_name(widget), "trailer1") == 0){
        gtk_show_uri_on_window(NULL, "https://www.youtube.com/watch?v=jtDRXPTZT-M", GDK_CURRENT_TIME, NULL);
    }
    else if (g_strcmp0(gtk_widget_get_name(widget), "trailer2") == 0){
        gtk_show_uri_on_window(NULL, "https://www.youtube.com/watch?v=06pl59umDlw", GDK_CURRENT_TIME, NULL);
    }
    else if (g_strcmp0(gtk_widget_get_name(widget), "trailer3") == 0){
        gtk_show_uri_on_window(NULL, "https://www.youtube.com/watch?v=cSp1dM2Vj48", GDK_CURRENT_TIME, NULL);
    }
    else if (g_strcmp0(gtk_widget_get_name(widget), "trailer4") == 0){
        gtk_show_uri_on_window(NULL, "https://www.youtube.com/watch?v=ZjIquMdpsSA&t=5s", GDK_CURRENT_TIME, NULL);
    }
}

void display_dashboard(GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));

    // WIDGET
    GtkWidget *windowDashboard;
    GtkWidget *layoutDashboard;
    GtkWidget *bgDashboard;

    // LABEL
    GtkWidget *labelTitle;
    GtkWidget *labelPlaying;

    // BUTTON
    GtkWidget *buttonDashtoLogin;
    GtkWidget *buttonPembelian;
    GtkWidget *buttonPemilihanKursi;
    GtkWidget *buttonPembayaran;
    GtkWidget *buttonInformasi;

    GtkWidget *buttonTrailerFilm1;
    GtkWidget *buttonTrailerFilm2;
    GtkWidget *buttonTrailerFilm3;
    GtkWidget *buttonTrailerFilm4;

    // ICON
    GdkPixbuf *iconDashboard;

    gtk_init(NULL, NULL);

    // WINDOW
    windowDashboard = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowDashboard), "Dashboard D'MOV1E");
    gtk_window_set_default_size(GTK_WINDOW(windowDashboard), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowDashboard), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutDashboard = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowDashboard), layoutDashboard);

    // ICON
    iconDashboard = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowDashboard), iconDashboard);

    // BACKGROUND
    bgDashboard = gtk_image_new_from_file("src/image/dashboard.png");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), bgDashboard, 0, 0);

    // LABEL
    labelTitle = gtk_label_new("Dashboard");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), labelTitle, 270, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");

    labelPlaying = gtk_label_new("Playing Now!");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), labelPlaying, 880, 50);
    gtk_widget_set_name(labelPlaying, "labelPlaying");

    // BUTTON
    buttonDashtoLogin = gtk_button_new_with_label("Logout");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonDashtoLogin, 140, 565);
    gtk_widget_set_size_request(buttonDashtoLogin, 400, 75);


    buttonPembelian = gtk_button_new_with_label("Pembelian");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonPembelian, 140, 165);
    gtk_widget_set_size_request(buttonPembelian, 400, 75);

    buttonPemilihanKursi = gtk_button_new_with_label("Pemilihan Kursi");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonPemilihanKursi, 140, 265);
    gtk_widget_set_size_request(buttonPemilihanKursi, 400, 75);

    buttonPembayaran = gtk_button_new_with_label("Pembayaran");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonPembayaran, 140, 365);
    gtk_widget_set_size_request(buttonPembayaran, 400, 75);

    buttonInformasi = gtk_button_new_with_label("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonInformasi, 140, 465);
    gtk_widget_set_size_request(buttonInformasi, 400, 75);

    // FILM POSTER
    buttonTrailerFilm1 = gtk_button_new_with_label("");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonTrailerFilm1, 800, 170);
    gtk_widget_set_name(buttonTrailerFilm1, "trailer1");
    gtk_widget_set_size_request(buttonTrailerFilm1, 141, 210);

    buttonTrailerFilm2 = gtk_button_new_with_label("");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonTrailerFilm2, 980, 170);
    gtk_widget_set_name(buttonTrailerFilm2, "trailer2");
    gtk_widget_set_size_request(buttonTrailerFilm2, 141, 210);

    buttonTrailerFilm3 = gtk_button_new_with_label("");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonTrailerFilm3, 800, 425);
    gtk_widget_set_name(buttonTrailerFilm3, "trailer3");
    gtk_widget_set_size_request(buttonTrailerFilm3, 141, 210);

    buttonTrailerFilm4 = gtk_button_new_with_label("");
    gtk_layout_put(GTK_LAYOUT(layoutDashboard), buttonTrailerFilm4, 980, 425);
    gtk_widget_set_name(buttonTrailerFilm4, "trailer4");
    gtk_widget_set_size_request(buttonTrailerFilm4, 141, 210);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowDashboard);

    // SIGNAL
    g_signal_connect(windowDashboard, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonDashtoLogin, "clicked", G_CALLBACK(handle_display_sign_in), windowDashboard);
    g_signal_connect(buttonPembelian, "clicked", G_CALLBACK(handle_display_dashboard_pembelian), windowDashboard);
    g_signal_connect(buttonPemilihanKursi, "clicked", G_CALLBACK(handle_display_dashboard_pemilihan_kursi), windowDashboard);
    g_signal_connect(buttonPembayaran, "clicked", G_CALLBACK(handle_display_dashboard_pembayaran), windowDashboard);
    g_signal_connect(buttonInformasi, "clicked", G_CALLBACK(handle_display_dashboard_informasi), windowDashboard);
    // TRAILER SIGNAL
    g_signal_connect(buttonTrailerFilm1, "clicked", G_CALLBACK(open_website), windowDashboard);
    g_signal_connect(buttonTrailerFilm2, "clicked", G_CALLBACK(open_website), windowDashboard);
    g_signal_connect(buttonTrailerFilm3, "clicked", G_CALLBACK(open_website), windowDashboard);
    g_signal_connect(buttonTrailerFilm4, "clicked", G_CALLBACK(open_website), windowDashboard);

    gtk_main();
}
void handle_display_dashboard(GtkWidget *widget, GtkWidget *window){
    display_dashboard(window);
}
