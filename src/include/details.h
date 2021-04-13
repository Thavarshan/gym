#pragma once

#include <cstdio>
#include <map>
#include <string>

/**
 * @brief Package details.
 */
struct Package
{
    std::string name;
    std::string price;
};

extern std::map<std::string, struct Package> registry;
extern std::map<std::string, std::string> users;

std::string authUserLookup(std::string email);
std::string details(int index);
const bool isPackage(std::string id);
void getDetails(int choice);
double packagePriceLookup(std::string id);
std::string packageNameLookup(std::string id);
