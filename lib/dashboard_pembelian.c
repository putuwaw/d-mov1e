#include "dashboard_pembelian.h"

// WINDOW
GtkWidget *windowDashPembelian;

// LAYOUT
GtkWidget *layoutDashPembelian;

// LABEL
GtkWidget *labelTimeNow;

// INPUT
GtkWidget *entryNamaPembeli;
GtkWidget *entryTiketDibeli;

// COMBOBOX
GtkWidget *comboBox;
GtkWidget *comboBoxWaktu;

bool exitDashPembelian;

void reset_window(GtkWidget *widget){
    exitDashPembelian = true;
}

void combo_selected(GtkWidget *comboBox){
    // GET ACTIVE TEXT FROM COMBOBOX
    gchar *getFilmDipilih = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBox));
    char chrGetFilmDipilih[50];
    reset_string(chrGetFilmDipilih, 50);
    sprintf(chrGetFilmDipilih, "%s", getFilmDipilih);

    // RESET COMBOBOX WAKTU
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(comboBoxWaktu));

    // ADD COMBOBOX WAKTU WITH NEW DATA ACCORDING TO ACTIVE TEXT
    int i,j;
    for (i =0 ; i< 4; i++){
        // FIND THE ACTIVE TEXT IN THE ARRAY
        char compFilmDipilih[50];
        reset_string(compFilmDipilih, 50);
        strcpy(compFilmDipilih, FilmName[i]);
        if(strcmp(compFilmDipilih, chrGetFilmDipilih) == 0){
            // IF FOUND
            for (j =0; j < 3 ; j++){
                // ADD COMBOBOX WAKTU WITH THE DATA
                char appendText[20];
                reset_string(appendText, 20);
                strcpy(appendText, FilmTime[i][j]);
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBoxWaktu), appendText);
            }
            break;
        }
    }
    gtk_widget_show_all(windowDashPembelian);
}

void display_warn_pembelian(GtkWidget *widget, GtkWidget *window){
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_YES_NO,
            "Konfirmasi Pembelian?");
    gtk_widget_set_name(dialog, "warnDialog");
    gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
    int res = gtk_dialog_run(GTK_DIALOG(dialog));

    // GET USER INPUT IN GCHAR
    const gchar *gNamaPembeli = gtk_entry_get_text(GTK_ENTRY(entryNamaPembeli));
    const gchar *gTiketDibeli = gtk_entry_get_text(GTK_ENTRY(entryTiketDibeli));

    if (res == -8){ // YES
        char chrWaktuFilm[20];
        reset_string(chrWaktuFilm, 20);

        // TIME CALC
        time_t rawtime;
        struct tm *timeInfo;
        time(&rawtime);
        timeInfo = localtime(&rawtime);
        int timeNow, timeCompare = 0;
        timeNow = (10000 * timeInfo->tm_hour) + (100*timeInfo->tm_min) + timeInfo->tm_sec;

        // GET TIME FILM
        strcpy(chrWaktuFilm, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxWaktu)));

        // CONVERT TIME STRING TO INT FOR COMPARISON WITH TIME NOW
        int i;
        for (i = 0; i < 5; i++){
            if (chrWaktuFilm[i] == '.'){
                continue;
            }
            else{
                int temp = chrWaktuFilm[i] - '0';
                timeCompare = timeCompare * 10 + temp;
            }
        }
        timeCompare *= 100;

        // ERROR DETECTION
        if (g_strcmp0(gNamaPembeli, "") == 0 || g_strcmp0(gTiketDibeli, "") == 0){
            display_error_dialog("Nama atau tiket tidak boleh kosong!", window);
            gtk_widget_destroy(dialog);
        }
        else if(g_strcmp0(gTiketDibeli, "0") == 0){
            display_error_dialog("Pembelian tiket minimal satu!", window);
            gtk_widget_destroy(dialog);
        }
        else if(timeCompare < timeNow){
            display_error_dialog("Waktu pembelian tiket telah ditutup!", window);
            gtk_widget_destroy(dialog);
        }
        else{ // NO ERROR DETECTED
            char chrTiketDibeli[5];
            char chrFilmDipilih[50];
            reset_string(chrFilmDipilih, 50);
            reset_string(chrTiketDibeli, 5);

            // CONVERT GCHAR TO CHAR
            strcpy(chrFilmDipilih, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBox)));
            sprintf(chrTiketDibeli, "%s", gTiketDibeli);

            // CONVERT STRING TIKET TO INT
            int iTiketDibeli = 0, i = 0;
            while(chrTiketDibeli[i] != '\0'){
                int temp = chrTiketDibeli[i] - '0';
                iTiketDibeli = iTiketDibeli * 10 + temp;
                i++;
            }

            /* COUNT AVAILABLE CHAIR */
            FILE *f;
            f = fopen("data/studio_chair_data.txt", "r");
            char ch;
            int counterLine = 0, counterStudio = 0, temp = 0;
            while(!feof(f)){
                ch = fgetc(f);
                if (ch == '\n'){
                    AvailableChair[counterStudio][counterLine] = temp;
                    temp = 0;
                    counterLine++;
                    if (counterLine % 4 == 0){
                        counterLine = 0;
                        counterStudio++;
                    }
                }
                else{
                    if (ch == '0'){
                        temp++;
                    }
                }
            }
            /* END OF COUNT AVAILABLE CHAIR */

            int tempStudioFilm, tempStdTime;
            bool isDoraemon = false;

            // COMPARISON
            if (strcmp("KKN Desa Penari", chrFilmDipilih) == 0){
                tempStudioFilm = 0;
            }
            else if (strcmp("The Throne", chrFilmDipilih) == 0){
                tempStudioFilm = 1;
            }
            else if (strcmp("Ready Player One", chrFilmDipilih) == 0){
                tempStudioFilm = 2;
            }
            else{
                isDoraemon = true;
                if (strcmp("09.00-10.30", chrWaktuFilm) == 0){
                    tempStudioFilm = 2;
                    tempStdTime = 0;
                }
                else if(strcmp("10.00-11.30", chrWaktuFilm) == 0){
                    tempStudioFilm = 1;
                    tempStdTime = 1;
                }
                else if(strcmp("12.00-13.30", chrWaktuFilm) == 0){
                    tempStudioFilm = 0;
                    tempStdTime = 2;
                }
            }

            // DORAEMON PLAYS IN DIFFERENT STUDIO
            if (!isDoraemon){
                int i;
                for (i = 0; i < 4; i++){
                    if (strcmp(chrWaktuFilm, StudioTime[tempStudioFilm][i]) == 0){
                        tempStdTime = i;
                        break;
                    }
                }
            }

            // COMPARE TICKET WITH CHAIR AVAILABLE
            if (AvailableChair[tempStudioFilm][tempStdTime] < iTiketDibeli){
                display_error_dialog("Jumlah tiket melebihi batas!", window);
                gtk_widget_destroy(dialog);
            }
            else{ // AVAILABLE
                // CREATE NEW QUEUE
                qUser = (Queue*)malloc(sizeof(Queue));

                // USERNAME DAN TIKET
                strcpy(qUser->qUsername, gNamaPembeli);
                qUser->qTiketDibeli = iTiketDibeli;

                // TIME
                time_t rawtime;
                struct tm *timeInfo;
                time(&rawtime);
                timeInfo = localtime(&rawtime);
                char chrTimeNow[100];
                reset_string(chrTimeNow, 100);
                strcpy(chrTimeNow, timeToStr(timeInfo));
                strcpy(qUser->qBuyTime, chrTimeNow);

                // STUDIO FILM
                bool isDoraemon = false;
                if (strcmp("KKN Desa Penari", chrFilmDipilih) == 0){
                    qUser->qStudioFilm = 1;
                }
                else if (strcmp("The Throne", chrFilmDipilih) == 0){
                    qUser->qStudioFilm = 2;
                }
                else if (strcmp("Ready Player One", chrFilmDipilih) == 0){
                    qUser->qStudioFilm = 3;
                }
                else{
                    isDoraemon = true;
                    if (strcmp("09.00-10.30", chrWaktuFilm) == 0){
                        qUser->qStudioFilm = 3;
                        qUser->qStudioTime = 1;
                    }
                    else if(strcmp("10.00-11.30", chrWaktuFilm) == 0){
                        qUser->qStudioFilm = 2;
                        qUser->qStudioTime = 2;
                    }
                    else if(strcmp("12.00-13.30", chrWaktuFilm) == 0){
                        qUser->qStudioFilm = 1;
                        qUser->qStudioTime = 3;
                    }
                }

                // SEARCH FOR STUDIO TIME
                int tempStudioTime  = qUser->qStudioFilm;
                tempStudioTime--;

                // DORAEMON PLAYS IN DIFFERENT STUDIO
                if (!isDoraemon){
                    int i;
                    for (i = 0; i < 4; i++){
                        if (strcmp(chrWaktuFilm, StudioTime[tempStudioTime][i]) == 0){
                            qUser->qStudioTime = (i+1);
                            break;
                        }
                    }
                }

                // COPY OTHER INFORMATION
                strcpy(qUser->qFilmName, chrFilmDipilih);
                strcpy(qUser->qFilmTime, chrWaktuFilm);
                strcpy(qUser->qFilmChair, "");
                // NEXT
                qUser->qNext = NULL;

                // ENQUEUE
                if (qFront == NULL){
                    qFront = qUser;
                }
                else{
                    Queue *prev = qFront;
                    while(prev->qNext != NULL){
                        prev = prev->qNext;
                    }
                    prev->qNext = qUser;
                }

                /* FILE OPERATION */
                FILE *f;
                f = fopen("data/user_ticket_data.txt", "a");
                fprintf(f, "%s|%d|%s|%d|%d|%s|%s|%s\n",
                        qUser->qUsername, qUser->qTiketDibeli, qUser->qBuyTime,
                        qUser->qStudioFilm, qUser->qStudioTime,
                        qUser->qFilmName, qUser->qFilmTime, qUser->qFilmChair);
                fclose(f);
                /* END OF FILE OPERATION */

                // RESET INPUT
                gtk_entry_set_text(GTK_ENTRY(entryNamaPembeli), "");
                gtk_entry_set_text(GTK_ENTRY(entryTiketDibeli), "");
                gtk_widget_destroy(dialog);

                // SUCCESS
                display_info_dialog("Pembelian berhasil", window);
            }
        }
    }
    else{
        gtk_widget_destroy(dialog);
    }
}

gboolean time_handler(){
    gchar buf[100];
    if (exitDashPembelian){
        return false;
    }
    GDateTime *now = g_date_time_new_now_local();
    gchar *my_time = g_date_time_format(now, "%H:%M:%S WITA");
    sprintf(buf, "%s", my_time);
    gtk_widget_destroy(labelTimeNow);
    labelTimeNow = gtk_label_new(buf);
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelTimeNow, 1000, 100);
    gtk_widget_show_all(windowDashPembelian);
    g_free(my_time);
    g_date_time_unref(now);
    return true;
}

void display_dashboard_pembelian(){
    exitDashPembelian = false;

    // TIME CALC
    time_t rawtime;
    struct tm *timeInfo;
    time(&rawtime);
    timeInfo = localtime(&rawtime);
    char chrTimeNow[50];
    reset_string(chrTimeNow, 20);
    sprintf(chrTimeNow, "%.2d:%.2d.%.2d WITA", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    int timeNow;
    timeNow = (10000 * timeInfo->tm_hour) + (100*timeInfo->tm_min) + timeInfo->tm_sec;

    // TIME STATUS
    char status[10];
    reset_string(status, 10);

    // BACKGROUND
    GtkWidget *bgDashPembelian;

    // LABEL
    GtkWidget *labelTitle;
    GtkWidget *labelNamaPembeli;
    GtkWidget *labelTiketDibeli;
    GtkWidget *labelFilmDipilih;
    GtkWidget *labelWaktuDipilih;

    // BUTTON
    GtkWidget *buttonBackToDash;
    GtkWidget *buttonConfirmBeli;
    GtkWidget *buttonFilmTitle;
    GtkWidget *buttonFilmTime;

    // ICON
    GdkPixbuf *iconDashPembelian;

    gtk_init(NULL, NULL);

    // WINDOW
    windowDashPembelian = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowDashPembelian), "PEMBELIAN TIKET");
    gtk_window_set_default_size(GTK_WINDOW(windowDashPembelian), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowDashPembelian), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutDashPembelian = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowDashPembelian), layoutDashPembelian);

    // ICON
    iconDashPembelian = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowDashPembelian), iconDashPembelian);

    // BACKGROUND
    bgDashPembelian = gtk_image_new_from_file("src/image/dashboard_pembelian.png");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), bgDashPembelian, 0, 0);

    // LABEL
    labelTitle = gtk_label_new("Menu Pembelian");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelTitle, 550, 75);
    gtk_widget_set_name(labelTitle, "labelTitle");

    labelNamaPembeli = gtk_label_new("Nama");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelNamaPembeli, 150, 150);

    labelTiketDibeli = gtk_label_new("Jumlah Tiket:");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelTiketDibeli, 150, 250);

    labelTimeNow = gtk_label_new(chrTimeNow);
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelTimeNow, 1000, 100);

    // ENTRY
    entryNamaPembeli = gtk_entry_new();
    gtk_widget_set_size_request(entryNamaPembeli, 250, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNamaPembeli), "Masukkan nama pembeli...");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), entryNamaPembeli, 150, 190);

    entryTiketDibeli = gtk_entry_new();
    gtk_widget_set_size_request(entryTiketDibeli, 250, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryTiketDibeli), "Masukkan jumlah tiket...");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), entryTiketDibeli, 150, 290);

    // BUTTON
    buttonBackToDash = gtk_button_new_with_label("Dashboard");
    gtk_widget_set_name(buttonBackToDash, "buttonAction");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), buttonBackToDash, 150, 600);

    buttonConfirmBeli = gtk_button_new_with_label("Buat Pesanan");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), buttonConfirmBeli, 1000, 600);
    gtk_widget_set_name(buttonConfirmBeli, "buttonAction");

    /* FILM TIME DISPLAY */
    int i,j;
    gint x = 550, y = 200; // POSITION
    for (i =0; i < 4; i++){
        x = 550;
        for (j = 0; j < 3; j++){
            char filmTime[10];
            reset_string(filmTime, 10);
            strcpy(filmTime, FilmTime[i][j]);
            // MAKE NEW BUTTON
            buttonFilmTime = gtk_button_new_with_label(filmTime);
            gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), buttonFilmTime, x, y);
            gtk_widget_set_size_request(buttonFilmTime, 35, 20);
            // COMPARISON TIME
            int k, compareTime = 0;
            for (k = 0; k < 5; k++){
                if (filmTime[k] == '.'){
                    continue;
                }
                else{
                    int temp = filmTime[k] - '0';
                    compareTime = compareTime * 10 + temp;
                }
            }
            compareTime *= 100;
            reset_string(status, 10);
            strcpy(status, "avail");
            if (compareTime < timeNow){
                reset_string(status, 10);
                strcpy(status, "late");
            }
            // GIVE COLOR TO BUTTON
            gtk_widget_set_name(buttonFilmTime, status);
            x += 145;
        }
        y += 100;
    }
    /* END OF FILM TIME DISPLAY */

    /* FILM NAME DISPLAY */
    y = 150;
    x = 550;
    for (i = 0; i < 4; i++){
        char filmName[50], chrTicketPrice[50];
        reset_string(filmName, 50);
        reset_string(chrTicketPrice, 50);
        strcpy(filmName, FilmName[i]);
        strcat(filmName, " - Rp. ");
        sprintf(chrTicketPrice, "%d", TicketPrice[i]);
        strcat(filmName, chrTicketPrice);
        buttonFilmTitle = gtk_button_new_with_label(filmName);
        gtk_widget_set_name(buttonFilmTitle, "namaFilm");
        gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), buttonFilmTitle, x, y);
        gtk_widget_set_size_request(buttonFilmTitle, 425, 50);
        y += 100;
    }
    /* END OF FILM NAME DISPLAY */

    labelFilmDipilih = gtk_label_new("Pilih Film:");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelFilmDipilih, 150, 350);

    // COMBOBOX
    comboBox = gtk_combo_box_text_new();
    for (i =0 ; i< 4; i++){
        char appendText[50];
        reset_string(appendText, 50);
        strcpy(appendText, FilmName[i]);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), appendText);
    }
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian),comboBox, 150,390);
    labelWaktuDipilih = gtk_label_new("Pilih Waktu:");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian), labelWaktuDipilih, 150, 440);

    // COMBOBOX WAKTU
    comboBoxWaktu = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBoxWaktu), "Silakan pilih film dahulu!");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembelian),comboBoxWaktu, 150,480);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_pembelian.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    g_timeout_add(1000, (GSourceFunc)time_handler, NULL);
    gtk_widget_show_all(windowDashPembelian);

    // SIGNAL
    g_signal_connect(windowDashPembelian, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(windowDashPembelian, "destroy", G_CALLBACK(reset_window), NULL);
    g_signal_connect(buttonBackToDash, "clicked", G_CALLBACK(handle_display_dashboard), windowDashPembelian);
    g_signal_connect(buttonConfirmBeli, "clicked", G_CALLBACK(display_warn_pembelian), windowDashPembelian);
    g_signal_connect(comboBox, "changed", G_CALLBACK(combo_selected), NULL);
    gtk_main();
}

void handle_display_dashboard_pembelian(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_dashboard_pembelian();
}
