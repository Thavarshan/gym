#pragma once

#include <string>
#include <vector>

std::string askName();
const bool inList(std::string element, std::vector<std::string> &list);
const bool inRange(unsigned low, unsigned high, unsigned input);
std::string currentDateTime();
const bool fileExists(std::string name);
void writeFile(std::string file, std::string content);
void readFile(std::string file);
