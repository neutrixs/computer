#ifndef SET_H
#define SET_H
#include <vector>
#include <string>

namespace set {
    std::vector<char> compile(std::string input, int current_line);
    std::vector<short> gen_instructions(short dest_instruction, short data);
}

#endif