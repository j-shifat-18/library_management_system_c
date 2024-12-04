#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void addBook();
void searchBook();
void viewBooks();
void removeBook();
void issueBook();
void viewIssue();
void studentInfo();
void returnBook();
void toUpperCase(char str[]);

struct book
{
    int id;
    char book_name[100];
    char author_name[100];
    char date[12];
} b;

struct student
{
    int id;
    char sName[100];
    char sClass[100];
    int sId;
    char date[12];
    char book_name[100];
} s;

FILE *file;
FILE *ftemp;

int main()
{
    int n;

    while (1)
    {
        system("cls");
        printf("***************Welcome to IUT Library***************\n");
        printf("1.Add book.\n");
        printf("2.Search book.\n");
        printf("3.View books list.\n");
        printf("4.Remove book.\n");
        printf("5.Issue book.\n");
        printf("6.View issued book list.\n");
        printf("7.Student Information.\n");
        printf("8.Return book.\n");
        printf("0.Exit.\n");
        printf("-----------------------------------------------------\n");
        printf("Enter your choice:");
        scanf("%d", &n);
        getchar();

        switch (n)
        {
        case 0:
            exit(0);
        case 1:
            addBook();
            break;
        case 2:
            searchBook();
            break;
        case 3:
            viewBooks();
            break;
        case 4:
            removeBook();
            break;
        case 5:
            issueBook();
            break;
        case 6:
            viewIssue();
            break;
        case 7:
            studentInfo();
            break;
        case 8:
            returnBook();
            break;
        default:
            printf("Enter a valid number (1-10).\n");
        }

        printf("Press Any Key To Continue...");
        getch();
    }

    return 0;
}

void addBook()
{
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(b.date, myDate);

    file = fopen("books.txt", "ab");

    printf("Enter book id : ");
    scanf("%d", &b.id);
    getchar();

    printf("Enter book name : ");
    gets(b.book_name);
    toUpperCase(b.book_name);

    printf("Enter author name : ");
    fflush(stdin);
    gets(b.author_name);
    toUpperCase(b.author_name);

    printf("Book added successfully.\n");

    fwrite(&b, sizeof(b), 1, file);
    fclose(file);
}

void searchBook()
{
    char bookName[100];
    int f = 0;
    system("cls");
    printf("<== Search Book ==>\n\n");
    printf("Enter book name to search : ");
    fflush(stdin);
    gets(bookName);
    toUpperCase(bookName);

    file = fopen("books.txt", "rb");

    ftemp = fopen("temp.txt", "wb");

    while (fread(&b, sizeof(b), 1, file) == 1)
    {
        if (strcmp(bookName, b.book_name) == 0)
        {
            f = 1;
            fwrite(&b, sizeof(b), 1, ftemp);
        }
    }

    fclose(ftemp);

    if (f == 0)
    {
        printf("Sorry ,no record was found!\n");
    }
    else
    {
        printf("%-10s %-30s %-20s %s\n\n", "Book Id", "Book Name", "Author Name", "Date");
        ftemp = fopen("temp.txt", "rb");

        while (fread(&b, sizeof(b), 1, ftemp) == 1)
        {
            printf("%-10d %-30s %-20s %s\n", b.id, b.book_name, b.author_name, b.date);
        }
    }

    fclose(file);
    fclose(ftemp);
    remove("temp.txt");
}

void viewBooks()
{
    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book Id", "Book Name", "Author Name", "Date");

    file = fopen("books.txt", "rb");

    if (file == NULL)
    {
        printf("Error: Unable to open books file!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, file) == 1)
    {
        printf("%-10d %-30s %-20s %s\n", b.id, b.book_name, b.author_name, b.date);
    }

    fclose(file);
}

void removeBook()
{
    int id, f = 0;
    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter book id to remove : ");
    scanf("%d", &id);
    getchar();

    file = fopen("books.txt", "rb");
    if (file == NULL)
    {
        printf("Error: Unable to open books file!\n");
        return;
    }

    ftemp = fopen("temp.txt", "wb");
    if (ftemp == NULL)
    {
        printf("Error: Unable to create temporary file!\n");
        fclose(file);
        return;
    }

    while (fread(&b, sizeof(b), 1, file) == 1)
    {
        if (id == b.id)
        {
            f = 1;
        }
        else
        {
            fwrite(&b, sizeof(b), 1, ftemp);
        }
    }

    fclose(file);
    fclose(ftemp);

    if (f == 1)
    {
        printf("\n\nBook Removed Successfully.\n");
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }
    else
    {
        printf("\n\nSorry, no record was found!\n");
        remove("temp.txt");
    }
    
}

void issueBook()
{
    char issueDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(issueDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(s.date, issueDate);

    int f = 0;

    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &s.id);
    getchar();

    file = fopen("books.txt", "rb");
    if (file == NULL)
    {
        printf("File not found.\n");
    }

    while (fread(&b, sizeof(b), 1, file) == 1)
    {
        if (b.id == s.id)
        {
            f = 1;
            strcpy(s.book_name, b.book_name);
            break;
        }
    }

    fclose(file);

    if (f == 0)
    {
        printf("No book found with this id\n");
        printf("Please try again...\n\n");
        return;
    }

    file = fopen("issue.txt", "ab");

    printf("Enter student name : ");
    fflush(stdin);
    gets(s.sName);
    toUpperCase(s.sName);

    printf("Enter student class : ");
    fflush(stdin);
    gets(s.sClass);
    toUpperCase(s.sClass);

    printf("Enter student roll : ");
    scanf("%d", &s.sId);

    printf("Book Issued Successfully\n\n");

    fwrite(&s, sizeof(s), 1, file);
    fclose(file);
}

void viewIssue()
{
    system("cls");
    printf("<== Book Issue List ==>\n\n");

    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "BOOK id", "Name", "Class", "Roll", "Book Name", "Issue Date");

    file = fopen("issue.txt", "rb");
    if (file == NULL)
    {
        printf("File not found.\n");
    }
    while (fread(&s, sizeof(s), 1, file) == 1)
    {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sId, s.book_name, s.date);
    }

    fclose(file);
}

void studentInfo()
{
    int roll;
    char class[100];
    int f = 0;
    system("cls");
    printf("<== Student Information ==>\n\n");

    printf("Enter student class : ");
    fflush(stdin);
    gets(class);
    toUpperCase(class);

    printf("Enter student roll : ");
    scanf("%d", &roll);
    getchar();

    file = fopen("issue.txt", "rb");
    ftemp = fopen("temp.txt", "wb");

    while (fread(&s, sizeof(s), 1, file) == 1)
    {
        if ((roll == s.sId) && (strcmp(class, s.sClass) == 0))
        {
            fwrite(&s, sizeof(s), 1, ftemp);
            f = 1;
        }
    }

    fclose(file);
    fclose(ftemp);

    printf("\n");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "BOOK id", "Name", "Class", "Roll", "Book Name", "Issue Date");

    ftemp = fopen("temp.txt", "rb");

    while (fread(&s, sizeof(s), 1, ftemp) == 1)
    {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sId, s.book_name, s.date);
    }

    fclose(ftemp);
    remove("temp.txt");
}

void returnBook()
{
    int id, roll, f = 0;
    char class[100];
    system("cls");
    printf("<== Remove Issued Books ==>\n\n");
    printf("Enter book id to return : ");
    scanf("%d", &id);
    getchar();

    printf("Enter student class : ");
    gets(class);
    toUpperCase(class);

    printf("Enter student roll : ");
    scanf("%d", &roll);
    getchar();

    file = fopen("issue.txt", "rb");
    if (file == NULL)
    {
        printf("Error: Unable to issue books file!\n");
        return;
    }
    ftemp = fopen("temp.txt", "wb");
    if (file == NULL)
    {
        printf("Error: Unable to temporary  file!\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, file) == 1)
    {
        if ((id == s.id) && (roll == s.sId) && (strcmp(class, s.sClass) == 0))
        {
            f = 1;
        }
        else
        {
            fwrite(&s, sizeof(s), 1, ftemp);
        }
    }

    fclose(file);
    fclose(ftemp);

    if (f == 1)
    {
        printf("\n\nBook returned Successfully.\n");
        remove("issue.txt");
        rename("temp.txt", "issue.txt");
    }
    else
    {
        printf("\n\nSorry, no record was found!\n");
        remove("temp.txt");
    }
}

void toUpperCase(char str[])
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = toupper(str[i]);
        i++;
    }
}