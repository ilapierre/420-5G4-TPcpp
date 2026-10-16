#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

using namespace std;

// Enum to represent log type
enum LogType {
  // FileManager
  SAVE_LIBRARY,
  LOAD_LIBRARY,
  BACKUP_LIBRARY,
  IMPORT_USERS,
  IMPORT_BOOKS,
  // Library
  ADD_BOOK,
  ADD_USER,
  REMOVE_BOOK,
  CHECKOUT_BOOK,
  RETURN_BOOK
};

class Logger {
private:
    ofstream logFile; // File stream for the log file

    // Converts log type to a string for output
    string typeToString(LogType type) const;

public:
    // Constructor: Opens the log file in append mode
    explicit Logger(const string& filename);

    // Destructor: Closes the log file
    ~Logger();

    // Logs a message with a given log type
    void log(LogType type, const string& message);
};

#endif
