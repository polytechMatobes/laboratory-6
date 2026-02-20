#ifndef BOOK_H
#define BOOK_H

#define MAX_LENGTH 150

enum SortField {
    SORT_BY_AUTHOR = 1,
    SORT_BY_TITLE = 2,
    SORT_BY_YEAR = 3,
    SORT_BY_PRICE = 4,
    SORT_BY_CATEGORY = 5
};

struct Book {
    char author[MAX_LENGTH];
    char title[MAX_LENGTH];
    int year;
    float price;
    char category[MAX_LENGTH];
};

void printBook(const struct Book* book);
void sortLibrary(struct Book* library, int count, enum SortField field);

#endif