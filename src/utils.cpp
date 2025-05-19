//
// Created by Ian Parker on 23/10/2024.
//

#include "utils.h"

using namespace std;

vector<string> splitString(string line, char splitChar)
{
    vector<string> parts;

    while (!line.empty())
    {
        size_t pos = line.find(splitChar);
        if (pos == string::npos)
        {
            pos = line.length();
            if (pos == 0)
            {
                break;
            }
        }
        if (pos >= 1)
        {
            string part = line.substr(0, pos);
            trim(part);
            parts.push_back(part);
        }
        if (pos == line.length())
        {
            break;
        }
        line = line.substr(pos + 1);
    }

    return parts;
}

vector<vector<string>> readTextFile(string filename, bool split)
{
    vector<vector<string>> result;
    FILE* fd = fopen(filename.c_str(), "r");
    if (fd == nullptr)
    {
        printf("readTextFile: Failed to open file %s\n", filename.c_str());
        return result;
    }

    char lineBuffer[2048];
    while (fgets(lineBuffer, 2048, fd) != nullptr)
    {
        int len = strlen(lineBuffer);
        while (len > 0 && (lineBuffer[len-1] == '\r' || lineBuffer[len-1] == '\n'))
        {
            lineBuffer[len - 1] = 0;
            len--;
        }
        printf("Line: %s\n", lineBuffer);
        if (split)
        {
            result.push_back(splitString(lineBuffer, ' '));
        }
        else
        {
            result.push_back({lineBuffer});
        }
    }
    fclose(fd);
    return result;
}
