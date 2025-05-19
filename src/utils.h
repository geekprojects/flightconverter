#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

std::vector<std::string> splitString(std::string line, char splitChar);
std::vector<std::vector<std::string>> readTextFile(std::string filename, bool split);

#endif
