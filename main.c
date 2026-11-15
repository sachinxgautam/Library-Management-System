#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_BOOKS 1000
#define FILE_NAME "books.dat"

struct Book {
    int id;
    char title[100];
    char author[100];
    int issued;
};

struct Book books[MAX_BOOKS];
int count = 0;

// Read integer safely
int readInt(char message[]) {
    char input[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }

        value = strtol(input, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end == input || *end != '\0' ||
            value < INT_MIN || value > INT_MAX) {
            printf("Invalid input! Enter a valid number.\n");
        } else {
            return (int)value;
        }
    }
}

// Read text safely
void readText(char message[], char text[], int size) {
    printf("%s", message);

    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}

// Save books to file
void saveBooks() {
    FILE *fp = fopen(FILE_NAME, "wb");

    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), count, fp);

    fclose(fp);
}

// Load books from file
void loadBooks() {
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        count = 0;
        return;
    }

    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        count < 0 || count > MAX_BOOKS) {
        count = 0;
        fclose(fp);
        return;
    }

    if (fread(books, sizeof(struct Book), count, fp) !=
        (size_t)count) {
        count = 0;
    }

    fclose(fp);
}

// Find book by ID
int findBook(int id) {
    for (int i = 0; i < count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }

    return -1;
}

// Add a new book
void addBook() {
    if (count >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }

    int id = readInt("Enter Book ID: ");

    if (id <= 0) {
        printf("Invalid Book ID!\n");
        return;
    }

    if (findBook(id) != -1) {
        printf("Book ID already exists!\n");
        return;
    }

    books[count].id = id;

    readText("Enter Book Title: ",
             books[count].title, 100);

    readText("Enter Author Name: ",
             books[count].author, 100);

    if (strlen(books[count].title) == 0 ||
        strlen(books[count].author) == 0) {
        printf("Title and author cannot be empty!\n");
        return;
    }

    books[count].issued = 0;
    count++;

    saveBooks();

    printf("\nBook added successfully!\n");
}

// View all books
void viewBooks() {
    if (count == 0) {
        printf("\nNo books available!\n");
        return;
    }

    printf("\n========== BOOK LIST ==========\n");

    for (int i = 0; i < count; i++) {
        printf("\nBook ID: %d\n", books[i].id);
        printf("Title: %s\n", books[i].title);
        printf("Author: %s\n", books[i].author);

        if (books[i].issued) {
            printf("Status: Issued\n");
        } else {
            printf("Status: Available\n");
        }
    }

    printf("\n================================\n");
}

// Search book
void searchBook() {
    int id = readInt("Enter Book ID to search: ");

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    printf("\nBook Found!\n");
    printf("ID: %d\n", books[index].id);
    printf("Title: %s\n", books[index].title);
    printf("Author: %s\n", books[index].author);
    printf("Status: %s\n",
           books[index].issued ? "Issued" : "Available");
}

// Issue book
void issueBook() {
    int id = readInt("Enter Book ID to issue: ");

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    if (books[index].issued) {
        printf("Book is already issued!\n");
        return;
    }

    books[index].issued = 1;
    saveBooks();

    printf("Book issued successfully!\n");
}

// Return book
void returnBook() {
    int id = readInt("Enter Book ID to return: ");

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    if (!books[index].issued) {
        printf("Book was not issued!\n");
        return;
    }

    books[index].issued = 0;
    saveBooks();

    printf("Book returned successfully!\n");
}

// Main function
int main() {
    int choice;

    loadBooks();

    do {
        printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n");
        printf("1. Add Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Exit\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1:
                addBook();
                break;

            case 2:
                viewBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                returnBook();
                break;

            case 6:
                printf("\nThank you for using Library Management System!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}