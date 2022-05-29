#include "d_mov1e.h"

// LINKED LIST
UserInfo *User, *Head;

// QUEUE
Queue *qUser, *qFront, *qFrontPay;;

// STACK
Stack *sChair, *sTop, *sTopChair;

// BST
BST *bRoot, *bNode;

char FilmChair[16][3] ={
    "A1","A2","A3","A4",
    "B1","B2","B3","B4",
    "C1","C2", "C3", "C4",
    "D1", "D2", "D3", "D4"
};

char FilmName[4][50] = {
    "KKN Desa Penari",
    "The Throne",
    "Ready Player One",
    "Stand by Me Doraemon 2"
};

char FilmTime[4][3][20] = {
    {
        "08.00-10.00",
        "10.00-12.00",
        "13.30-15.30"
    },
    {
        "08.30-10.00",
        "11.30-13.00",
        "13.00-14.30"
    },
    {
        "10.30-11.30",
        "11.30-12.30",
        "12.30-13.30"
    },
    {
        "09.00-10.30",
        "10.00-11.30",
        "12.00-13.30"
    }
};

char StudioTime[3][4][20] = {
    {
        "08.00-10.00",
        "10.00-12.00",
        "12.00-13.30",
        "13.30-15.30"
    },
    {
        "08.30-10.00",
        "10.00-11.30",
        "11.30-13.00",
        "13.00-14.30"
    },
    {
        "09.00-10.30",
        "10.30-11.30",
        "11.30-12.30",
        "12.30-13.30"
    }
};

char StudioChair[3][4][16];

int TicketPrice[4] = {80000, 60000, 65000, 70000};

int AvailableChair[3][4];

int MoneyCurrency[7] = {100000, 50000, 20000, 10000, 5000, 2000, 1000};

bool admitLogin;

void reset_string(char *str, int len){
    int i;
    for (i = 0; i < len; i++){
        str[i] = '\0';
    }
}

void display_info_dialog(const gchar *info, GtkWidget *window){
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            info);
    gtk_widget_set_name(dialog, "infoDialog");
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void display_error_dialog(const gchar *error, GtkWidget *window){
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            error);
    gtk_widget_set_name(dialog, "errorDialog");
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void display_warn_dialog(const gchar *warn, GtkWidget *window){
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_YES_NO,
            warn);
    gtk_widget_set_name(dialog, "warnDialog");
    gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void get_user_pass_from_file(){
    char compUsr[100], compPwd[100];
    reset_string(compUsr, 100);
    reset_string(compPwd, 100);

    Head = NULL;
    FILE *f;
    f = fopen("data/user_pass_data.txt", "r");
    char ch;
    int idx = 1, counter = 0;
    while(!feof(f)){
        ch = fgetc(f);
        if (ch == '|'){
            counter = 0;
            idx = 2;
        }
        else if (ch == '\n'){
            User = malloc(sizeof(UserInfo));
            strcpy(User->Username, compUsr);
            strcpy(User->Password, compPwd);
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
            // RESET
            reset_string(compUsr, 100);
            reset_string(compPwd, 100);
            counter = 0;
            idx = 1;
        }
        else{
            if (idx == 1){
                compUsr[counter] = ch;
                counter++;
            }
            else if(idx == 2){
                compPwd[counter] = ch;
                counter++;
            }
        }
    }
    fclose(f);
}

char* do_password_hash(char *password){
    static char result[100];
    reset_string(result, 100);

    SHA256_CTX context;
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256_Init(&context);
    SHA256_Update(&context, (unsigned char *)password, sizeof(password));
    SHA256_Final(md, &context);

    int i = 0;
    while(md[i] != '\0'){
        int temp = md[i];
        int modTemp = temp % 16;
        if (modTemp < 10){
            result[i] = modTemp + '0';
        }
        else{
            modTemp -=  10;
            result[i] = modTemp + 'a';
        }
        i++;
    }
    return result;
}

char *time_to_str(struct tm *timeInfo){
    static const char weekDay[][10] = {
        "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
    static const char monthDay[][15] = {
        "Januari", "Februari", "Maret", "April", "Mei", "Juni",
        "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
    static char timeResult[100];
    reset_string(timeResult, 100);
    sprintf(timeResult, "%.2d:%.2d:%.2d - %s, %d %s %d",
            timeInfo->tm_hour,
            timeInfo->tm_min,
            timeInfo->tm_sec,
            weekDay[timeInfo->tm_wday],
            timeInfo->tm_mday,
            monthDay[timeInfo->tm_mon],
            1900 + timeInfo->tm_year);
    return timeResult;
}
