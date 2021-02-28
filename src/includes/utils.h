#pragma once

#include <string>

std::string askName();
const bool inRange(unsigned low, unsigned high, unsigned input);
std::string currentDateTime();
const bool fileExists(std::string name);
void writeFile(std::string file, std::string content);
void readFile(std::string file);
void createAndWriteToFile(std::string file, std::string content);
