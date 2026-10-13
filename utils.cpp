#include "utils.h"
#include <algorithm>
#include <cctype>

// NOTE: La solution du trim a été trouvée ici:
// https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring

// Left-side trim
string ltrim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));

  return s;
}

// Right-side trim
string rtrim(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());

  return s;
}

// Trim
string trim(std::string s) {
  return ltrim(rtrim(s));
}
