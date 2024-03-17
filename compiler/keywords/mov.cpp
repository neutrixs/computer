#include <vector>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "mov.h"
#include "set.h"
#include "../token.h"

#define i(a) instructions.push_back(a)

std::vector<char> mov::compile(std::string input, int current_line) {
    std::vector<std::string> sep{ " ", "\t", "\n", "\r", "\f", "\v" };
    std::vector<char> instructions;
    auto invalid_syntax = boost::str(boost::format("At line %1%: Invalid syntax \"%2%\"") % current_line % input);

    // remove the first syntax
    auto inputc = input.substr(token::string::find_not_whitespace(input));

    size_t end_syntax_pos = token::string::find_whitespace(inputc);
    if (end_syntax_pos == std::string::npos) throw invalid_syntax;
    inputc = inputc.substr(end_syntax_pos);

    size_t start_syntax_pos = token::string::find_not_whitespace(inputc);
    if (start_syntax_pos == std::string::npos) throw invalid_syntax;
    inputc = inputc.substr(start_syntax_pos);

    end_syntax_pos = token::string::find_whitespace(inputc);
    auto inputc_low = inputc;
    boost::algorithm::to_lower(inputc_low);
    auto dest_reg = inputc_low.substr(0, end_syntax_pos);


    bool to_ram = false;
    uint16_t dest_reg_instruction = 0;
    uint16_t dest_address = 0;
    size_t next_syntax_if_to_ram;

    bool from_ram = false;
    uint16_t src_reg_instruction = 0;
    uint16_t src_address = 0;


    // get destination
    bool dest_isdigit = isdigit(inputc[0]);
    if (dest_isdigit) {
        to_ram = true;
        dest_reg_instruction = DEST_RAM;
        try {
            dest_address = token::conversion::parse16(inputc, sep, &next_syntax_if_to_ram);
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }
    else if (dest_reg == "a") {
        dest_reg_instruction = DEST_A;
    }
    else if (dest_reg == "d") {
        dest_reg_instruction = DEST_D;
    }
    else if (dest_reg == "prt") {
        dest_reg_instruction = DEST_PRT;
    }
    else {
        throw boost::str(boost::format("At line %1%: Invalid dest \"%2%\"") % current_line % dest_reg);
    }

    if (to_ram) {
        if (next_syntax_if_to_ram == std::string::npos) throw invalid_syntax;
        inputc = inputc.substr(next_syntax_if_to_ram);
    }
    else {
        if (end_syntax_pos == std::string::npos) throw invalid_syntax;
        inputc = inputc.substr(end_syntax_pos);

        start_syntax_pos = token::string::find_not_whitespace(inputc);
        if (start_syntax_pos == std::string::npos) throw invalid_syntax;
        inputc = inputc.substr(start_syntax_pos);
    }

    end_syntax_pos = token::string::find_whitespace(inputc);
    inputc_low = inputc;
    boost::algorithm::to_lower(inputc_low);
    auto src_reg = inputc_low.substr(0, end_syntax_pos);

    // get source
    bool src_isdigit = isdigit(inputc[0]);
    if (src_isdigit) {
        from_ram = true;
        src_reg_instruction = SRC_RAM;
        try {
            src_address = token::conversion::parse16(inputc, sep, &next_syntax_if_to_ram);
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }
    else if (src_reg == "a") {
        src_reg_instruction = SRC_A;
    }
    else if (src_reg == "d") {
        src_reg_instruction = SRC_D;
    }
    else {
        throw boost::str(boost::format("At line %1%: Invalid source \"%2%\"") % current_line % src_reg);
    }

    if (from_ram) {
        if (next_syntax_if_to_ram != std::string::npos) throw invalid_syntax;
    }
    else if (end_syntax_pos != std::string::npos) {
        inputc = inputc.substr(end_syntax_pos);
        start_syntax_pos = token::string::find_not_whitespace(inputc);
        if (start_syntax_pos != std::string::npos) throw invalid_syntax;
    }

    auto raw_instructions = gen_instructions(dest_reg_instruction, dest_address, src_reg_instruction, src_address);
    for (auto& each : raw_instructions) {
        instructions.push_back((each >> 8) & 255);
        instructions.push_back(each & 255);
    }

    return instructions;
}

std::vector<uint16_t> mov::gen_instructions(uint16_t dest_reg_instruction, uint16_t dest_address, uint16_t src_reg_instruction, uint16_t src_address) {
    std::vector<uint16_t> instructions;
    bool need_to_backup_a = dest_reg_instruction != DEST_A;
    bool need_to_backup_d = dest_reg_instruction != DEST_D;
    bool using_a = dest_reg_instruction == DEST_A || src_reg_instruction == SRC_A;
    bool using_d = dest_reg_instruction == DEST_D || src_reg_instruction == SRC_D;

    // backup A
    // copy A to b
    i(1 << 15 | 1 << 13 | 1 << 10 | 1 << 7);
    // move bit 0-7 of b to 0x1
    i(0x1); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
    // right shift B by 8, output to b
    i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 8);
    // move bit 0-7 of b to 0x0
    i(0x0); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);

    // backup D
    if (!using_d) {
        // copy D to b
        i(1 << 15 | 1 << 13 | 1 << 10 | 1 << 7 | 1 << 6);
        // move bit 0-7 of b to 0x3
        i(0x3); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
        // right shift B by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 8);
        // move bit 0-7 of b to 0x2
        i(0x2); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
    }

    // restore A
    if (using_a) {
        // move 0x0 to b
        i(0x0);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
        // left shift b by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
        // do b + rA at 0x1, output to a
        i(0x1); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 5);
    }

    // for ease sake, we wanna move the source to D first
    // insert the source address to A
    if (src_reg_instruction == SRC_RAM) {
        auto cmd = boost::str(boost::format("set A %1%") % src_address);
        auto additional = set::compile(cmd, 0);
        for (int c = 0; c < additional.size(); c++) {
            if (c % 2) {
                instructions.back() |= additional[c] & 255;
            }
            else {
                instructions.push_back((additional[c] & 255) << 8);
            }
        }
    }

    // move source to d
    i(1 << 15 | src_reg_instruction | DEST_D);

    // insert dest address to A
    if (dest_reg_instruction == DEST_RAM) {
        auto cmd = boost::str(boost::format("set A %1%") % dest_address);
        auto additional = set::compile(cmd, 0);
        for (int c = 0; c < additional.size(); c++) {
            if (c % 2) {
                instructions.back() |= additional[c] & 255;
            }
            else {
                instructions.push_back((additional[c] & 255) << 8);
            }
        }
    }

    // move d to dest
    i(1 << 15 | SRC_D | dest_reg_instruction);

    // restore 0x2 and 0x3 to d
    if (!using_d) {
        // move 0x2 to b
        i(0x2);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
        // left shift b by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
        // do b + rA at 0x3, output to d
        i(0x3); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 4);
    }

    // restore 0x0 and 0x1 to a
    if (!using_a) {
        // move 0x0 to b
        i(0x0);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
        // left shift b by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
        // do b + rA at 0x1, output to a
        i(0x1); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 5);
    }

    return instructions;
}