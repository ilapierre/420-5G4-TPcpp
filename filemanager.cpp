#include <fstream>
#include <iostream>
#include <filesystem>
#include "filemanager.h"
#include "utils.h"

using namespace std;

// Constructor
FileManager::FileManager(const string& booksFile, const string& usersFile)
    : booksFileName(booksFile), usersFileName(usersFile) {}

// Save all library data
bool FileManager::saveLibraryData(Library& library) {
    return saveBooksToFile(library) && saveUsersToFile(library);
}

// Load all library data
bool FileManager::loadLibraryData(Library& library) {
    bool booksLoaded = loadBooksFromFile(library);
    bool usersLoaded = loadUsersFromFile(library);
    return booksLoaded || usersLoaded; // Return true if at least one file was loaded
}

// Save books to file
bool FileManager::saveBooksToFile(Library& library) {
    ofstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << booksFileName << " en écriture.\n";
        return false;
    }
    
    auto books = library.getAllBooks();
    for (Book* book : books) {
        file << book->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Save users to file
bool FileManager::saveUsersToFile(Library& library) {
    ofstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << usersFileName << " en écriture.\n";
        return false;
    }
    
    auto users = library.getAllUsers();
    for (User* user : users) {
        file << user->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Load books from file
bool FileManager::loadBooksFromFile(Library& library) {
    ifstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier de livres existant trouvé. Démarrage avec une bibliothèque vide.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            Book book;
            book.fromFileFormat(line);
            library.addBook(book);
            count++;
        }
    }
    
    file.close();
    cout << "Chargé " << count << " livre(s) depuis le fichier.\n";
    return true;
}

// Load users from file
bool FileManager::loadUsersFromFile(Library& library) {
    ifstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier d'utilisateurs existant trouvé. Démarrage sans utilisateurs enregistrés.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            User user;
            user.fromFileFormat(line);
            library.addUser(user);
            count++;
        }
    }
    
    file.close();
    cout << "Chargé " << count << " utilisateur(s) depuis le fichier.\n";
    return true;
}

// Check if file exists
bool FileManager::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Create backup
void FileManager::createBackup() {
    if (fileExists(booksFileName)) {
        filesystem::copy_file(booksFileName, booksFileName + ".backup");
    }
    
    if (fileExists(usersFileName)) {
        filesystem::copy_file(usersFileName, usersFileName + ".backup");
    }
    
    cout << "Fichiers de sauvegarde créés.\n";
}

// Import Books by CSV
bool FileManager::importBooksCSV(Library &library, string csvPath) {
  ifstream file(csvPath);

  if (!file.is_open()) {
    cout << "Erreur : Impossible d'ouvrir le fichier de livres (" + csvPath + ").\n";
    return false;
  }

  string line;
  int count = 0;
  int lineNumber = 0;

  // Validate every book
  while (getline(file, line)) {
    lineNumber++;

    if (lineNumber == 1) {
      // Skip the header of CSVs
      continue;
    }

    if (trim(line).empty())
      continue;

    Book book;
    book.fromCSV(line);

    if (!verifyBookConflict(library, book)) {
      cout << csvPath << ":" << "Ligne " << lineNumber << endl;
      cout << "Import interrompu : " << count << " livre(s) chargé(s) avant l'erreur." << endl;
      return false;
    }

    library.addBook(book);

    // Borrow book if imported book had a borrowerId
    if (book.getBorrowerId() != "") {
      User *user = library.findUserById(book.getBorrowerId());
      user->borrowBook(book.getISBN());
    }

    count++;
  }

  file.close();
  cout << "Chargé " << count << " livre(s) depuis le fichier.\n";
  return true;
}

// Import Users by CSV
bool FileManager::importUsersCSV(Library &library, string csvPath) {
  ifstream file(csvPath);

  if (!file.is_open()) {
    cout << "Erreur : Impossible d'ouvrir le fichier d'utilisateurs (" + csvPath + ").\n";
    return false;
  }

  string line;
  int count = 0;
  int lineNumber = 0;

  // Validate every user
  while (getline(file, line)) {
    lineNumber++;

    if (lineNumber == 1) {
      // Skip the header of CSVs
      continue;
    }

    if (trim(line).empty())
      continue;

    User user;
    user.fromCSV(line);

    if (!verifyUserConflict(library, user)) {
      cout << csvPath << ":" << "Ligne " << lineNumber << endl;
      cout << "Import interrompu : " << count << " utilisateur(s) chargé(s) avant l'erreur." << endl;
      return false;
    }

    library.addUser(user);

    // Borrow books if imported user had borrowed books
    for (const string &isbn : user.getBorrowedBooks()) {
      Book *book = library.findBookByISBN(isbn);
      book->checkOut(user.getUserId());
    }

    count++;
  }

  file.close();
  cout << "Chargé " << count << " utilisateur(s) depuis le fichier.\n";
  return true;
}

// Export CSV
bool FileManager::exportCSV(Library &library) { return true; }

bool FileManager::verifyBookConflict(Library &library, Book& book) {
  string title = book.getTitle();
  string author = book.getAuthor();
  string isbn = book.getISBN();
  bool isAvailable = book.getAvailability();
  string borrowerId = book.getBorrowerId();

  // Check for empty fields
  if (title.empty() || author.empty() || isbn.empty()) {
    cout << "\nErreur : Tous les champs sont requis. Veuillez réessayer." << endl;
    return false;
  }

  // Check unique ISBN
  if (library.findBookByISBN(isbn)) {
    cout << "\nErreur : Un livre avec l'ISBN " << isbn << " existe déjà. " << endl;
    return false;
  }

  // Check unique title + author
  for (Book *b : library.searchBooksByTitle(title)) {
    if (b->getAuthor() == author) {
      cout << "\nErreur : Un livre de titre (" << title
           << ") de l'auteur (" + author + ") existe déjà." << endl;
      return false;
    }
  }

  // Check for borrower ID + Availablity mismatch
  if (isAvailable && borrowerId != "") {
    cout << "\nErreur: Le livre '" << title << "' est marqué comme disponible alors qu'il est emprunté" << endl;
    return false;
  }

  if (!isAvailable && borrowerId == "") {
    cout << "\nErreur: Le livre '" << title << "' est marqué comme indisponbile alors qu'il n'est pas emprunté" << endl;
    return false;
  }

  // Check for existing users
  if (borrowerId != "" && library.findUserById(borrowerId) == NULL) {
    cout << "\nErreur: L'utilisateur " << borrowerId << " n'existe pas" << endl;
    return false;
  }

  return true;
}

bool FileManager::verifyUserConflict(Library &library, User &user) {
  string name = user.getName();
  string userId = user.getUserId();
  vector<string> borrowedBooks = user.getBorrowedBooks();

  // Check for empty fields
  if (name.empty() || userId.empty()) {
    cout << "\nErreur : Tous les champs sont requis. Veuillez réessayer." << endl;
    return false;
  }

  // Check unique user ID
  if (library.findUserById(userId)) {
    cout << "\nErreur : Un utilisateur avec l'ID " << userId << " existe déjà. " << endl;
    return false;
  }

  for (const string &isbn : borrowedBooks) {
    // Check for existing books
    Book *book = library.findBookByISBN(isbn);
    if (book == NULL) {
      cout << "\nErreur: Le livre avec l'ISBN " << isbn << " n'existe pas" << endl;
      return false;
    }

    // Check for borrowed book + Availablity mismatch
    if (!book->getAvailability()) {
      cout << "\nErreur: Le livre '" << book->getTitle() << "' est déjà emprunté" << endl;
      return false;
    }
  }

  return true;
}
