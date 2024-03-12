#ifndef SET_H
#define SET_H
#include <array>
#include <vector>
#include <string>
#include <stdint.h>

namespace set {
    std::vector<char> compile(std::string input, int32_t current_line);
    std::vector<uint16_t> gen_instructions(uint16_t dest_instruction, uint16_t data, uint16_t address = 0);
}

#endif