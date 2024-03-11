#ifndef LOADER_H
#define LOADER_H
#include <string>
#include <vector>

std::string read(std::string path);
std::vector<unsigned short> strtovec16(std::string input);
std::vector<unsigned short> parse(std::string input, std::string sep);

#endif