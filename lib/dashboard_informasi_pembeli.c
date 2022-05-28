#include "dashboard_informasi_pembeli.h"

int foundHighest[3];
char chrUsernameFoundHighest[3][100] = {"data kosong", "data kosong", "data kosong"};
char chrFilmNameFoundHighest[3][50] = {"data kosong", "data kosong", "data kosong"};
int counterFoundHigh = -1;

void find_highest(BST *curr){
    if (curr != NULL){
        find_highest(curr->bRight);

        counterFoundHigh++;
        if (counterFoundHigh < 3){
            strcpy(chrUsernameFoundHighest[counterFoundHigh], curr->bUsername);
            strcpy(chrFilmNameFoundHighest[counterFoundHigh], curr->bFilmName);
            foundHighest[counterFoundHigh] = curr->bPayment;
        }

        find_highest(curr->bLeft);
    }
}

void display_informasi_pembeli(){
    bRoot = NULL;
     // WIDGET
    GtkWidget *windowInfoBST;
    GtkWidget *layoutInfoBST;
    GtkWidget *bgInfoBST;

    GtkWidget *buttonToInformasi;

    // LABEL
    GtkWidget *labelPembeliTerbaik;
    GtkWidget *labelTitle;

    // ICON
    GdkPixbuf *iconInfoBST;

    gtk_init(NULL, NULL);

    // WINDOW
    windowInfoBST = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowInfoBST), "Informasi Pembeli Terbaik");
    gtk_window_set_default_size(GTK_WINDOW(windowInfoBST), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowInfoBST), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutInfoBST = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowInfoBST), layoutInfoBST);

    // ICON
    iconInfoBST = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowInfoBST), iconInfoBST);

    // BACKGROUND
    bgInfoBST = gtk_image_new_from_file("src/image/PEMBELI TERSULTAN.png");
    gtk_layout_put(GTK_LAYOUT(layoutInfoBST), bgInfoBST, 0, 0);

     // label
    labelTitle = gtk_label_new("Pembeli Terbaik");
    gtk_layout_put(GTK_LAYOUT(layoutInfoBST), labelTitle, 550, 50);
    gtk_widget_set_name(labelTitle, "labelTitle");

    // BUTTON
    buttonToInformasi = gtk_button_new_with_label("Informasi");
    gtk_layout_put(GTK_LAYOUT(layoutInfoBST), buttonToInformasi, 600,650);

    // load data from file
    FILE *f;
    f = fopen("data/user_transaction.txt", "r");
    // make binary search tree from file
    char compUsr[100], compFilmName[50];
    int compPayment;

    reset_string(compUsr, 100);
    reset_string(compFilmName, 50);
    compPayment = 0;

    int counter = 0, idx = 1;
    char ch;
    while(!feof(f)){
        ch = fgetc(f);
        if (ch == '\n'){
            // make tree
            bNode = (BST*)malloc(sizeof(BST));
            strcpy(bNode->bUsername, compUsr);
            strcpy(bNode->bFilmName, compFilmName);
            bNode->bPayment = compPayment;
            bNode->bLeft = NULL;
            bNode->bRight = NULL;

            if (bRoot == NULL){
                bRoot = bNode;
            }
            else{
                BST *curr = bRoot, *parent = NULL;
                while(curr != NULL){
                    parent = curr;
                    if (bNode->bPayment > curr->bPayment){
                        curr = curr->bRight;
                    }
                    else{
                        curr = curr->bLeft;
                    }
                }
                if (bNode->bPayment > parent->bPayment){
                    parent->bRight = bNode;
                }
                else{
                    parent->bLeft = bNode;
                }
            }
            // reset
            reset_string(compUsr, 100);
            reset_string(compFilmName, 50);
            compPayment = 0;

            counter = 0;
            idx = 1;
        }
        else if (ch == '|'){
            counter = 0;
            idx++;
        }
        else{
            if (idx == 1){
                compUsr[counter] = ch;
                counter++;
            }
            else if (idx == 2){
                compFilmName[counter] = ch;
                counter++;
            }
            else if (idx == 3){
                int temp = ch - '0';
                compPayment = (compPayment * 10) + temp;
            }
        }
    }
    fclose(f);

    // search top highest 3
    if (bRoot != NULL){
            find_highest(bRoot);

    }
    // display it

    int i, x, y;
    for (i = 0; i < 3; i++){
        if (i == 0){
            x = 570;
            y = 300;
        }
        else if (i == 1){
            x = 300;
            y = 430;
        }
        else{
            x = 830;
            y = 520;
        }
        labelPembeliTerbaik = gtk_label_new(chrUsernameFoundHighest[i]);
        gtk_layout_put(GTK_LAYOUT(layoutInfoBST), labelPembeliTerbaik, x,y);
        y += 20;
        labelPembeliTerbaik = gtk_label_new(chrFilmNameFoundHighest[i]);
        gtk_layout_put(GTK_LAYOUT(layoutInfoBST), labelPembeliTerbaik, x,y);
        y += 20;
        char temp[100];
        reset_string(temp, 100);
        if (foundHighest[i] == 0){
            strcpy(temp, "data kosong");
        }
        else{
            sprintf(temp, "%d", foundHighest[i]);
        }

        labelPembeliTerbaik = gtk_label_new(temp);
        gtk_layout_put(GTK_LAYOUT(layoutInfoBST), labelPembeliTerbaik, x,y);
    }

    counterFoundHigh = -1;

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/dashboard_informasi_pembeli.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowInfoBST);

    // SIGNAL
    g_signal_connect(windowInfoBST, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonToInformasi, "clicked", G_CALLBACK(handle_display_dashboard_informasi), windowInfoBST);

    gtk_main();
}

void handle_display_informasi_pembeli(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_informasi_pembeli();
}
