#include <sstream>
#include <string>

#include "book.h"

using namespace std;

// Default constructor
Book::Book() {}

// Parameterized constructor
Book::Book(const string &title, const string &author, const string &isbn) {
  this->title = title;
  this->author = author;
  this->isbn = isbn;
  this->isAvailable = true;
  this->borrowerId = "";
}

// Getters
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getISBN() const { return isbn; }
bool Book::getAvailability() const { return isAvailable; }
string Book::getBorrowerId() const { return borrowerId; }

// Setters
void Book::setTitle(const string &title) { this->title = title; }
void Book::setAuthor(const string &author) { this->author = author; }
void Book::setISBN(const string &isbn) { this->isbn = isbn; }
void Book::setAvailability(bool available) { this->isAvailable = available; }
void Book::setBorrowerId(const string &id) { this->borrowerId = id; }

// Check out a book
void Book::checkOut(const string &borrowerId) {
  setAvailability(false);
  setBorrowerId(borrowerId);
}

// Return a book
void Book::returnBook() {
  setAvailability(true);
  setBorrowerId("");
}

// Display book information
string Book::toString() const {
  string result = "Titre: " + title + "\nAuteur: " + author +
                  "\nISBN: " + isbn + "\nStatut: ";

  if (!isAvailable) {
    result += "Emprunté par : " + borrowerId;
  } else {
    result += "Disponible";
  }

  return result;
}

// Format for file storage
string Book::toFileFormat() const {
  if (isAvailable) {
    return title + "|" + author + "|" + isbn + "|" + "0" + "|";
  }

  return title + "|" + author + "|" + isbn + "|" + "1" + "|" + borrowerId;
}

// Parse from file format
void Book::fromFileFormat(const string &line) {
  stringstream ss(line);

  getline(ss, title, '|');
  getline(ss, author, '|');
  getline(ss, isbn, '|');

  string available;
  getline(ss, available, '|');

  isAvailable = true;
  if (available == "0") {
    isAvailable = false;
  }

  getline(ss, borrowerId, '|');
}

void Book::fromCSV(const string &line) {
  stringstream ss(line);

  getline(ss, title, ',');
  getline(ss, author, ',');
  getline(ss, isbn, ',');

  string available;
  getline(ss, available, ',');

  isAvailable = true;
  if (available == "0") {
    isAvailable = false;
  }

  getline(ss, borrowerId, '|');
}
