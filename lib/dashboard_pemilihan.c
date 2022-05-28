#include "dashboard_pembelian.h"

// LAYOUT
GtkWidget *layoutPemilihanKursi;

// LABEL
GtkWidget *servePerson;
GtkWidget *labelCountTicket;
GtkWidget *labelStudioName;
GtkWidget *labelFilmTime;
GtkWidget *labelTiketDipilih;
GtkWidget *labelEmptyData;

// BUTTON
GtkWidget *buttonAddKursi;
GtkWidget *buttonUndoKursi;
GtkWidget *buttonKursi;

// COMBO BOX
GtkWidget *comboSelectKursi;

int ChairSelected;

void get_user_ticket_from_file(){
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

    qFront = NULL;
    FILE *f;
    f = fopen("data/user_ticket_data.txt", "r");
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
            if (qFront == NULL){
                qFront = qUser;
            }
            else{
                Queue *prev;
                prev = qFront;
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

void write_user_ticket_to_file(){
    FILE *f;
    f = fopen("data/user_ticket_data.txt", "w");
    Queue *curr;
    curr = qFront;
    while(curr != NULL){
        fprintf(f, "%s|%d|%s|%d|%d|%s|%s|%s\n",curr->qUsername, curr->qTiketDibeli, curr->qBuyTime, curr->qStudioFilm, curr->qStudioTime, curr->qFilmName, curr->qFilmTime, curr->qFilmChair);
        curr = curr->qNext;
    }
    fclose(f);
}

void display_warn_pemilihan_kursi(GtkWidget *widget, GtkWidget *window){
    bool afterDel = false;
    if (qFront == NULL){
        display_error_dialog("Data pelanggan kosong", window);
    }
    else if(qFront->qTiketDibeli != ChairSelected){
        display_error_dialog("Jumlah tiket tidak sama", window);
    }
    else{ // NO ERROR
        afterDel = true;
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_YES_NO,
                "Konfirmasi Pemilihan Kursi?");
        gtk_widget_set_name(dialog, "warnConfirm");
        gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
        int res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == -8){ // YES
            char tempWriteToData[100];
            char tempFilmName[3];
            reset_string(tempWriteToData, 100);
            reset_string(tempFilmName, 3);

            // GET DATA FROM TIKET DIPILIH
            strcpy(tempWriteToData, gtk_label_get_label(GTK_LABEL(labelTiketDipilih)));
            int i = 0, counter = 0;
            while(tempWriteToData[i] != '\0'){
                if (tempWriteToData[i] == '\n'){
                    reset_string(tempFilmName, 3);
                    counter = 0;
                }
                else{
                    tempFilmName[counter] = tempWriteToData[i];
                    counter++;
                    if (counter == 2){
                        // SEARCH FILM CHAIR
                        int j, savePos;
                        for (j = 0; j < 16; j++){
                            if (strcmp(FilmChair[j], tempFilmName) == 0){
                                savePos = j;
                                j += 16; // BREAK LOOP
                            }
                        }
                        // CHANGE ARRAY
                        StudioChair[(qFront->qStudioFilm) - 1][(qFront->qStudioTime - 1)][savePos] = 1;
                    }
                }
                i++;
            }

            /* FILE OPERATION STUDIO CHAIR DATA */
            FILE *f;
            f = fopen("data/studio_chair_data.txt", "w");
            int a, b, c;
            for (a = 0; a < 3; a++){
                for (b = 0; b < 4; b++){
                    for (c = 0; c < 16; c++){
                        fprintf(f, "%d", StudioChair[a][b][c]);
                    }
                    fprintf(f, "\n");
                }
            }
            fclose(f);
            /* END OF FILE OPERATION STUDIO CHAIR DATA */

            // COPY DATA TIKET TO QUEUE
            char tempChairSelected[100];
            reset_string(tempChairSelected, 100);
            Stack *now = sTop;
            while(now != NULL){
                strcat(tempChairSelected, now->sChairName);
                strcat(tempChairSelected, " ");
                now = now->sNext;
            }
            strcpy(qFront->qFilmChair, tempChairSelected);

            // WRITE TO FILE
            f = fopen("data/user_invoice_data.txt", "a");
            fprintf(f, "%s|%d|%s|%d|%d|%s|%s|%s\n",
                    qFront->qUsername, qFront->qTiketDibeli, qFront->qBuyTime,
                    qFront->qStudioFilm, qFront->qStudioTime,
                    qFront->qFilmName, qFront->qFilmTime, qFront->qFilmChair);
            fclose(f);
            // END OF WRITE TO FILE

            // DEQUEUE
            Queue *curr;
            curr = qFront;
            while(curr->qNext != NULL){
                curr = curr->qNext;
            }
            if(curr == qFront){
                qFront = NULL;
                free(curr);
                curr = NULL;
            }
            else{
                curr = qFront;
                qFront = qFront->qNext;
                free(curr);
                curr = NULL;
            }
            // WRITE AGAIN
            write_user_ticket_to_file();
            gtk_widget_destroy(dialog);
            display_info_dialog("Pemilihan Kursi Berhasil!", window);
        }
        else{
            gtk_widget_destroy(dialog);
        }
    }
    if(qFront == NULL){
        gtk_label_set_text(GTK_LABEL(labelCountTicket), "Data kosong");
        gtk_label_set_text(GTK_LABEL(servePerson), "Data kosong");
        gtk_label_set_text(GTK_LABEL(labelStudioName), "Data kosong");
        gtk_label_set_text(GTK_LABEL(labelFilmTime), "Data kosong");
        gtk_label_set_text(GTK_LABEL(labelEmptyData), "Data kosong");
        if (afterDel){
            gtk_window_close(GTK_WINDOW(window));
            display_dashboard_pemilihan_kursi();
        }
    }
    else{
        gtk_widget_destroy(labelTiketDipilih);

        // REMOVE FROM STACK
        while(sTop != NULL){
            Stack *curr = sTop;
            sTop= sTop->sNext;
            free(curr);
            curr = NULL;
        }
        free(sTop);
        sTop = NULL;

        // CHANGE DISPLAY
        char temp[3];
        reset_string(temp, 3);
        sprintf(temp, "%d", qFront->qTiketDibeli);
        gtk_label_set_text(GTK_LABEL(labelCountTicket), temp);
        gtk_label_set_text(GTK_LABEL(servePerson), qFront->qUsername);
        char tempStudioFilm[5];
        reset_string(tempStudioFilm, 5);
        sprintf(tempStudioFilm, "%d", qFront->qStudioFilm);
        gtk_label_set_text(GTK_LABEL(labelStudioName),tempStudioFilm);
        gtk_label_set_text(GTK_LABEL(labelFilmTime), qFront->qFilmTime);

        // RESET COMBO BOX
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(comboSelectKursi));

        // ADD DATA TO COMBO BOX
        int counter;
        int i, j, delpos;
        // COMBO BOX
        for (i = 0; i < 16; i++){
            char temp[5];
            reset_string(temp, 5);
            strcpy(temp, FilmChair[i]);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSelectKursi), temp);
        }

        // READ STUDIO CHAIR FROM FILE
        FILE *f;
        f = fopen("data/studio_chair_data.txt", "r");
        char ch;
        int studio = 0, waktu = 0, kursi = 0;
        while(!feof(f)){
            ch = fgetc(f);
            if (ch == '\n' && waktu == 3){
                studio++;
                waktu = 0;
                kursi = 0;
            }
            else if (ch == '\n' && waktu < 3){
                waktu++;
                kursi = 0;
            }
            else{
                int temp = ch - '0';
                StudioChair[studio][waktu][kursi] = temp;
                kursi++;
            }
        }
        fclose(f);

        // DELETE COMBO BOX
        counter = 0;
        delpos = counter;
        gint x = 400, y = 260;
        for (i = 0; i < 4; i++){
            x = 400;
            for (j = 0; j < 4; j++){
                char copyCounter[3];
                strcpy(copyCounter, FilmChair[counter]);
                buttonKursi = gtk_button_new_with_label(copyCounter);
                if (StudioChair[(qFront->qStudioFilm - 1)][(qFront->qStudioTime - 1)][counter] != 0){
                    gtk_widget_set_name(buttonKursi, "late");
                    // DELETE FROM COMBO BOX
                    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(comboSelectKursi), delpos);
                }
                else{
                    // add to stack
                    sChair = (Stack*)malloc(sizeof(Stack));
                    strcpy(sChair->sChairName, FilmChair[counter]);
                    sChair->sNext = NULL;
                    if (sTopChair == NULL){
                        sTopChair = sChair;
                    }
                    else{
                        Stack *prev = sTopChair;
                        while(prev->sNext != NULL){
                            prev = prev->sNext;
                        }
                        prev->sNext = sChair;
                    }
                    delpos++;
                }
                counter++;

                gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonKursi, x, y);
                gtk_widget_set_size_request(buttonKursi, 50,50);
                x += 100;
            }
            y += 100;
        }
        gtk_widget_show_all(window);
        gtk_window_close(GTK_WINDOW(window));
        display_dashboard_pemilihan_kursi();
    }
}

void add_kursi(GtkWidget *widget, GtkWidget *window){
    if (sTopChair == NULL){
        display_error_dialog("Kursi telah habis dipesan", window);
    }
    else{
        // GET INPUT
        char text[10];
        reset_string(text, 10);
        strcpy(text, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboSelectKursi)));

        // SEARCH POSITION TO REMOVE IN COMBOBOX
        Stack *curr;
        curr = sTopChair;
        int count = -1;
        while(curr != NULL){
            count++;
            if (strcmp(curr->sChairName, text) == 0){
                if (sTopChair->sNext == NULL){
                    sTopChair = NULL;
                    free(curr);
                    curr = NULL;
                }
                else{
                    if (curr == sTopChair){
                        sTopChair = sTopChair->sNext;
                        free(curr);
                        curr = NULL;
                    }
                    else{
                        Stack *prev;
                        prev = sTopChair;
                        while(prev->sNext != curr){
                            prev = prev->sNext;
                        }
                        prev->sNext = curr->sNext;
                        curr->sNext = NULL;
                        free(curr);
                        curr = NULL;
                    }
                }
                break;
            }
            curr = curr->sNext;
        }
        gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(comboSelectKursi), count);

        // ADD TO STACK
        ChairSelected++;
        sChair = (Stack*)malloc(sizeof(Stack));
        strcpy(sChair->sChairName, text);
        sChair->sNext = NULL;
        if (sTop == NULL){
            sTop = sChair;
        }
        else{
            Stack *prev;
            prev = sTop;
            while(prev->sNext != NULL){
                prev = prev->sNext;
            }
            prev->sNext = sChair;
        }

        // DISPLAY STACK
        Stack *now;
        now = sTop;
        char textDisplay[100];
        reset_string(textDisplay, 100);
        while(now != NULL){
            strcat(textDisplay, now->sChairName);
            strcat(textDisplay, "\n");
            now = now->sNext;
        }
        gtk_widget_destroy(GTK_WIDGET(labelTiketDipilih));
        labelTiketDipilih = gtk_label_new(textDisplay);
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelTiketDipilih, 800, 200);
        gtk_widget_show_all(window);
    }
}

void undo_kursi(GtkWidget *widget, GtkWidget *window){
    char text[10];
    reset_string(text, 10);
    ChairSelected--;
    if (sTop == NULL){
        display_error_dialog("Error belum ada dipilih!", window);
    }
    else{
        // POP FROM STACK
        Stack *curr = sTop, *prev = sTop;
        if (sTop->sNext == NULL){
            strcpy(text, sTop->sChairName);
            free(sTop);
            sTop = NULL;
        }
        else{
            while(curr->sNext != NULL){
            curr = curr->sNext;
            }
            while(prev->sNext != curr){
                prev = prev->sNext;
            }
            prev->sNext = NULL;
            strcpy(text, curr->sChairName);
            free(curr);
            curr = NULL;
        }

        // DISPLAY STACK
        char textDisplay[100];
        reset_string(textDisplay, 100);
        Stack *now;
        now = sTop;
        while(now != NULL){
            strcat(textDisplay, now->sChairName);
            strcat(textDisplay, "\n");
            now = now->sNext;
        }
        gtk_widget_destroy(GTK_WIDGET(labelTiketDipilih));
        labelTiketDipilih = gtk_label_new(textDisplay);
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelTiketDipilih, 800, 200);
        gtk_widget_show_all(window);

        // ADD TO STACK CHAIR
        sChair = (Stack*)malloc(sizeof(Stack));
        strcpy(sChair->sChairName, text);
        sChair->sNext = NULL;

        if(sTopChair == NULL){
            sTopChair = sChair;
        }
        else{
            Stack *prev;
            prev = sTopChair;
            while(prev->sNext != NULL){
                prev = prev->sNext;
            }
            prev->sNext = sChair;
        }

        // SORT STACK CHAIR
        bool isSorted = false;
        while(!isSorted){
            isSorted = true;
            Stack *curr = sTopChair;
            while (curr->sNext != NULL){
                if (strcmp(curr->sChairName, curr->sNext->sChairName) > 0){
                    isSorted = false;
                    Stack *temp;
                    temp = (Stack*)malloc(sizeof(Stack));
                    strcpy(temp->sChairName, curr->sChairName);
                    strcpy(curr->sChairName, curr->sNext->sChairName);
                    strcpy(curr->sNext->sChairName, temp->sChairName);
                    free(temp);
                    temp = NULL;
                }
                curr = curr->sNext;
            }
        }

        // SEARCH POSITION
        int counter = -1;
        curr = sTopChair;
        while(curr != NULL){
            counter++;
            if(strcmp(curr->sChairName, text) == 0){
                break;
            }
            curr = curr->sNext;
        }

        // ADD TO COMBOBOX AT RIGHT POSITION
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(comboSelectKursi), counter, text);
    }
}

void display_dashboard_pemilihan_kursi(){
    get_user_ticket_from_file();

    sTopChair = NULL;
    sTop = NULL;
    int counter = 0;
    ChairSelected = 0;

    GtkWidget *windowPemilihanKursi;
    GtkWidget *bgPemilihanKursi;

    GtkWidget *labelTitle;
    GtkWidget *labelServePeople;
    GtkWidget *labelTicketAvail;
    GtkWidget *labelNoStudio;
    GtkWidget *labelTiketSementara;

    // BUTTON
    GtkWidget *buttonBackToDash;
    GtkWidget *buttonConfirmPilih;
    GtkWidget *buttonScreenArea;

    // ICON
    GdkPixbuf *iconPemilihanKursi;

    gtk_init(NULL, NULL);

    // WINDOW
    windowPemilihanKursi = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowPemilihanKursi), "Pemilihan Kursi");
    gtk_window_set_default_size(GTK_WINDOW(windowPemilihanKursi), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowPemilihanKursi), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutPemilihanKursi = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowPemilihanKursi), layoutPemilihanKursi);

    // ICON
    iconPemilihanKursi= gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowPemilihanKursi), iconPemilihanKursi);

    // BACKGROUND
    bgPemilihanKursi = gtk_image_new_from_file("src/image/dashboard_pembelian.png");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), bgPemilihanKursi, 0, 0);

    labelTiketDipilih = gtk_label_new("");

    labelTiketSementara = gtk_label_new("Tiket yang Sudah Dipilih:");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelTiketSementara, 800, 150);

    labelTitle = gtk_label_new("Menu Pemilihan Kursi");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelTitle, 500, 75);
    gtk_widget_set_name(labelTitle, "labelTitle");

    labelServePeople = gtk_label_new("Pelanggan:");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelServePeople, 150, 200);

    if(qFront == NULL){
        labelEmptyData = gtk_label_new("Data kosong");
        servePerson = gtk_label_new("Data kosong");
        labelCountTicket = gtk_label_new("Data kosong");
        labelStudioName = gtk_label_new("Data kosong");
        labelFilmTime = gtk_label_new("Data kosong");
    }
    else{
        labelEmptyData = gtk_label_new("");
        char temp[3];
        reset_string(temp, 3);

        sprintf(temp, "%d", qFront->qTiketDibeli);
        labelCountTicket = gtk_label_new(temp);
        servePerson = gtk_label_new(qFront->qUsername);

        char tempStudioFilm[5];
        reset_string(tempStudioFilm, 5);
        sprintf(tempStudioFilm, "%d", qFront->qStudioFilm);
        labelStudioName = gtk_label_new(tempStudioFilm);
        labelFilmTime = gtk_label_new(qFront->qFilmTime);

        // BUTTON
        buttonAddKursi = gtk_button_new_with_label("Tambahkan");
        gtk_widget_set_name(buttonAddKursi, "buttonAction");
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonAddKursi, 150, 500);

        buttonUndoKursi = gtk_button_new_with_label("Batalkan");
        gtk_widget_set_name(buttonUndoKursi, "buttonAction");
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonUndoKursi, 300, 500);

        buttonScreenArea = gtk_button_new_with_label("Screen Area");
        gtk_widget_set_name(buttonScreenArea, "screenArea");
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonScreenArea, 500, 500);
        gtk_widget_set_size_request(buttonScreenArea, 300, 10);

        int i, j, delpos;
        // COMBO BOX
        comboSelectKursi = gtk_combo_box_text_new();
        for (i = 0; i < 16; i++){
            char temp[5];
            reset_string(temp, 5);
            strcpy(temp, FilmChair[i]);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSelectKursi), temp);
        }
        gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), comboSelectKursi, 150, 400);
        gtk_widget_set_name(comboSelectKursi, "comboKursi");
        gtk_widget_set_size_request(comboSelectKursi, 80, 30);

        // READ DATA FROM STUDIO CHAIR DATA
        FILE *f;
        f = fopen("data/studio_chair_data.txt", "r");
        char ch;
        int studio = 0, waktu = 0, kursi = 0;
        while(!feof(f)){
            ch = fgetc(f);
            if (ch == '\n' && waktu == 3){
                studio++;
                waktu = 0;
                kursi = 0;
            }
            else if (ch == '\n' && waktu < 3){
                waktu++;
                kursi = 0;
            }
            else{
                int temp = ch - '0';
                StudioChair[studio][waktu][kursi] = temp;
                kursi++;
            }
        }
        fclose(f);
        // END OF READ DATA FROM STUDIO CHAIR DATA

        // COMBOBOX CREATION
        counter = 0;
        delpos = counter;
        gint x = 500, y = 210;
        for (i = 0; i < 4; i++){
            x = 500;
            for (j = 0; j < 4; j++){
                char copyCounter[3];
                strcpy(copyCounter, FilmChair[counter]);
                buttonKursi = gtk_button_new_with_label(copyCounter);
                if (StudioChair[(qFront->qStudioFilm - 1)][(qFront->qStudioTime - 1)][counter] != 0){
                    gtk_widget_set_name(buttonKursi, "late");
                    // DON'T ADD TO COMBOBOX
                    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(comboSelectKursi), delpos);
                }
                else{
                    gtk_widget_set_name(buttonKursi, "avail");
                    // ADD TO STACK CHAIR
                    sChair = (Stack*)malloc(sizeof(Stack));
                    strcpy(sChair->sChairName, FilmChair[counter]);
                    sChair->sNext = NULL;
                    if (sTopChair == NULL){
                        sTopChair = sChair;
                    }
                    else{
                        Stack *prev = sTopChair;
                        while(prev->sNext != NULL){
                            prev = prev->sNext;
                        }
                        prev->sNext = sChair;
                    }
                    delpos++;
                }
                counter++;
                gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonKursi, x, y);
                gtk_widget_set_size_request(buttonKursi, 50,50);
                x += 70;
            }
            y += 70;
        }
        // ADD AND REMOVE CHAIR SIGNAL
        g_signal_connect(buttonAddKursi, "clicked", G_CALLBACK(add_kursi), windowPemilihanKursi);
        g_signal_connect(buttonUndoKursi, "clicked", G_CALLBACK(undo_kursi), windowPemilihanKursi);
    }
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), servePerson,300, 200);
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelCountTicket, 300, 150);
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelStudioName, 230, 250);
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelFilmTime, 350, 250);
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelEmptyData, 800, 200);

    // LABEL
    labelTicketAvail = gtk_label_new("Jumlah Tiket: ");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelTicketAvail, 150, 150);

    labelNoStudio = gtk_label_new("Studio: ");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), labelNoStudio, 150 ,250);

    // BUTTON
    buttonBackToDash = gtk_button_new_with_label("Dashboard");
    gtk_widget_set_name(buttonBackToDash, "buttonAction");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi  ), buttonBackToDash, 150, 600);

    buttonConfirmPilih = gtk_button_new_with_label("Pilih Kursi");
    gtk_widget_set_name(buttonConfirmPilih, "buttonAction");
    gtk_layout_put(GTK_LAYOUT(layoutPemilihanKursi), buttonConfirmPilih, 1000, 600);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_pemilihan.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowPemilihanKursi);

    // SIGNAL
    g_signal_connect(windowPemilihanKursi, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonBackToDash, "clicked", G_CALLBACK(handle_display_dashboard), windowPemilihanKursi);
    g_signal_connect(buttonConfirmPilih, "clicked", G_CALLBACK(display_warn_pemilihan_kursi), windowPemilihanKursi);

    gtk_main();
}
void handle_display_dashboard_pemilihan_kursi(GtkWidget *widget,  GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_dashboard_pemilihan_kursi();
}
