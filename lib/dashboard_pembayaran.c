#include "dashboard_pembayaran.h"

// WINDOW
GtkWidget *windowDashPembayaran;

// ENTRY
GtkWidget *entryDibayarPelanggan;

// LAYOUT
GtkWidget *layoutDashPembayaran;

// LABEL
GtkWidget *labelPecahanUangTersedia;
GtkWidget *labelPecahanUangKembali;

int arrUangKembalian[7];

void get_user_invoice_from_file(){
    qFrontPay = NULL;

    char compUsr[100], compBuyTime[50], compFilmName[50], compFilmTime[20], compFilmChair[50];
    int compTicket, compStudioFilm, compStudioTime;
    reset_string(compUsr, 100);
    reset_string(compBuyTime, 50);
    reset_string(compFilmName, 50);
    reset_string(compFilmTime, 20);
    reset_string(compFilmChair, 50);

    compTicket = 0;
    compStudioFilm = 0;
    compStudioTime = 0;

    FILE *f;
    f = fopen("data/user_invoice_data.txt", "r");
    char ch;
    int idx = 1, counter = 0;
    while(!feof(f)){
        ch = fgetc(f);
        if (ch == '|'){
            counter = 0;
            idx++;
        }
        else if (ch == '\n'){

            qUser = malloc(sizeof(Queue));
            strcpy(qUser->qUsername, compUsr);
            qUser->qTiketDibeli = compTicket;
            strcpy(qUser->qBuyTime, compBuyTime);

            qUser->qStudioFilm = compStudioFilm;
            qUser->qStudioTime = compStudioTime;

            strcpy(qUser->qFilmName, compFilmName);
            strcpy(qUser->qFilmTime, compFilmTime);
            strcpy(qUser->qFilmChair, compFilmChair);

            qUser->qNext = NULL;

            if (qFrontPay == NULL){
                qFrontPay = qUser;
            }
            else{
                Queue *prev;
                prev = qFrontPay;
                while(prev->qNext != NULL){
                    prev = prev->qNext;
                }
                prev->qNext = qUser;
            }

            // reset
            reset_string(compUsr, 100);
            reset_string(compBuyTime, 50);
            reset_string(compFilmName, 50);
            reset_string(compFilmTime, 20);
            reset_string(compFilmChair, 50);

            compTicket = 0;
            compStudioFilm = 0;
            compStudioTime = 0;

            counter = 0;
            idx = 1;
        }
        else{
            if (idx == 1){
                compUsr[counter] = ch;
                counter++;
            }
            else if(idx == 2){
                int tempCompTicket = ch - '0';
                compTicket = compTicket*10 + tempCompTicket;
                counter++;
            }
            else if (idx == 3){
                compBuyTime[counter] = ch;
                counter++;
            }
            else if (idx == 4){
                int tempCompStudioFilm = ch - '0';
                compStudioFilm = tempCompStudioFilm;
                counter++;
            }
            else if (idx == 5){
                int tempCompStudioTime = ch - '0';
                compStudioTime = tempCompStudioTime;
                counter++;
            }
            else if (idx == 6){
                compFilmName[counter] = ch;
                counter++;
            }
            else if (idx == 7){
                compFilmTime[counter] = ch;
                counter++;
            }
            else if (idx == 8){
                compFilmChair[counter] = ch;
                counter++;
            }
        }
    }
    fclose(f);
}

int get_uang_kembalian_rec(int money, int depth){
    if (money == 0){
        return 0;
    }
    else if(money < MoneyCurrency[depth]){
        get_uang_kembalian_rec(money, depth+1);
    }
    else{
        arrUangKembalian[depth] = money / MoneyCurrency[depth];
        get_uang_kembalian_rec(money % MoneyCurrency[depth], depth);
    }
    return 0;
}


void count_cash_back(GtkWidget *widget, GtkWidget *window){
    char tempUangPelanggan[100];
    reset_string(tempUangPelanggan, 100);
    const gchar *gUangPelanggan = gtk_entry_get_text(GTK_ENTRY(entryDibayarPelanggan));
    strcpy(tempUangPelanggan, gUangPelanggan);

    // UANG FROM STRING TO INT
    int i = 0, iUangPelanggan = 0;
    while(tempUangPelanggan[i] != '\0'){
        int temp = tempUangPelanggan[i] - '0';
        iUangPelanggan = iUangPelanggan * 10 + temp;
        i++;
    }

    // GET TICKET PRICE
    int saveIndex;
    for (i = 0; i < 4; i++){
        if (strcmp(qFrontPay->qFilmName, FilmName[i]) == 0){
            saveIndex = i;
            break;
        }
    }
    if (iUangPelanggan < (qFrontPay->qTiketDibeli * TicketPrice[saveIndex])){
        display_error_dialog("Uang tidak cukup!", window);
    }
    else{
        gtk_widget_destroy(labelPecahanUangTersedia);
        gtk_widget_destroy(labelPecahanUangKembali);

        // COUNT MONEY BACK
        labelPecahanUangTersedia = gtk_label_new(
                                    "Rp 100.000 : \n\n"
                                    "Rp 50.000 : \n\n"
                                    "Rp 20.000 : \n\n"
                                    "Rp 10.000 : \n\n"
                                    "Rp 5.000 : \n\n"
                                    "Rp 2.000 : \n\n"
                                    "Rp 1.000 : "
                                    );
        char tempPecahanUangKembali[50];
        reset_string(tempPecahanUangKembali, 50);
        int uangKembalian = iUangPelanggan - (qFrontPay->qTiketDibeli * TicketPrice[saveIndex]);
        int i;
        for (i = 0; i < 7; i++){
            arrUangKembalian[i] = 0;
        }

        // RECURSIVE
        get_uang_kembalian_rec(uangKembalian, 0);

        // DISPLAY MONEY BACK
        for (i = 0; i < 7; i++){
            char strTemp[100];
            reset_string(strTemp, 100);
            sprintf(strTemp, "%d lembar\n\n", arrUangKembalian[i]);
            strcat(tempPecahanUangKembali, strTemp);
        }
        labelPecahanUangKembali = gtk_label_new(tempPecahanUangKembali);
        GtkWidget *labelTitleKembalian;
        labelTitleKembalian = gtk_label_new("Uang Kembalian");
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelTitleKembalian, 950, 200);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPecahanUangTersedia, 950, 250);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPecahanUangKembali, 1065, 250);
        gtk_widget_show_all(window);
    }
}

void display_warn_pembayaran(GtkWidget *widget, GtkWidget *window){
    bool isDeleted = false;
    const gchar *gEntry = gtk_entry_get_text(GTK_ENTRY(entryDibayarPelanggan));
    if (qFrontPay == NULL){
        display_error_dialog("Data pembayaran kosong", window);
    }
    else if (strcmp(gEntry, "") == 0){
        display_error_dialog("Uang tidak boleh kosong!", windowDashPembayaran);
    }
    else{
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_YES_NO,
                "Konfirmasi Pembayaran?");
        gtk_widget_set_name(dialog, "warnDialog");
        gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
        int res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == -8){ // YES
            isDeleted = true;
            // ADD TO TRANSACTION HISTORY
            FILE *f;
            f = fopen("data/transaction_history.txt", "a");
            fprintf(f, "%s|%d|%s|%d|%d|%s|%s|%s\n",
                        qFrontPay->qUsername, qFrontPay->qTiketDibeli, qFrontPay->qBuyTime,
                        qFrontPay->qStudioFilm, qFrontPay->qStudioTime,
                        qFrontPay->qFilmName, qFrontPay->qFilmTime, qFrontPay->qFilmChair);
            fclose(f);

            // ADD TO USER TRANSACTION TO FIND BEST USER
            f = fopen("data/user_transaction.txt", "a");
            int saveIndex, i;
            for (i = 0; i < 4; i++){
                if (strcmp(qFrontPay->qFilmName, FilmName[i]) == 0){
                    saveIndex = i;
                    break;
                }
            }
            fprintf(f, "%s|%s|%d\n", qFrontPay->qUsername, qFrontPay->qFilmName, (qFrontPay->qTiketDibeli * TicketPrice[saveIndex]));
            fclose(f);

            // DELETE IN QUEUE
            if (qFrontPay->qNext == NULL){
                free(qFrontPay);
                qFrontPay = NULL;
            }
            else{
                Queue *curr = qFrontPay;
                qFrontPay = qFrontPay->qNext;
                free(curr);
                curr = NULL;
            }

            // UPDATE DATA IN USER INVOICE
            f = fopen("data/user_invoice_data.txt", "w");
            Queue *curr = qFrontPay;
            while(curr != NULL){
                fprintf(f, "%s|%d|%s|%d|%d|%s|%s|%s\n",
                        curr->qUsername, curr->qTiketDibeli, curr->qBuyTime,
                        curr->qStudioFilm, curr->qStudioTime,
                        curr->qFilmName, curr->qFilmTime, curr->qFilmChair);
                curr = curr->qNext;
            }
            fclose(f);

            // REMOVE ALL QUEUE
            while(qFrontPay != NULL){
                Queue *curr = qFrontPay;
                if (qFrontPay->qNext == NULL){
                    free(qFrontPay);
                    qFrontPay = NULL;
                }
                else{
                    qFrontPay = qFrontPay->qNext;
                    free(curr);
                    curr = NULL;
                }
            }
        }
        gtk_widget_destroy(dialog);
    }
    if(isDeleted){
        // RELOAD TO READ DATA
        gtk_window_close(GTK_WINDOW(window));
        display_dashboard_pembayaran();
    }
}

void display_dashboard_pembayaran(){
    get_user_invoice_from_file();



    // ICON
    GdkPixbuf *iconDashPembayaran;

    // BACKGROUND
    GtkWidget *bgDashPembayaran;

    // BUTTON
    GtkWidget *buttonBackToDash;
    GtkWidget *buttonConfirmBayar;
    GtkWidget *buttonCountCashBack;

    // LABEL
    GtkWidget *labelPembayaranNamaPembeli;
    GtkWidget *labelPembayaranJumlahTiket;
    GtkWidget *labelPembayaranWaktuPembelian;
    GtkWidget *labelPembayaranNamaFilm;
    GtkWidget *labelPembayaranStudioFilm;
    GtkWidget *labelPembayaranWaktuFilm;
    GtkWidget *labelPembayaranKursiDipilih;
    GtkWidget *labelPembayaranTotalHarga;
    GtkWidget *labelPembayaranDibayar;
    GtkWidget *labelUserPayInfo;
    GtkWidget *labelEmptyData;
    GtkWidget *labelTitle;

    gtk_init(NULL, NULL);

    // WINDOW
    windowDashPembayaran = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowDashPembayaran), "Pembayaran Tiket");
    gtk_window_set_default_size(GTK_WINDOW(windowDashPembayaran), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowDashPembayaran), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutDashPembayaran = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowDashPembayaran), layoutDashPembayaran);

    // ICON
    iconDashPembayaran = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowDashPembayaran), iconDashPembayaran);

    // BACKGROUND
    bgDashPembayaran = gtk_image_new_from_file("src/image/dashboard_pembayaran.png");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), bgDashPembayaran, 0, 0);

    // label
    labelTitle = gtk_label_new("Pembayaran");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelTitle, 550, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");

    labelPecahanUangTersedia = gtk_label_new("");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPecahanUangTersedia, 800, 100);

    labelPecahanUangKembali = gtk_label_new("");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPecahanUangKembali, 900, 100);

    // label
    labelPembayaranNamaPembeli = gtk_label_new("Nama: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranNamaPembeli, 100, 200);
    labelPembayaranJumlahTiket = gtk_label_new("Jumlah: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranJumlahTiket, 100, 250);
    labelPembayaranWaktuPembelian = gtk_label_new("Waktu beli: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranWaktuPembelian, 100, 300);

    labelPembayaranNamaFilm = gtk_label_new("Film: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranNamaFilm, 100, 350);
    labelPembayaranStudioFilm = gtk_label_new("Studio: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranStudioFilm, 100, 400);
    labelPembayaranWaktuFilm = gtk_label_new("Waktu: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranWaktuFilm, 100, 450);

    labelPembayaranKursiDipilih = gtk_label_new("Kursi: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranKursiDipilih, 100, 500);

    labelPembayaranTotalHarga = gtk_label_new("Total Harga: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranTotalHarga, 500, 200);

    labelPembayaranDibayar = gtk_label_new("Pembayaran: ");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelPembayaranDibayar, 500, 250);

    // BUTTON
    buttonBackToDash = gtk_button_new_with_label("Dashboard");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), buttonBackToDash, 150, 550);

    buttonConfirmBayar = gtk_button_new_with_label("Konfirmasi Pembayaran");
    gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), buttonConfirmBayar, 950, 550);

    buttonCountCashBack = gtk_button_new_with_label("");

    // DISPLAY IF THERE IS NO DATA
    if (qFrontPay == NULL){
        int i, y = 200;
        for (i = 0; i < 7; i++){
            labelEmptyData = gtk_label_new("Belum ada data");
            gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelEmptyData, 200, y);
            y += 50;
        }
        y = 200;
        for (i = 0; i < 2; i++){
            labelEmptyData = gtk_label_new("Belum ada data");
            gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelEmptyData, 610, y);
            y += 50;
        }
    }

    // DISPLAY IF THERE IS DATA
    if (qFrontPay != NULL){
        gtk_widget_destroy(buttonCountCashBack);

        buttonCountCashBack = gtk_button_new_with_label("Hitung Kembalian");
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), buttonCountCashBack, 550, 550);

        entryDibayarPelanggan = gtk_entry_new();
        gtk_widget_set_size_request(entryDibayarPelanggan, 250, 40);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), entryDibayarPelanggan, 500, 300);

        // USER INFORMATION
        labelUserPayInfo = gtk_label_new(qFrontPay->qUsername);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 200);

        char tempUserInfo[50];
        reset_string(tempUserInfo, 50);
        sprintf(tempUserInfo, "%d", qFrontPay->qTiketDibeli);
        labelUserPayInfo = gtk_label_new(tempUserInfo);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 250);

        char tempBuyTime[10];
        reset_string(tempBuyTime, 10);
        int a = 0;
        while(a < 9){
            tempBuyTime[a] = qFrontPay->qBuyTime[a];
            a++;
        }
        labelUserPayInfo = gtk_label_new(tempBuyTime);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 300);

        labelUserPayInfo = gtk_label_new(qFrontPay->qFilmName);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 350);

        reset_string(tempUserInfo, 50);
        sprintf(tempUserInfo, "%d", qFrontPay->qStudioFilm);
        labelUserPayInfo = gtk_label_new(tempUserInfo);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 400);

        reset_string(tempUserInfo, 50);
        strcpy(tempUserInfo, StudioTime[(qFrontPay->qStudioFilm - 1)][(qFrontPay->qStudioTime - 1)]);
        labelUserPayInfo = gtk_label_new(tempUserInfo);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 450);

        labelUserPayInfo = gtk_label_new(qFrontPay->qFilmChair);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 200, 500);

        int i, saveIndex;
        for (i = 0; i < 4; i++){
            if (strcmp(qFrontPay->qFilmName, FilmName[i]) == 0){
                saveIndex = i;
                break;
            }
        }
        reset_string(tempUserInfo, 50);
        sprintf(tempUserInfo, "%d", qFrontPay->qTiketDibeli * TicketPrice[saveIndex]);
        labelUserPayInfo = gtk_label_new(tempUserInfo);
        gtk_layout_put(GTK_LAYOUT(layoutDashPembayaran), labelUserPayInfo, 610, 200);
    }

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_pembayaran.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowDashPembayaran);

    // SIGNAL
    g_signal_connect(windowDashPembayaran, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonBackToDash, "clicked", G_CALLBACK(handle_display_dashboard), windowDashPembayaran);
    g_signal_connect(buttonCountCashBack, "clicked", G_CALLBACK(count_cash_back), windowDashPembayaran);
    g_signal_connect(buttonConfirmBayar, "clicked", G_CALLBACK(display_warn_pembayaran), windowDashPembayaran);

    gtk_main();
}

void handle_display_dashboard_pembayaran(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_dashboard_pembayaran();
}
