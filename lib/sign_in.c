#include "sign_in.h"

// WINDOW
GtkWidget *windowSignIn;

// INPUT
GtkWidget *entryUsernameSignIn;
GtkWidget *entryPasswordSignIn;

void check_login(){
    char pass[100], username[100], passHashing[100];
    reset_string(pass, 100);
    reset_string(username, 100);
    reset_string(passHashing, 100);

    const gchar *gtextUsername = gtk_entry_get_text(GTK_ENTRY(entryUsernameSignIn));
    const gchar *gtextPass = gtk_entry_get_text(GTK_ENTRY(entryPasswordSignIn));
    sprintf(username, "%s", gtextUsername);
    sprintf(pass, "%s", gtextPass);

    strcpy(passHashing, pass);
    strcpy(passHashing, do_password_hash(passHashing));

    // CHECK
    admitLogin = false;
    UserInfo *temp = Head;
    while(temp != NULL){
        if (strcmp(username, temp->Username) == 0 && strcmp(passHashing, temp->Password) == 0){
            admitLogin = true;
            break;
        }
        temp = temp->Next;
    }
    if (admitLogin){
        display_info_dialog("Login Berhasil!", windowSignIn);
        display_dashboard(windowSignIn);
    }
    else{
        display_error_dialog("Username atau Password Salah!", windowSignIn);
    }
}

void display_sign_in(){
    // WIDGET
    GtkWidget *layoutSignIn;
    GtkWidget *bgSignIn;

    // ICON
    GdkPixbuf *iconSignIn;

    // BUTTON
    GtkWidget *buttonSignIn;
    GtkWidget *buttonSignUp;

    // LABEL
    GtkWidget *labelTitle;
    GtkWidget *labelContent;
    GtkWidget *labelUsername;
    GtkWidget *labelPassword;
    GtkWidget *labelNewAccount;

    gtk_init(NULL, NULL);

    // WINDOW
    windowSignIn = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowSignIn), "D'MOV1E");
    gtk_window_set_default_size(GTK_WINDOW(windowSignIn), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(windowSignIn), GTK_WIN_POS_CENTER);

    // LAYOUT
    layoutSignIn = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (windowSignIn), layoutSignIn);

    // ICON
    iconSignIn = gdk_pixbuf_new_from_file("src/image/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(windowSignIn), iconSignIn);

    // BACKGROUND
    bgSignIn = gtk_image_new_from_file("src/image/login.png");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), bgSignIn, 0, 0);

    // LABEL
    labelTitle = gtk_label_new("Welcome to D'MOV1E");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), labelTitle, 900, 100);
    gtk_widget_set_name(labelTitle, "labelTitle");

    labelContent = gtk_label_new("Sign In");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), labelContent, 900, 150);
    gtk_widget_set_name(labelContent, "labelContent");

    labelNewAccount = gtk_label_new("Belum punya akun?");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), labelNewAccount, 900, 500);

    labelUsername = gtk_label_new("Username");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), labelUsername, 900, 270);

    labelPassword = gtk_label_new("Password");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), labelPassword, 900, 355);

    // ENTRY
    entryUsernameSignIn = gtk_entry_new();
    gtk_widget_set_size_request(entryUsernameSignIn, 250, 40);
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), entryUsernameSignIn, 900, 300);

    entryPasswordSignIn = gtk_entry_new();
    gtk_widget_set_size_request(entryPasswordSignIn, 250, 40);
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), entryPasswordSignIn, 900, 390);
    gtk_entry_set_visibility(GTK_ENTRY(entryPasswordSignIn), false); // password hidden
    gtk_widget_set_name(entryPasswordSignIn, "entryPassSignIn");

    // BUTTON
    buttonSignIn = gtk_button_new_with_label("Login");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), buttonSignIn, 900, 450);

    buttonSignUp = gtk_button_new_with_label("Buat Akun");
    gtk_layout_put(GTK_LAYOUT(layoutSignIn), buttonSignUp, 1070, 495);

    // CSS
    GdkDisplay *display;
    display = gdk_display_get_default();
    GdkScreen *screen;
    screen = gdk_display_get_default_screen(display);
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/css/sign_in.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // DISPLAY
    gtk_widget_show_all(windowSignIn);

    // SIGNAL
    g_signal_connect(windowSignIn, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonSignUp, "clicked", G_CALLBACK(handle_display_sign_up), windowSignIn);
    g_signal_connect(buttonSignIn, "clicked", G_CALLBACK(check_login), NULL);

    gtk_main();
}
void handle_display_sign_in(GtkWidget *widget, GtkWidget *window){
    gtk_window_close(GTK_WINDOW(window));
    display_sign_in();
}
