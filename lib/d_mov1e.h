#ifndef D_MOV1E_H_INCLUDED
#define D_MOV1E_H_INCLUDED

// C STD LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// OpenSSL LIBRARY
#include <openssl/sha.h>

// GTK LIBRARY
#include <gtk/gtk.h>

// GLOBAL VARIABLE
typedef struct UserInfo{
    char Username[100];
    char Password[100];
    struct UserInfo *Next;
}UserInfo;
extern UserInfo *User, *Head;

typedef struct Queue{
    char qUsername[100];
    int qTiketDibeli;
    char qBuyTime[50];
    int qStudioFilm;
    int qStudioTime;
    char qFilmName[50];
    char qFilmTime[20];
    char qFilmChair[50];

    struct Queue *qNext;
}Queue;
extern Queue *qUser, *qFront, *qFrontPay;

typedef struct Stack{
    char sChairName[10];
    struct Stack *sNext;
}Stack;
extern Stack *sChair, *sTop, *sTopChair;

typedef struct BST{
    char bUsername[100];
    char bFilmName[50];
    int bPayment;
    struct BST *bLeft;
    struct BST *bRight;
}BST;
extern BST *bRoot, *bNode;

extern char FilmChair[16][3];
extern char FilmName[4][50];
extern char StudioTime[3][4][20];
extern char FilmTime[4][3][20];
extern char StudioChair[3][4][16];
extern int TicketPrice[4];
extern int AvailableChair[3][4];
extern int MoneyCurrency[7];

extern bool admitLogin;
extern int ChairSelected;

// LOCAL LIBRARY
#include "sign_in.h"
#include "sign_up.h"
#include "dashboard.h"

// FUNCTION
void reset_string(char *str, int len);
void display_info_dialog(const gchar *info, GtkWidget *window);
void display_error_dialog(const gchar *error, GtkWidget *window);
void display_warn_dialog(const gchar *warn, GtkWidget *window);
void get_user_pass_from_file();
char *do_password_hash(char *password);
char *timeToStr(struct tm *timeInfo);

#endif // D_MOV1E_H_INCLUDED
