#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int year;
} Book;

void addBook(FILE *file) {
    Book book;
    printf("Enter Book ID: ");
    scanf("%d", &book.id);
    printf("Enter Book Title: ");
    getchar(); // Consume newline
    fgets(book.title, MAX_TITLE_LENGTH, stdin);
    book.title[strcspn(book.title, "\n")] = '\0'; // Remove newline
    printf("Enter Author Name: ");
    fgets(book.author, MAX_AUTHOR_LENGTH, stdin);
    book.author[strcspn(book.author, "\n")] = '\0'; // Remove newline
    printf("Enter Publication Year: ");
    scanf("%d", &book.year);

    fseek(file, 0, SEEK_END);
    fwrite(&book, sizeof(Book), 1, file);
    printf("Book added successfully!\n");
}

void displayBooks(FILE *file) {
    Book book;
    rewind(file);
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n", book.id, book.title, book.author, book.year);
    }
}

void searchBook(FILE *file, int id) {
    Book book;
    rewind(file);
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n", book.id, book.title, book.author, book.year);
            return;
        }
    }
    printf("Book not found.\n");
}

void updateBook(FILE *file, int id) {
    Book book;
    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error creating temporary file.\n");
        return;
    }
    
    rewind(file);
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            printf("Enter new Book Title: ");
            getchar(); // Consume newline
            fgets(book.title, MAX_TITLE_LENGTH, stdin);
            book.title[strcspn(book.title, "\n")] = '\0'; // Remove newline
            printf("Enter new Author Name: ");
            fgets(book.author, MAX_AUTHOR_LENGTH, stdin);
            book.author[strcspn(book.author, "\n")] = '\0'; // Remove newline
            printf("Enter new Publication Year: ");
            scanf("%d", &book.year);
            found = 1;
        }
        fwrite(&book, sizeof(Book), 1, tempFile);
    }
    fclose(file);
    fclose(tempFile);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found) {
        printf("Book updated successfully!\n");
    } else {
        printf("Book not found.\n");
    }
}

void deleteBook(FILE *file, int id) {
    Book book;
    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error creating temporary file.\n");
        return;
    }
    
    rewind(file);
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id != id) {
            fwrite(&book, sizeof(Book), 1, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found) {
        printf("Book deleted successfully!\n");
    } else {
        printf("Book not found.\n");
    }
}

int main() {
    FILE *file = fopen("library.dat", "rb+");
    if (!file) {
        file = fopen("library.dat", "wb+");
        if (!file) {
            printf("Error opening file.\n");
            return 1;
        }
    }

    int choice, id;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(file);
                break;
            case 2:
                displayBooks(file);
                break;
            case 3:
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                searchBook(file, id);
                break;
            case 4:
                printf("Enter Book ID to update: ");
                scanf("%d", &id);
                updateBook(file, id);
                break;
            case 5:
                printf("Enter Book ID to delete: ");
                scanf("%d", &id);
                deleteBook(file, id);
                break;
            case 6:
                fclose(file);
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
