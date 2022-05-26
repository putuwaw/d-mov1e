#ifndef DASHBOARD_H_INCLUDED
#define DASHBOARD_H_INCLUDED

#include "d_mov1e.h"

void display_dashboard(GtkWidget *window);
void handle_display_dashboard(GtkWidget *widget, GtkWidget *window);

// DASHBOARD HEADER
#include "dashboard_pembelian.h"
#include "dashboard_pemilihan.h"
#include "dashboard_pembayaran.h"
#include "dashboard_informasi.h"

#endif // DASHBOARD_H_INCLUDED
