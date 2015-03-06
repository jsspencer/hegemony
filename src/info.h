/**
 * @file
 */
#pragma once

#include <string>

/**
 * Print a banner containing useful information about the program.
 */
void print_banner(void);

/**
 * Print the contents of an input file to STDOUT.
 *
 * @param[in] inp input filename.
 */
int echo_input(const std::string &inp);

