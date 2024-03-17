#ifndef SHIFT_H
#define SHIFT_H
#include <vector>
#include <string>
#include <stdint.h>

namespace shift {
    std::vector<char> compile(std::string input, int current_line);
    std::vector<uint16_t> gen_instructions(bool lshift, bool from_to_ram, uint8_t amount, uint16_t dest_reg_instruction, uint16_t src_reg_instruction, uint16_t address);
}

#endif // !SHIFT_H
