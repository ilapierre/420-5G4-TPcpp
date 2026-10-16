// NOTE: Solution adaptée de:
// https://www.geeksforgeeks.org/cpp/logging-system-in-cpp/

#include <ctime>
#include <iostream>
#include <sstream>

#include "logger.h"

using namespace std;

// Constructor: Opens the log file in append mode
Logger::Logger(const string &filename) {
  logFile.open(filename, ios::app);
  if (!logFile.is_open()) {
    cerr << "Error opening log file." << endl;
  }
}

// Destructor: Closes the log file
Logger::~Logger() { logFile.close(); }

// Logs a message with a given log type
void Logger::log(LogType type, const string &message) {
  // Get current timestamp
  time_t now = time(0);
  tm *timeinfo = localtime(&now);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

  // Create log entry
  ostringstream logEntry;
  logEntry << "[" << timestamp << "] " << typeToString(type) << ": "
           << message << endl;

  // Output to console
  cout << logEntry.str();

  // Output to log file
  if (logFile.is_open()) {
    logFile << logEntry.str();
    logFile.flush(); // Ensure immediate write to file
  }
}

// Converts log type to a string for output
string Logger::typeToString(LogType type) const {
  switch (type) {
    // FileManager
    case SAVE_LIBRARY:
      return "SAUVEGARDE LIBRAIRIE";
    case LOAD_LIBRARY:
      return "CHARGEMENT LIBRAIRIE";
    case BACKUP_LIBRARY:
      return "BACKUP LIBRAIRIE";
    case IMPORT_USERS:
      return "IMPORT UTILISATEURS";
    case IMPORT_BOOKS:
      return "IMPORT LIVRES";
    // Library
    case ADD_BOOK:
      return "AJOUT LIVRE";
    case ADD_USER:
      return "AJOUT UTILISATEUR";
    case REMOVE_BOOK:
      return "SUPPRESSION LIVRE";
    case CHECKOUT_BOOK:
      return "EMPRUNT";
    case RETURN_BOOK:
      return "RETOUR LIVRE";
    default:
      return "INCONNU";
  }
}
