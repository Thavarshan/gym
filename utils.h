#pragma once

/**
 * @brief Deterine if the given input is within acceptable range.
 *
 * @param low
 * @param high
 * @param input
 * @return true
 * @return false
 */
bool isWithinRange(unsigned low, unsigned high, unsigned input);

/**
 * @brief Determine if the given file exists.
 *
 * @param name
 * @return true
 * @return false
 */
bool fileExists(const char *name);

/**
 * @brief Read content of ".txt" file and display it for the user.
 *
 * @param file
 */
void readFile(const char *file);
