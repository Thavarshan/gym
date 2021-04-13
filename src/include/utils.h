#pragma once

#include <string>
#include <vector>

std::vector<std::string> explode(const std::string &text, const char &delimeter);
std::string askName();
const bool inList(std::string element, std::vector<std::string> &list);
const bool inRange(unsigned low, unsigned high, unsigned input);
std::string currentDateTime();
const bool fileExists(std::string name);
void writeFile(std::string file, std::string content);
std::string readFile(std::string file);
