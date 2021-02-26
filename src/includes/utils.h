#pragma once

#include <string>

std::string askName();
bool inRange(unsigned low, unsigned high, unsigned input);
bool fileExists(std::string name);
void writeFile(std::string file, std::string content);
void readFile(std::string file);
