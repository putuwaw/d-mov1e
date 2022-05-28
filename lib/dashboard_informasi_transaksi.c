#include "dashboard_informasi_transaksi.h"

void display_informasi_transaksi(){
    GtkWidget *windowInfoTransaksi;
    GtkWidget *layoutInfoTransaksi;
    GtkWidget *bgInfoTransaksi;

    GtkWidget *labelTitle;

    GtkWidget *buttonToInformasi;

    // LABEL
    GtkWidget *labelListTransaksi;
    GtkWidget *labelEmptyData1;
    GtkWidget *labelEmptyData2;
    GtkWidget *labelEmptyData3;

    // ICON
    GdkPixbuf *iconInfoTransaksi;

    bool hasDeleted;

    gtk_init(NULL, NULL);

    // WINDOW
    windowInfoTransaksi = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowInfoTransaksi), "Informasi Transaksi");
    gtk_window_set_default_size(GTK_WINDOW(windowInfoTransaksi), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowInfoTransaksi), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutInfoTransaksi = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowInfoTransaksi), layoutInfoTransaksi);

    // ICON
    iconInfoTransaksi = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowInfoTransaksi), iconInfoTransaksi);

    // BACKGROUND
    bgInfoTransaksi = gtk_image_new_from_file("src/image/dashboard_informasi_transaksi.png");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), bgInfoTransaksi, 0, 0);

     // label
    labelTitle = gtk_label_new("Informasi Transaksi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelTitle, 500, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");

    hasDeleted = false;
    labelEmptyData1 = gtk_label_new("data kosong");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelEmptyData1, 100, 200);
    labelEmptyData2 = gtk_label_new("data kosong");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelEmptyData2, 500, 200);
    labelEmptyData3 = gtk_label_new("data kosong");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelEmptyData3, 950, 200);

    // BUTTON
    buttonToInformasi = gtk_button_new_with_label("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), buttonToInformasi, 570,660);

    FILE *f;
    f = fopen("data/transaction_history.txt", "r");
    char compUsr[100], compBuyTime[50], compFilmName[50];
    reset_string(compUsr, 100);
    reset_string(compBuyTime, 50);
    reset_string(compFilmName, 50);

    char ch;
    int counter = 0, idx = 1, maxTransaction = 0, y = 200;
    while(!feof(f) && maxTransaction < 10){
        ch = fgetc(f);
        if (ch == '\n'){
            maxTransaction++;

            if (!hasDeleted){
                gtk_widget_destroy(labelEmptyData1);
                gtk_widget_destroy(labelEmptyData2);
                gtk_widget_destroy(labelEmptyData3);
                hasDeleted = true;
            }

            labelListTransaksi = gtk_label_new(compUsr);
            gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelListTransaksi, 100,y);

            labelListTransaksi = gtk_label_new(compFilmName);
            gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelListTransaksi, 500,y);

            labelListTransaksi = gtk_label_new(compBuyTime);
            gtk_layout_put(GTK_LAYOUT(layoutInfoTransaksi), labelListTransaksi, 950,y);

            y += 40;

            reset_string(compUsr, 100);
            reset_string(compBuyTime, 50);
            reset_string(compFilmName, 50);

            idx = 1;
            counter = 0;
        }
        else if (ch == '|'){
            idx++;
            counter = 0;
        }
        else{
            if (idx == 1){
                compUsr[counter] = ch;
                counter++;
            }
            else if (idx == 3){
                compBuyTime[counter] = ch;
                counter++;
            }
            else if (idx == 6){
                compFilmName[counter] = ch;
                counter++;
            }
        }
    }

    fclose(f);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_informasi_transaksi.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowInfoTransaksi);

    // SIGNAL
    g_signal_connect(windowInfoTransaksi, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonToInformasi, "clicked", G_CALLBACK(handle_display_dashboard_informasi), windowInfoTransaksi);

    gtk_main();
}

void handle_display_informasi_transaksi(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_informasi_transaksi();
}
