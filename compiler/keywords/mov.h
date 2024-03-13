#ifndef MOV_H
#define MOV_H
#include <vector>
#include <string>
#include <stdint.h>

namespace mov {
    constexpr uint16_t SRC_RAM = 1 << 12 | 1 << 10 | 1 << 7;
    constexpr uint16_t SRC_A = 1 << 10 | 1 << 7;
    constexpr uint16_t SRC_D = 1 << 10 | 1 << 7 | 1 << 6;
    constexpr uint16_t DEST_RAM = 1 << 3;
    constexpr uint16_t DEST_A = 1 << 5;
    constexpr uint16_t DEST_D = 1 << 4;
    constexpr uint16_t DEST_PRT = 1 << 11;
    std::vector<char> compile(std::string input, int current_line);
    std::vector<uint16_t> gen_instructions(uint16_t dest_reg_instruction, uint16_t dest_address, uint16_t src_reg_instruction, uint16_t src_address);
}

#endif // !MOV_H
