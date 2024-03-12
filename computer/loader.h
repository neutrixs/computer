#ifndef LOADER_H
#define LOADER_H
#include <string>
#include <vector>
#include <stdint.h>

std::string read(std::string path);
std::vector<uint16_t> strtovec16(std::string input);
std::vector<uint16_t> parse(std::string input, std::string sep);

#endif