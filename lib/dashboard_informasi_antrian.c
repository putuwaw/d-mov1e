#include "dashboard_informasi_antrian.h"

void display_informasi_antrian(){
     // WIDGET
    GtkWidget *windowInfoAntrian;
    GtkWidget *layoutInfoAntrian;
    GtkWidget *bgInfoAntrian;

    GtkWidget *buttonToInformasi;

    // LABEL
    GtkWidget *labelQueueSelect;
    GtkWidget *labelQueuePay;
    GtkWidget *labelTitle;

    GtkWidget *labelQueueName;
    GtkWidget *labelEmptyData;

    // ICON
    GdkPixbuf *iconInfoAntrian;

    gtk_init(NULL, NULL);

    // WINDOW
    windowInfoAntrian = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowInfoAntrian), "Informasi Antrian");
    gtk_window_set_default_size(GTK_WINDOW(windowInfoAntrian), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowInfoAntrian), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutInfoAntrian = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowInfoAntrian), layoutInfoAntrian);

    // ICON
    iconInfoAntrian = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowInfoAntrian), iconInfoAntrian);

    // BACKGROUND
    bgInfoAntrian = gtk_image_new_from_file("src/image/dashboard_informasi_antrian.png");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), bgInfoAntrian, 0, 0);

    // LABEL
    labelTitle = gtk_label_new("Informasi Antrian");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelTitle, 500, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");

    // BUTTON
    buttonToInformasi = gtk_button_new_with_label("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), buttonToInformasi, 570,660);

    labelQueueName = gtk_label_new("Antrian Pemilihan Kursi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelQueueName, 300,200);

    labelQueueName = gtk_label_new("Antrian Pembayaran Tiket");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelQueueName, 750,200);

    labelEmptyData = gtk_label_new("Belum ada data");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelEmptyData, 300,250);

    int x = 300, y = 250;

    FILE *f;
    f = fopen("data/user_ticket_data.txt", "r");
    int counterLine;

    char ch, compUsr[100];
    int counter, idx;

    reset_string(compUsr, 100);
    counter = 0;
    counterLine = 0;
    idx = 1;
    bool hasDeleted = false;

    while(!feof(f) && counterLine < 10){
        ch = fgetc(f);
        if (ch == '\n'){
            counterLine++;
            labelQueueSelect = gtk_label_new(compUsr);
            if (!hasDeleted){
                gtk_widget_destroy(labelEmptyData);
                hasDeleted = true;
            }
            gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelQueueSelect, x,y);
            y += 50;

            reset_string(compUsr, 100);
            counter = 0;
            idx = 1;
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
        }

    }
    fclose(f);

    labelEmptyData = gtk_label_new("Belum ada data");
    gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelEmptyData, 750,250);
    hasDeleted = false;

    x = 750, y = 250;
    f = fopen("data/user_invoice_data.txt", "r");

    reset_string(compUsr, 100);
    counter = 0;
    counterLine = 0;
    idx = 1;

    while(!feof(f) && counterLine < 10){
        ch = fgetc(f);
        if (ch == '\n'){
            counterLine++;
            labelQueuePay = gtk_label_new(compUsr);
            if (!hasDeleted){
                gtk_widget_destroy(labelEmptyData);
                hasDeleted = true;
            }
            gtk_layout_put(GTK_LAYOUT(layoutInfoAntrian), labelQueuePay, x,y);
            y += 50;

            reset_string(compUsr, 100);
            counter = 0;
            idx = 1;
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
        }

    }
    fclose(f);


    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_informasi_antrian.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowInfoAntrian);

    // SIGNAL
    g_signal_connect(windowInfoAntrian, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonToInformasi, "clicked", G_CALLBACK(handle_display_dashboard_informasi), windowInfoAntrian);

    gtk_main();
}

void handle_display_informasi_antrian(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_informasi_antrian();
}
