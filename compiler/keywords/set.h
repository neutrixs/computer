#ifndef SET_H
#define SET_H
#include <array>
#include <vector>
#include <string>

namespace set {
    std::vector<char> compile(std::string input, int current_line);
    std::vector<short> gen_instructions(short dest_instruction, short data, short address = 0);
}

#endif