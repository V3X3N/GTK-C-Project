#include <gtk/gtk.h>

// Struktura reprezentująca informacje o książce
typedef struct {
    char *imie;
    char *nazwisko;
    char *tytul;
    float cena;
    int liczba;
} Ksiazka;

// Lista przechowująca informacje o książkach
GList *lista_ksiazek = NULL;

// Tablica widgetów do wprowadzania danych
GtkWidget *wprowadzenie[5];

// Etykieta do wyświetlania wyników wyszukiwania
GtkWidget *wyniki_etykieta;

// Funkcja zapisująca dane o książce
static void zapisz_dane(GtkWidget *widget, gpointer data) {
    // Pobranie danych z pól wprowadzania
    const char *imie = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[0])));
    const char *nazwisko = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[1])));
    const char *tytul = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[2])));
    float cena = atof(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[3]))));
    int liczba = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[4]))));

    // Tworzenie nowej struktury Ksiazka i zapis danych
    Ksiazka *nowa_ksiazka = g_malloc(sizeof(Ksiazka));
    nowa_ksiazka->imie = g_strdup(imie);
    nowa_ksiazka->nazwisko = g_strdup(nazwisko);
    nowa_ksiazka->tytul = g_strdup(tytul);
    nowa_ksiazka->cena = cena;
    nowa_ksiazka->liczba = liczba;

    // Dodanie nowej książki do listy
    lista_ksiazek = g_list_append(lista_ksiazek, nowa_ksiazka);
}

// Deklaracja funkcji usuwajacej ksiazke z listy
static void usun_ksiazke(GtkWidget *widget, gpointer data);

// Funkcja wyświetlająca listę książek
static void wyswietl_liste(GtkWidget *widget, gpointer data) {
    GtkWidget *okno_lista = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(okno_lista), "Lista Książek");

    GtkWidget *kontener_lista = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(kontener_lista, 10);
    gtk_widget_set_margin_end(kontener_lista, 10);
    gtk_widget_set_margin_top(kontener_lista, 10);
    gtk_widget_set_margin_bottom(kontener_lista, 10);

    GList *iter;
    int indeks = 0;
    for (iter = lista_ksiazek; iter != NULL; iter = g_list_next(iter)) {
        Ksiazka *ksiazka = (Ksiazka *)iter->data;
        // Tworzenie informacji o książce
        char *info = g_strdup_printf("Imię: %s\nNazwisko: %s\nTytuł: %s\nLiczba: %d\nCena: %.2f\n\n",
                                     ksiazka->imie, ksiazka->nazwisko, ksiazka->tytul, ksiazka->liczba, ksiazka->cena);
        GtkWidget *etykieta_ksiazki = gtk_label_new(info);
        g_free(info);

        // Tworzenie przycisku do usuwania książki
        GtkWidget *przycisk_usun = gtk_button_new_with_label("Usuń");
        g_signal_connect(przycisk_usun, "clicked", G_CALLBACK(usun_ksiazke), GINT_TO_POINTER(indeks));
        gtk_box_append(GTK_BOX(kontener_lista), przycisk_usun);
        gtk_box_append(GTK_BOX(kontener_lista), etykieta_ksiazki);

        indeks++;
    }

    // Ustawienie widżetu z listą książek w oknie
    gtk_window_set_child(GTK_WINDOW(okno_lista), kontener_lista);
    gtk_window_present(GTK_WINDOW(okno_lista));
}

// Funkcja usuwająca książkę z listy
static void usun_ksiazke(GtkWidget *widget, gpointer data) {
    int indeks_do_usuniecia = GPOINTER_TO_INT(data);

    if (indeks_do_usuniecia >= 0 && indeks_do_usuniecia < g_list_length(lista_ksiazek)) {
        lista_ksiazek = g_list_delete_link(lista_ksiazek, g_list_nth(lista_ksiazek, indeks_do_usuniecia));
        wyswietl_liste(NULL, NULL);
    }
}

// Funkcja wyświetlająca nowe okno
static void nowe_okno(GtkWidget *widget, gpointer data) {
    GtkWidget *okno_nowe = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(okno_nowe), "Nowe Okno");

    GtkWidget *etykieta_nowa = gtk_label_new("To jest nowe okno!");
    gtk_widget_set_margin_start(etykieta_nowa, 10);
    gtk_widget_set_margin_end(etykieta_nowa, 10);
    gtk_widget_set_margin_top(etykieta_nowa, 10);
    gtk_widget_set_margin_bottom(etykieta_nowa, 10);

    // Ustawienie widżetu z etykietą w oknie
    gtk_window_set_child(GTK_WINDOW(okno_nowe), etykieta_nowa);
    gtk_window_present(GTK_WINDOW(okno_nowe));
}

// Funkcja wyszukująca książkę
static void szukaj_ksiazki(GtkWidget *widget, gpointer data) {
    const char *szukane_imie = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[0])));
    const char *szukane_nazwisko = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[1])));
    const char *szukany_tytul = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(wprowadzenie[2])));

    GList *iter;
    for (iter = lista_ksiazek; iter != NULL; iter = g_list_next(iter)) {
        Ksiazka *ksiazka = (Ksiazka *)iter->data;
        if (g_strcmp0(ksiazka->imie, szukane_imie) == 0 &&
            g_strcmp0(ksiazka->nazwisko, szukane_nazwisko) == 0 &&
            g_strcmp0(ksiazka->tytul, szukany_tytul) == 0) {
            GtkWidget *okno_wyniku = gtk_application_window_new(GTK_APPLICATION(data));
            gtk_window_set_title(GTK_WINDOW(okno_wyniku), "Wynik wyszukiwania");

            // Tworzenie informacji o znalezionej książce
            char *info = g_strdup_printf("Imię: %s\nNazwisko: %s\nTytuł: %s\nLiczba: %d\nCena: %.2f\n",
                                         ksiazka->imie, ksiazka->nazwisko, ksiazka->tytul, ksiazka->liczba, ksiazka->cena);
            wyniki_etykieta = gtk_label_new(info);
            g_free(info);

            gtk_widget_set_margin_start(wyniki_etykieta, 10);
            gtk_widget_set_margin_end(wyniki_etykieta, 10);
            gtk_widget_set_margin_top(wyniki_etykieta, 10);
            gtk_widget_set_margin_bottom(wyniki_etykieta, 10);

            // Ustawienie widżetu z etykietą w oknie
            gtk_window_set_child(GTK_WINDOW(okno_wyniku), wyniki_etykieta);
            gtk_window_present(GTK_WINDOW(okno_wyniku));
            return;
        }
    }

    // Informacja o braku wyniku wyszukiwania
    GtkWidget *okno_braku_wyniku = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(okno_braku_wyniku), "Brak wyniku wyszukiwania");

    GtkWidget *brak_wyniku_etykieta = gtk_label_new("Nie znaleziono pasującej książki.");
    gtk_widget_set_margin_start(brak_wyniku_etykieta, 10);
    gtk_widget_set_margin_end(brak_wyniku_etykieta, 10);
    gtk_widget_set_margin_top(brak_wyniku_etykieta, 10);
    gtk_widget_set_margin_bottom(brak_wyniku_etykieta, 10);

    // Ustawienie widżetu z etykietą w oknie
    gtk_window_set_child(GTK_WINDOW(okno_braku_wyniku), brak_wyniku_etykieta);
    gtk_window_present(GTK_WINDOW(okno_braku_wyniku));
}

// Funkcja aktywująca główne okno aplikacji
static void aktywuj(GtkApplication *app, gpointer user_data) {
    GtkWidget *okno;
    GtkWidget *siatka;
    GtkWidget *przycisk;
    GtkWidget *etykieta[5];
    GtkEntryBuffer *bufor[5];

    // Utworzenie głównego okna
    okno = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(okno), "Okno");

    // Utworzenie siatki do rozmieszczenia elementów
    siatka = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(siatka), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(siatka), TRUE);

    // Ustawienie siatki jako zawartość okna
    gtk_window_set_child(GTK_WINDOW(okno), siatka);

    // Inicjalizacja pól wprowadzania i etykiet
    for (int i = 0; i < 5; i++) {
        bufor[i] = gtk_entry_buffer_new(NULL, 0);
        wprowadzenie[i] = gtk_entry_new_with_buffer(bufor[i]);
        gtk_widget_set_hexpand(wprowadzenie[i], TRUE);
        gtk_grid_attach(GTK_GRID(siatka), wprowadzenie[i], 1, i, 1, 1);

        etykieta[i] = gtk_label_new(NULL);
        const char *teksty_etykiet[] = {"Imię", "Nazwisko", "Tytuł", "Cena", "Liczba"};
        gtk_label_set_text(GTK_LABEL(etykieta[i]), teksty_etykiet[i]);
        gtk_grid_attach(GTK_GRID(siatka), etykieta[i], 0, i, 1, 1);
    }

    // Przycisk do zapisu danych
    przycisk = gtk_button_new_with_label("Zapisz");
    g_signal_connect(przycisk, "clicked", G_CALLBACK(zapisz_dane), NULL);
    gtk_widget_set_hexpand(przycisk, TRUE);
    gtk_grid_attach(GTK_GRID(siatka), przycisk, 0, 5, 2, 1);

    // Przycisk do wyświetlenia listy książek
    przycisk = gtk_button_new_with_label("Wyświetl Listę");
    g_signal_connect(przycisk, "clicked", G_CALLBACK(wyswietl_liste), app);
    gtk_widget_set_hexpand(przycisk, TRUE);
    gtk_grid_attach(GTK_GRID(siatka), przycisk, 0, 6, 2, 1);

    // Przycisk do wyszukiwania książek
    przycisk = gtk_button_new_with_label("Szukaj");
    g_signal_connect(przycisk, "clicked", G_CALLBACK(szukaj_ksiazki), app);
    gtk_widget_set_hexpand(przycisk, TRUE);
    gtk_grid_attach(GTK_GRID(siatka), przycisk, 0, 7, 2, 1);

    // Pusta etykieta dla lepszego formatowania
    GtkWidget *pusta_etykieta = gtk_label_new("");
    gtk_widget_set_hexpand(pusta_etykieta, TRUE);
    gtk_grid_attach(GTK_GRID(siatka), pusta_etykieta, 0, 8, 2, 1);

    // Przycisk do zamknięcia aplikacji
    przycisk = gtk_button_new_with_label("Wyjdź");
    g_signal_connect_swapped(przycisk, "clicked", G_CALLBACK(gtk_window_destroy), okno);
    gtk_widget_set_hexpand(przycisk, TRUE);
    gtk_grid_attach(GTK_GRID(siatka), przycisk, 0, 9, 2, 1);

    // Wyświetlenie głównego okna
    gtk_window_present(GTK_WINDOW(okno));
}

// Główna funkcja programu
int main(int argc, char **argv) {
    // Inicjalizacja aplikacji GTK
    GtkApplication *aplikacja;
    int status;

    aplikacja = gtk_application_new("org.gtk.aplikacja", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(aplikacja, "activate", G_CALLBACK(aktywuj), NULL);
    status = g_application_run(G_APPLICATION(aplikacja), argc, argv);
    g_object_unref(aplikacja);

    return status;
}
