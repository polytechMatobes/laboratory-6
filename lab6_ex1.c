#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"

#define INITIAL_CAPACITY 5
#define FILENAME "library.txt"

void printMenu();
void printLibrary(struct Book* library, int count);
void addBook(struct Book** library, int* count, int* capacity);
void deleteBook(struct Book* library, int* count);
void saveToFile(struct Book* library, int count);
void loadFromFile(struct Book** library, int* count, int* capacity);

int main() {
    struct Book* library = NULL;  
    int count = 0;                 
    int capacity = 0;              
    
    int choice;
    do {
        printMenu();
        printf("Выберите действие: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                printLibrary(library, count);
                break;
            case 2:
                addBook(&library, &count, &capacity);
                break;
            case 3:
                deleteBook(library, &count);
                break;
            case 4:
                saveToFile(library, count);
                break;
            case 5:
                loadFromFile(&library, &count, &capacity);
                break;
            case 6:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
        printf("\n");
    } while(choice != 6);
    
    free(library); 
    return 0;
}

void printMenu() {
    printf("=== МЕНЮ КАРТОТЕКИ ===\n");
    printf("1. Распечатать содержимое картотеки\n");
    printf("2. Ввести новую книгу\n");
    printf("3. Удалить существующую книгу\n");
    printf("4. Сохранить в файл\n");
    printf("5. Загрузить из файла\n");
    printf("6. Выход\n");
    printf("=======================\n");
}

void printLibrary(struct Book* library, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }
    
    printf("=== КНИГИ В КАРТОТЕКЕ ===\n");
    for (int i = 0; i < count; i++) {
        printf("Книга #%d:\n", i + 1);
        printBook(&library[i]);
        printf("-----------------------------\n");
    }
}

void addBook(struct Book** library, int* count, int* capacity) {
    if (*capacity == 0) {
        *capacity = INITIAL_CAPACITY;
        *library = (struct Book*)malloc(*capacity * sizeof(struct Book));
        if (*library == NULL) {
            printf("Ошибка выделения памяти!\n");
            return;
        }
    }
    else if (*count >= *capacity) {
        *capacity *= 2;
        struct Book* temp = (struct Book*)realloc(*library, *capacity * sizeof(struct Book));
        if (temp == NULL) {
            printf("Ошибка перевыделения памяти!\n");
            return;
        }
        *library = temp;
    }
    
    struct Book newBook;
    char buffer[200];
    
    printf("Введите данные о книге:\n");
    
    printf("Автор: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(newBook.author, buffer, MAX_LENGTH - 1);
    newBook.author[MAX_LENGTH - 1] = '\0';
    
    printf("Название: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(newBook.title, buffer, MAX_LENGTH - 1);
    newBook.title[MAX_LENGTH - 1] = '\0';
    
    printf("Год издания: ");
    scanf("%d", &newBook.year);
    getchar();
    
    printf("Цена: ");
    scanf("%f", &newBook.price);
    getchar();
    
    printf("Категория: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(newBook.category, buffer, MAX_LENGTH - 1);
    newBook.category[MAX_LENGTH - 1] = '\0';
    
    (*library)[*count] = newBook;
    (*count)++;
    
    printf("Книга добавлена успешно!\n");
}

void deleteBook(struct Book* library, int* count) {
    if (*count == 0) {
        printf("Картотека пуста. Нечего удалять.\n");
        return;
    }
    
    printLibrary(library, *count);
    
    int index;
    printf("Введите номер книги для удаления (1-%d): ", *count);
    scanf("%d", &index);
    getchar();
    
    if (index < 1 || index > *count) {
        printf("Неверный номер книги.\n");
        return;
    }
    
    for (int i = index - 1; i < *count - 1; i++) {
        library[i] = library[i + 1];
    }
    
    (*count)--;
    printf("Книга удалена успешно!\n");
}

void saveToFile(struct Book* library, int count) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }
    
    fprintf(file, "%d\n", count);
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", library[i].author);
        fprintf(file, "%s\n", library[i].title);
        fprintf(file, "%d\n", library[i].year);
        fprintf(file, "%.2f\n", library[i].price);
        fprintf(file, "%s\n", library[i].category);
    }
    
    fclose(file);
    printf("Картотека сохранена в файл '%s' (%d книг).\n", FILENAME, count);
}

void loadFromFile(struct Book** library, int* count, int* capacity) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Файл '%s' не найден или не может быть открыт.\n", FILENAME);
        return;
    }
    
    int fileCount;
    if (fscanf(file, "%d", &fileCount) != 1) {
        printf("Ошибка чтения данных из файла.\n");
        fclose(file);
        return;
    }
    getc(file);
    
    if (fileCount > *capacity) {
        *capacity = fileCount;
        struct Book* temp = (struct Book*)realloc(*library, *capacity * sizeof(struct Book));
        if (temp == NULL) {
            printf("Ошибка выделения памяти!\n");
            fclose(file);
            return;
        }
        *library = temp;
    }
    
    *count = 0;
    char buffer[MAX_LENGTH * 2];
    
    for (int i = 0; i < fileCount; i++) {
        struct Book book;
        
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(book.author, buffer, MAX_LENGTH - 1);
        book.author[MAX_LENGTH - 1] = '\0';
        
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(book.title, buffer, MAX_LENGTH - 1);
        book.title[MAX_LENGTH - 1] = '\0';
        
        fscanf(file, "%d", &book.year);
        getc(file);
        
        fscanf(file, "%f", &book.price);
        getc(file);
        
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(book.category, buffer, MAX_LENGTH - 1);
        book.category[MAX_LENGTH - 1] = '\0';
        
        (*library)[*count] = book;
        (*count)++;
    }
    
    fclose(file);
    printf("Загружено %d книг из файла '%s'.\n", *count, FILENAME);
}

void printBook(const struct Book* book) {
    printf("Автор: %s\n", book->author);
    printf("Название: %s\n", book->title);
    printf("Год издания: %d\n", book->year);
    printf("Цена: %.2f руб.\n", book->price);
    printf("Категория: %s\n", book->category);
}