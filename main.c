#include <gtk/gtk.h>

// Structure representing book information
typedef struct {
    char *firstName;
    char *lastName;
    char *title;
    float price;
    int quantity;
} Book;

// List storing information about books
GList *bookList = NULL;

// Array of widgets for data entry
GtkWidget *entryWidgets[5];

// Label for displaying search results
GtkWidget *resultsLabel;

// Function saving book data
static void saveData(GtkWidget *widget, gpointer data) {
    // Get data from entry fields
    const char *firstName = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[0])));
    const char *lastName = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[1])));
    const char *title = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[2])));
    float price = atof(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[3]))));
    int quantity = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[4]))));

    // Create a new Book structure and save the data
    Book *newBook = g_malloc(sizeof(Book));
    newBook->firstName = g_strdup(firstName);
    newBook->lastName = g_strdup(lastName);
    newBook->title = g_strdup(title);
    newBook->price = price;
    newBook->quantity = quantity;

    // Add the new book to the list
    bookList = g_list_append(bookList, newBook);
}

// Declaration of the function to remove a book from the list
static void removeBook(GtkWidget *widget, gpointer data);

// Function displaying the list of books
static void displayList(GtkWidget *widget, gpointer data) {
    GtkWidget *listWindow = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(listWindow), "Book List");

    GtkWidget *listContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(listContainer, 10);
    gtk_widget_set_margin_end(listContainer, 10);
    gtk_widget_set_margin_top(listContainer, 10);
    gtk_widget_set_margin_bottom(listContainer, 10);

    GList *iter;
    int index = 0;
    for (iter = bookList; iter != NULL; iter = g_list_next(iter)) {
        Book *book = (Book *)iter->data;
        // Create book information
        char *info = g_strdup_printf("First Name: %s\nLast Name: %s\nTitle: %s\nQuantity: %d\nPrice: %.2f\n\n",
                                     book->firstName, book->lastName, book->title, book->quantity, book->price);
        GtkWidget *bookLabel = gtk_label_new(info);
        g_free(info);

        // Create a button to remove the book
        GtkWidget *removeButton = gtk_button_new_with_label("Remove");
        g_signal_connect(removeButton, "clicked", G_CALLBACK(removeBook), GINT_TO_POINTER(index));
        gtk_box_append(GTK_BOX(listContainer), removeButton);
        gtk_box_append(GTK_BOX(listContainer), bookLabel);

        index++;
    }

    // Set the widget with the book list in the window
    gtk_window_set_child(GTK_WINDOW(listWindow), listContainer);
    gtk_window_present(GTK_WINDOW(listWindow));
}

// Function removing a book from the list
static void removeBook(GtkWidget *widget, gpointer data) {
    int indexToRemove = GPOINTER_TO_INT(data);

    if (indexToRemove >= 0 && indexToRemove < g_list_length(bookList)) {
        bookList = g_list_delete_link(bookList, g_list_nth(bookList, indexToRemove));
        displayList(NULL, NULL);
    }
}

// Function displaying a new window
static void newWindow(GtkWidget *widget, gpointer data) {
    GtkWidget *newWindow = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(newWindow), "New Window");

    GtkWidget *newLabel = gtk_label_new("This is a new window!");
    gtk_widget_set_margin_start(newLabel, 10);
    gtk_widget_set_margin_end(newLabel, 10);
    gtk_widget_set_margin_top(newLabel, 10);
    gtk_widget_set_margin_bottom(newLabel, 10);

    // Set the widget with the label in the window
    gtk_window_set_child(GTK_WINDOW(newWindow), newLabel);
    gtk_window_present(GTK_WINDOW(newWindow));
}

// Function searching for a book
static void searchBook(GtkWidget *widget, gpointer data) {
    const char *searchedFirstName = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[0])));
    const char *searchedLastName = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[1])));
    const char *searchedTitle = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entryWidgets[2])));

    GList *iter;
    for (iter = bookList; iter != NULL; iter = g_list_next(iter)) {
        Book *book = (Book *)iter->data;
        if (g_strcmp0(book->firstName, searchedFirstName) == 0 &&
            g_strcmp0(book->lastName, searchedLastName) == 0 &&
            g_strcmp0(book->title, searchedTitle) == 0) {
            GtkWidget *resultWindow = gtk_application_window_new(GTK_APPLICATION(data));
            gtk_window_set_title(GTK_WINDOW(resultWindow), "Search Result");

            // Create information about the found book
            char *info = g_strdup_printf("First Name: %s\nLast Name: %s\nTitle: %s\nQuantity: %d\nPrice: %.2f\n",
                                         book->firstName, book->lastName, book->title, book->quantity, book->price);
            resultsLabel = gtk_label_new(info);
            g_free(info);

            gtk_widget_set_margin_start(resultsLabel, 10);
            gtk_widget_set_margin_end(resultsLabel, 10);
            gtk_widget_set_margin_top(resultsLabel, 10);
            gtk_widget_set_margin_bottom(resultsLabel, 10);

            // Set the widget with the label in the window
            gtk_window_set_child(GTK_WINDOW(resultWindow), resultsLabel);
            gtk_window_present(GTK_WINDOW(resultWindow));
            return;
        }
    }

    // Information about no search result
    GtkWidget *noResultWindow = gtk_application_window_new(GTK_APPLICATION(data));
    gtk_window_set_title(GTK_WINDOW(noResultWindow), "No Search Result");

    GtkWidget *noResultLabel = gtk_label_new("No matching book found.");
    gtk_widget_set_margin_start(noResultLabel, 10);
    gtk_widget_set_margin_end(noResultLabel, 10);
    gtk_widget_set_margin_top(noResultLabel, 10);
    gtk_widget_set_margin_bottom(noResultLabel, 10);

    // Set the widget with the label in the window
    gtk_window_set_child(GTK_WINDOW(noResultWindow), noResultLabel);
    gtk_window_present(GTK_WINDOW(noResultWindow));
}

// Function activating the main application window
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *labels[5];
    GtkEntryBuffer *buffers[5];

    // Create the main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");

    // Create a grid to layout elements
    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // Set the grid as the content of the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Initialize entry fields and labels
    for (int i = 0; i < 5; i++) {
        buffers[i] = gtk_entry_buffer_new(NULL, 0);
        entryWidgets[i] = gtk_entry_new_with_buffer(buffers[i]);
        gtk_widget_set_hexpand(entryWidgets[i], TRUE);
        gtk_grid_attach(GTK_GRID(grid), entryWidgets[i], 1, i, 1, 1);

        labels[i] = gtk_label_new(NULL);
        const char *labelTexts[] = {"First Name", "Last Name", "Title", "Price", "Quantity"};
        gtk_label_set_text(GTK_LABEL(labels[i]), labelTexts[i]);
        gtk_grid_attach(GTK_GRID(grid), labels[i], 0, i, 1, 1);
    }

    // Button to save data
    button = gtk_button_new_with_label("Save");
    g_signal_connect(button, "clicked", G_CALLBACK(saveData), NULL);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 2, 1);

    // Button to display the list of books
    button = gtk_button_new_with_label("Display List");
    g_signal_connect(button, "clicked", G_CALLBACK(displayList), app);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 2, 1);

    // Button to search for books
    button = gtk_button_new_with_label("Search");
    g_signal_connect(button, "clicked", G_CALLBACK(searchBook), app);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 7, 2, 1);

    // Empty label for better formatting
    GtkWidget *emptyLabel = gtk_label_new("");
    gtk_widget_set_hexpand(emptyLabel, TRUE);
    gtk_grid_attach(GTK_GRID(grid), emptyLabel, 0, 8, 2, 1);

    // Button to close the application
    button = gtk_button_new_with_label("Exit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 9, 2, 1);

    // Display the main window
    gtk_window_present(GTK_WINDOW(window));
}

// Main program function
int main(int argc, char **argv) {
    // Initialize the GTK application
    GtkApplication *application;
    int status;

    application = gtk_application_new("org.gtk.application", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);

    return status;
}
