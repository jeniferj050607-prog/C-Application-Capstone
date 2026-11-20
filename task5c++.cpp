
#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    bool available;

public:
    Book(int id, std::string title, std::string author,
         bool available = true);

    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    bool isAvailable() const;

    void setAvailable(bool status);

    void display() const;
};

#endif


#include "Book.h"
#include <iostream>

Book::Book(int id, std::string title, std::string author,
           bool available)
    : id(id), title(title), author(author),
      available(available) {
}

int Book::getId() const {
    return id;
}

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

bool Book::isAvailable() const {
    return available;
}

void Book::setAvailable(bool status) {
    available = status;
}

void Book::display() const {
    std::cout << "ID: " << id
              << " | Title: " << title
              << " | Author: " << author
              << " | Status: "
              << (available ? "Available" : "Borrowed")
              << '\n';
}


#ifndef MEMBER_H
#define MEMBER_H

#include <string>

class Member {
private:
    int id;
    std::string name;

public:
    Member(int id, std::string name);

    int getId() const;
    std::string getName() const;

    void display() const;
};

#endif


#include "Member.h"
#include <iostream>

Member::Member(int id, std::string name)
    : id(id), name(name) {
}

int Member::getId() const {
    return id;
}

std::string Member::getName() const {
    return name;
}

void Member::display() const {
    std::cout << "Member ID: " << id
              << " | Name: " << name << '\n';
}


#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Member.h"

#include <vector>
#include <string>

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;

    const std::string bookFile = "data/books.txt";
    const std::string memberFile = "data/members.txt";

public:
    Library();

    void addBook(const Book& book);
    void registerMember(const Member& member);

    void displayBooks() const;
    void displayMembers() const;

    void searchBook(int id) const;

    void borrowBook(int bookId, int memberId);
    void returnBook(int bookId);

    void generateReport() const;

    void saveBooks() const;
    void saveMembers() const;
    void loadBooks();
    void loadMembers();
};

#endif


#include "Library.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

Library::Library() {
    loadBooks();
    loadMembers();
}

// Add a book
void Library::addBook(const Book& book) {
    for (const Book& existing : books) {
        if (existing.getId() == book.getId()) {
            std::cout << "Book ID already exists!\n";
            return;
        }
    }

    books.push_back(book);
    saveBooks();

    std::cout << "Book added successfully!\n";
}

// Register a member
void Library::registerMember(const Member& member) {
    for (const Member& existing : members) {
        if (existing.getId() == member.getId()) {
            std::cout << "Member ID already exists!\n";
            return;
        }
    }

    members.push_back(member);
    saveMembers();

    std::cout << "Member registered successfully!\n";
}

// Display books
void Library::displayBooks() const {
    if (books.empty()) {
        std::cout << "No books available.\n";
        return;
    }

    for (const Book& book : books) {
        book.display();
    }
}

// Display members
void Library::displayMembers() const {
    if (members.empty()) {
        std::cout << "No members registered.\n";
        return;
    }

    for (const Member& member : members) {
        member.display();
    }
}

// Search book by ID
void Library::searchBook(int id) const {
    for (const Book& book : books) {
        if (book.getId() == id) {
            std::cout << "Book found!\n";
            book.display();
            return;
        }
    }

    std::cout << "Book not found.\n";
}

// Borrow book
void Library::borrowBook(int bookId, int memberId) {
    bool memberExists = false;

    for (const Member& member : members) {
        if (member.getId() == memberId) {
            memberExists = true;
            break;
        }
    }

    if (!memberExists) {
        std::cout << "Member does not exist.\n";
        return;
    }

    for (Book& book : books) {
        if (book.getId() == bookId) {
            if (!book.isAvailable()) {
                std::cout << "Book is already borrowed.\n";
                return;
            }

            book.setAvailable(false);
            saveBooks();

            std::cout << "Book borrowed successfully!\n";
            return;
        }
    }

    std::cout << "Book not found.\n";
}

// Return book
void Library::returnBook(int bookId) {
    for (Book& book : books) {
        if (book.getId() == bookId) {
            if (book.isAvailable()) {
                std::cout << "Book is already available.\n";
                return;
            }

            book.setAvailable(true);
            saveBooks();

            std::cout << "Book returned successfully!\n";
            return;
        }
    }

    std::cout << "Book not found.\n";
}

// Report
void Library::generateReport() const {
    int total = books.size();
    int available = 0;
    int borrowed = 0;

    for (const Book& book : books) {
        if (book.isAvailable()) {
            available++;
        }
        else {
            borrowed++;
        }
    }

    std::cout << "\n===== LIBRARY ANALYTICS =====\n";
    std::cout << "Total Books       : " << total << '\n';
    std::cout << "Available Books   : " << available << '\n';
    std::cout << "Borrowed Books    : " << borrowed << '\n';
    std::cout << "Registered Members: " << members.size() << '\n';

    if (total > 0) {
        double percentage =
            (static_cast<double>(borrowed) / total) * 100;

        std::cout << "Borrowing Rate    : "
                  << percentage << "%\n";
    }

    std::cout << "=============================\n";
}

// Save books
void Library::saveBooks() const {
    std::ofstream file(bookFile);

    if (!file) {
        std::cerr << "Error: Could not save books.\n";
        return;
    }

    for (const Book& book : books) {
        file << book.getId() << '|'
             << book.getTitle() << '|'
             << book.getAuthor() << '|'
             << book.isAvailable() << '\n';
    }
}

// Save members
void Library::saveMembers() const {
    std::ofstream file(memberFile);

    if (!file) {
        std::cerr << "Error: Could not save members.\n";
        return;
    }

    for (const Member& member : members) {
        file << member.getId() << '|'
             << member.getName() << '\n';
    }
}

// Load books
void Library::loadBooks() {
    std::ifstream file(bookFile);

    if (!file) {
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string id, title, author, available;

        if (std::getline(ss, id, '|') &&
            std::getline(ss, title, '|') &&
            std::getline(ss, author, '|') &&
            std::getline(ss, available, '|')) {

            try {
                books.emplace_back(
                    std::stoi(id),
                    title,
                    author,
                    available == "1"
                );
            }
            catch (const std::exception&) {
                std::cerr << "Skipping invalid book record.\n";
            }
        }
    }
}

// Load members
void Library::loadMembers() {
    std::ifstream file(memberFile);

    if (!file) {
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string id, name;

        if (std::getline(ss, id, '|') &&
            std::getline(ss, name, '|')) {

            try {
                members.emplace_back(std::stoi(id), name);
            }
            catch (const std::exception&) {
                std::cerr << "Skipping invalid member record.\n";
            }
        }
    }
}



#include "Library.h"

#include <iostream>
#include <limits>

using namespace std;

int getIntegerInput(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            return value;
        }

        cout << "Invalid input. Please enter a number.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    Library library;

    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Search Book\n";
        cout << "4. Register Member\n";
        cout << "5. Display Members\n";
        cout << "6. Borrow Book\n";
        cout << "7. Return Book\n";
        cout << "8. Library Report\n";
        cout << "9. Exit\n";

        choice = getIntegerInput("Enter choice: ");

        switch (choice) {
            case 1: {
                int id = getIntegerInput("Book ID: ");

                string title;
                string author;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Book Title: ";
                getline(cin, title);

                cout << "Author: ";
                getline(cin, author);

                if (title.empty() || author.empty() || id <= 0) {
                    cout << "Invalid book details.\n";
                    break;
                }

                library.addBook(Book(id, title, author));
                break;
            }

            case 2:
                library.displayBooks();
                break;

            case 3: {
                int id = getIntegerInput("Enter Book ID: ");
                library.searchBook(id);
                break;
            }

            case 4: {
                int id = getIntegerInput("Member ID: ");

                string name;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Member Name: ";
                getline(cin, name);

                if (id <= 0 || name.empty()) {
                    cout << "Invalid member details.\n";
                    break;
                }

                library.registerMember(Member(id, name));
                break;
            }

            case 5:
                library.displayMembers();
                break;

            case 6: {
                int bookId = getIntegerInput("Book ID: ");
                int memberId = getIntegerInput("Member ID: ");

                library.borrowBook(bookId, memberId);
                break;
            }

            case 7: {
                int bookId = getIntegerInput("Book ID: ");
                library.returnBook(bookId);
                break;
            }

            case 8:
                library.generateReport();
                break;

            case 9:
                cout << "Thank you for using Library Manager!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 9);

    return 0;
}

101|C++ Programming|Bjarne Stroustrup|1
102|Python Basics|John Smith|0
103|Database Systems|Abraham Silberschatz|1

1|Arun
2|Priya
3|Kavi


#include "../include/Book.h"
#include "../include/Member.h"

#include <cassert>
#include <iostream>

void testBookCreation() {
    Book book(101, "C++ Programming", "Bjarne Stroustrup");

    assert(book.getId() == 101);
    assert(book.getTitle() == "C++ Programming");
    assert(book.getAuthor() == "Bjarne Stroustrup");
    assert(book.isAvailable());

    std::cout << "Book creation test passed.\n";
}

void testBookBorrowingStatus() {
    Book book(102, "Python Basics", "John");

    book.setAvailable(false);

    assert(!book.isAvailable());

    book.setAvailable(true);

    assert(book.isAvailable());

    std::cout << "Book status test passed.\n";
}

void testMemberCreation() {
    Member member(1, "Arun");

    assert(member.getId() == 1);
    assert(member.getName() == "Arun");

    std::cout << "Member creation test passed.\n";
}

void testBoundaryValues() {
    Book book(1, "", "");

    assert(book.getId() == 1);
    assert(book.getTitle().empty());
    assert(book.getAuthor().empty());

    std::cout << "Boundary test passed.\n";
}

int main() {
    std::cout << "===== CAPSTONE TESTS =====\n\n";

    testBookCreation();
    testBookBorrowingStatus();
    testMemberCreation();
    testBoundaryValues();

    std::cout << "\nAll tests passed!\n";

    return 0;
}


