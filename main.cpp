#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

// Book class
class Book {
private:
    int bookId;
    string title;
    string author;
    bool isIssued;
    string issuedTo;

public:
    Book(int id, string t, string a) : bookId(id), title(t), author(a), isIssued(false), issuedTo("") {}
    
    // Getters
    int getBookId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIsIssued() const { return isIssued; }
    string getIssuedTo() const { return issuedTo; }
    
    // Setters
    void issueBook(string memberName) {
        isIssued = true;
        issuedTo = memberName;
    }
    
    void returnBook() {
        isIssued = false;
        issuedTo = "";
    }
    
    void display() const {
        cout << left << setw(5) << bookId 
             << setw(25) << title 
             << setw(20) << author 
             << setw(10) << (isIssued ? "Issued" : "Available")
             << setw(15) << issuedTo << endl;
    }
};

// Member class
class Member {
private:
    int memberId;
    string name;
    vector<int> borrowedBooks;

public:
    Member(int id, string n) : memberId(id), name(n) {}
    
    // Getters
    int getMemberId() const { return memberId; }
    string getName() const { return name; }
    vector<int>& getBorrowedBooks() { return borrowedBooks; }
    
    void borrowBook(int bookId) {
        borrowedBooks.push_back(bookId);
    }
    
    void returnBook(int bookId) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookId);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
        }
    }
    
    void display() const {
        cout << "Member ID: " << memberId << ", Name: " << name 
             << ", Books Borrowed: " << borrowedBooks.size() << endl;
        if (!borrowedBooks.empty()) {
            cout << "Book IDs: ";
            for (int id : borrowedBooks) {
                cout << id << " ";
            }
            cout << endl;
        }
    }
};

// Library Management System class
class Library {
private:
    vector<Book*> books;
    vector<Member*> members;
    int nextBookId;
    int nextMemberId;

public:
    Library() : nextBookId(1), nextMemberId(1) {}
    
    ~Library() {
        for (auto book : books) delete book;
        for (auto member : members) delete member;
    }
    
    // Add book
    void addBook() {
        string title, author;
        cout << "\nEnter Book Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        
        Book* book = new Book(nextBookId++, title, author);
        books.push_back(book);
        cout << "Book added successfully! Book ID: " << book->getBookId() << endl;
    }
    
    // Add member
    void addMember() {
        string name;
        cout << "\nEnter Member Name: ";
        cin.ignore();
        getline(cin, name);
        
        Member* member = new Member(nextMemberId++, name);
        members.push_back(member);
        cout << "Member added successfully! Member ID: " << member->getMemberId() << endl;
    }
    
    // Issue book
    void issueBook() {
        displayBooks();
        if (books.empty()) {
            cout << "No books available!" << endl;
            return;
        }
        
        int bookId, memberId;
        cout << "\nEnter Book ID to issue: ";
        cin >> bookId;
        cout << "Enter Member ID: ";
        cin >> memberId;
        
        Book* book = findBook(bookId);
        Member* member = findMember(memberId);
        
        if (book && member && !book->getIsIssued()) {
            book->issueBook(member->getName());
            member->borrowBook(bookId);
            cout << "Book issued successfully to " << member->getName() << endl;
        } else if (book && book->getIsIssued()) {
            cout << "Book is already issued!" << endl;
        } else {
            cout << "Book or Member not found!" << endl;
        }
    }
    
    // Return book
    void returnBook() {
        displayBooks();
        if (books.empty()) {
            cout << "No books available!" << endl;
            return;
        }
        
        int bookId;
        cout << "\nEnter Book ID to return: ";
        cin >> bookId;
        
        Book* book = findBook(bookId);
        if (book && book->getIsIssued()) {
            string memberName = book->getIssuedTo();
            Member* member = findMemberByName(memberName);
            if (member) {
                member->returnBook(bookId);
            }
            book->returnBook();
            cout << "Book returned successfully!" << endl;
        } else {
            cout << "Book not found or not issued!" << endl;
        }
    }
    
    // Display all books
    void displayBooks() const {
        cout << "\n" << string(75, '=') << endl;
        cout << "                    LIBRARY BOOK INVENTORY" << endl;
        cout << string(75, '=') << endl;
        cout << left << setw(5) << "ID" 
             << setw(25) << "Title" 
             << setw(20) << "Author" 
             << setw(10) << "Status"
             << setw(15) << "Issued To" << endl;
        cout << string(75, '-') << endl;
        
        if (books.empty()) {
            cout << "No books in library!" << endl;
        } else {
            for (const auto& book : books) {
                book->display();
            }
        }
        cout << string(75, '=') << endl;
    }
    
    // Display all members
    void displayMembers() const {
        cout << "\n" << string(50, '=') << endl;
        cout << "                      MEMBERS LIST" << endl;
        cout << string(50, '=') << endl;
        
        if (members.empty()) {
            cout << "No members registered!" << endl;
        } else {
            for (const auto& member : members) {
                member->display();
                cout << endl;
            }
        }
        cout << string(50, '=') << endl;
    }
    
    // Search book by title or author
    void searchBook() const {
        string searchTerm;
        cout << "\nEnter title or author to search: ";
        cin.ignore();
        getline(cin, searchTerm);
        
        bool found = false;
        for (const auto& book : books) {
            if (book->getTitle().find(searchTerm) != string::npos || 
                book->getAuthor().find(searchTerm) != string::npos) {
                book->display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No books found matching your search!" << endl;
        }
    }
    
private:
    Book* findBook(int id) {
        for (auto book : books) {
            if (book->getBookId() == id) return book;
        }
        return nullptr;
    }
    
    Member* findMember(int id) {
        for (auto member : members) {
            if (member->getMemberId() == id) return member;
        }
        return nullptr;
    }
    
    Member* findMemberByName(const string& name) {
        for (auto member : members) {
            if (member->getName() == name) return member;
        }
        return nullptr;
    }
};

// Menu-driven interface
void showMenu() {
    cout << "\n" << string(50, '*') << endl;
    cout << "         LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << string(50, '*') << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Add Member" << endl;
    cout << "3. Issue Book" << endl;
    cout << "4. Return Book" << endl;
    cout << "5. Display All Books" << endl;
    cout << "6. Display All Members" << endl;
    cout << "7. Search Book" << endl;
    cout << "0. Exit" << endl;
    cout << string(50, '*') << endl;
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice;
    
    cout << "Welcome to Library Management System!" << endl;
    
    do {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.addMember();
                break;
            case 3:
                library.issueBook();
                break;
            case 4:
                library.returnBook();
                break;
            case 5:
                library.displayBooks();
                break;
            case 6:
                library.displayMembers();
                break;
            case 7:
                library.searchBook();
                break;
            case 0:
                cout << "\nThank you for using Library Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);
    
    return 0;
}
