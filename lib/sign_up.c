#include "sign_up.h"

// WIDGET
GtkWidget *windowSignUp;

// INPUT
GtkWidget *entryUsernameSignUp;
GtkWidget *entryPasswordSignUp;
GtkWidget *entryRepeatPass;

bool search_exist_acc(char *user, char *hash){
    UserInfo *temp;
    temp = Head;
    while(temp != NULL){
        if(strcmp(temp->Username, user) == 0 && strcmp(temp->Password, hash) == 0){
            return true;
        }
        temp = temp->Next;
    }
    return false;
}

void create_new_acc(){
    char pass[100], passRepeat[100], passHashing[100], username[100];
    // RESET
    reset_string(pass, 100);
    reset_string(passRepeat, 100);
    reset_string(username, 100);
    reset_string(passHashing, 100);

    const gchar *gtextPass = gtk_entry_get_text(GTK_ENTRY(entryPasswordSignUp));
    const gchar *gtextPassRepeat = gtk_entry_get_text(GTK_ENTRY(entryRepeatPass));
    const gchar *gtextUsername = gtk_entry_get_text(GTK_ENTRY(entryUsernameSignUp));

    sprintf(pass, "%s", gtextPass);
    sprintf(passRepeat, "%s", gtextPassRepeat);
    sprintf(username, "%s", gtextUsername);

    strcpy(passHashing, pass);
    strcpy(passHashing, do_password_hash(passHashing));

    bool isFound;
    isFound = search_exist_acc(username, passHashing);

    if (strcmp(pass, "") == 0 || strcmp(username, "") == 0 ){
        display_error_dialog("Username dan Password Harus Diisi!", windowSignUp);
    }
    else if(strcmp(pass, passRepeat) != 0){
        display_error_dialog("Password Tidak Sama!", windowSignUp);
    }
    else if(isFound){
        display_error_dialog("Akun Sudah Ada!", windowSignUp);
    }
    else{
        // FILE OPERATION
        FILE *f;
        f = fopen("data/user_pass_data.txt", "a");
        fprintf(f,"%s|%s\n", username, passHashing);
        fclose(f);

        User = malloc(sizeof(UserInfo));
        strcpy(User->Username, username);
        strcpy(User->Password, passHashing);
        User->Next = NULL;
        if (Head == NULL){
            Head = User;
        }
        else{
            UserInfo *prev;
            prev = Head;
            while(prev->Next != NULL){
                prev = prev->Next;
            }
            prev->Next = User;
        }
        // RESET INPUT
        gtk_entry_set_text(GTK_ENTRY(entryUsernameSignUp), "");
        gtk_entry_set_text(GTK_ENTRY(entryPasswordSignUp), "");
        gtk_entry_set_text(GTK_ENTRY(entryRepeatPass), "");
        // DISPLAY SUCCESS
        display_info_dialog("Akun Berhasi Dibuat!", windowSignUp);
    }
    // RESET
    reset_string(pass, 100);
    reset_string(passRepeat, 100);
    reset_string(username, 100);
    reset_string(passHashing, 100);
}

void display_sign_up(){
    // WIDGET
    GtkWidget *layoutSignUp;
    GtkWidget *bgSignUp;

    // LABEL
    GtkWidget *labelUsername;
    GtkWidget *labelPassword;
    GtkWidget *labelRepeatPass;
    GtkWidget *labelAlreadyAcc;

    // BUTTON
    GtkWidget *buttonCreateNewAcc;
    GtkWidget *buttonBackToLogin;

    // ICON
    GdkPixbuf *iconSignUp;
    gtk_init(NULL, NULL);

    // WINDOW
    windowSignUp = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowSignUp), "SIGN UP");
    gtk_window_set_default_size(GTK_WINDOW(windowSignUp), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowSignUp), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutSignUp = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowSignUp), layoutSignUp);

    // ICON
    iconSignUp = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowSignUp), iconSignUp);

    // BACKGROUND
    bgSignUp = gtk_image_new_from_file("src/image/sign_up.png");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), bgSignUp, 0, 0);

    // LABEL
    labelUsername = gtk_label_new("Username");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), labelUsername, 200, 172);

    labelPassword = gtk_label_new("Password");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), labelPassword, 200, 258);

    labelRepeatPass = gtk_label_new("Ulangi Password");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), labelRepeatPass, 200, 344);

    // ENTRY
    entryUsernameSignUp = gtk_entry_new();
    gtk_widget_set_size_request(entryUsernameSignUp, 250, 40);
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), entryUsernameSignUp, 200, 210);

    entryPasswordSignUp = gtk_entry_new();
    gtk_widget_set_size_request(entryPasswordSignUp, 250, 40);
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), entryPasswordSignUp, 200, 290);
    gtk_entry_set_visibility(GTK_ENTRY(entryPasswordSignUp), false); // password hidden

    entryRepeatPass = gtk_entry_new();
    gtk_widget_set_size_request(entryRepeatPass, 250, 40);
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), entryRepeatPass, 200, 380);
    gtk_entry_set_visibility(GTK_ENTRY(entryRepeatPass), false); // password hidden

    labelAlreadyAcc = gtk_label_new("Sudah punya akun?");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), labelAlreadyAcc, 200, 500);

    // BUTTON
    buttonCreateNewAcc = gtk_button_new_with_label("Daftar");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), buttonCreateNewAcc, 200, 440);

    buttonBackToLogin = gtk_button_new_with_label("Login");
    gtk_layout_put(GTK_LAYOUT(layoutSignUp), buttonBackToLogin, 375, 495);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/sign_up.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowSignUp);

    // SIGNAL
    g_signal_connect(windowSignUp, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonBackToLogin, "clicked", G_CALLBACK(handle_display_sign_in), windowSignUp);
    g_signal_connect(buttonCreateNewAcc, "clicked", G_CALLBACK(create_new_acc), NULL);

    gtk_main();
}
void handle_display_sign_up(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_sign_up();
}

