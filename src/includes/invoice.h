#pragma once

#include <string>
#include <vector>

std::string currentDateTime();
void generateInvoice(std::map<std::string, double> &details);
