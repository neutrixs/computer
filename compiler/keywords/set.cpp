#include <vector>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <ctype.h>
#include "set.h"
#include "../token.h"

std::vector<char> set::compile(std::string input, int current_line) {
    std::vector<std::string> sep{ " ", "\t", "\n", "\r", "\f", "\v" };
    std::vector<char> instructions;
    std::string inputc = input.substr(token::string::find_not_whitespace(input)).substr(3);

    size_t nextcmd_pos = token::string::find_not_whitespace(inputc);
    inputc = inputc.substr(nextcmd_pos);
    size_t end_target_input = token::string::find_whitespace(inputc);
    std::string dest_reg = inputc.substr(0, end_target_input);

    size_t hex_pos = inputc.find("0x");
    size_t char_pos = inputc.find("'");
    bool is_normal_num = isdigit(input[0]);

    bool to_ram = false;
    short dest_instruction = 0;
    short address = 0;
    short data = 0;
    size_t next = 0;

    if (hex_pos == 0 || char_pos == 0 || is_normal_num) {
        to_ram = true;
        dest_instruction = 1 << 3;
        try {
            address = token::conversion::parse16(inputc, sep, &next);
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }
    else if (dest_reg == "a") {
        dest_instruction = 1 << 5;
    }
    else if (dest_reg == "d") {
        dest_instruction = 1 << 4;
    }
    else if (dest_reg == "prt") {
        dest_instruction = 1 << 11;
    }
    else {
        auto err_message = boost::str(boost::format("At line %1%: destination %2% not supported") % current_line % dest_reg);
        throw err_message;
    }

    auto syntax_inv = boost::str(boost::format("At line %1%: Invalid syntax \"%2%\"") % current_line % input);

    if (!to_ram) {
        try {
            size_t ws = token::string::find_whitespace(inputc);
            size_t not_ws = token::string::find_not_whitespace(inputc.substr(ws));
            if (ws == std::string::npos || not_ws == std::string::npos) {
                next = std::string::npos;
            }
            else {
                next = ws + not_ws;
            }
        }
        catch (...) {
            throw syntax_inv;
        }
    }

    try {
        inputc = inputc.substr(next);
    }
    catch (...) {
        throw syntax_inv;
    }

    if (to_ram) {
        try {
            data = token::conversion::parse8(inputc, sep, &next);
            data = data & 255;
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }
    else {
        try {
            data = token::conversion::parse16(inputc, sep, &next);
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }

    if (next != std::string::npos) {
        throw syntax_inv;
    }

    auto raw_instructions = gen_instructions(dest_instruction, data, address);
    for (auto& ins : raw_instructions) {
        instructions.push_back((ins >> 8) & 255);
        instructions.push_back(ins & 255);
    }

    return instructions;
}

std::vector<short> set::gen_instructions(short dest_instruction, short data, short address) {
    std::vector<short> instructions;
    bool is_dest_a = dest_instruction == 1 << 5;
    bool is_dest_ra = dest_instruction == 1 << 3;
#define i(a) instructions.push_back((short)a)

    if (!is_dest_a) {
        // backup A to 0x0 and 0x1
        // move A to b
        i(0xa480);

        // move bit 0-7 of B to 0x1
        i(0x1); i(0xc4c8);

        // move bit 8-15 of B to 0x0
        // shift-right B by 8
        i(0x8); i(0xe100);

        // move bit 0-7 of B to 0x0
        i(0x0); i(0xc4c8);

        // backup done.
    }

    if (is_dest_ra) {
        // backup D to 0x2 and 0x3
        // move D to b
        i(0xa4c0);

        // move bit 0-7 of B to 0x3
        i(0x3); i(0xc4c8);

        // move bit 8-15 of B to 0x2
        // shift-right B by 8
        i(0x8); i(0xe100);

        // move bit 0-7 of B to 0x2
        i(0x2); i(0xc4c8);

        // backup done.
    }

    // we cannot put data where bit 15 is 1.
    // we will copy the data. data 1 will be data from bit 8-15, data 2 will be from bit 0-7 (8 smallest bit)
    short data1 = (data >> 8) & 255;
    short data2 = data & 255;

    // place data 1 to a and move it to b, shift left by 8
    i(data1); i(0xa480);
    i(0x8); i(0xe300);

    // place data 2 to a, add A + B, output to dest, or d if we wanna move to RAM
    i(data2); i(0xc400 | (is_dest_ra ? 1 << 4 : dest_instruction));

    if (is_dest_ra) {
        // put the target address to A
        std::vector<short> addr_instruct = gen_instructions(1 << 5, address);
        instructions.insert(instructions.end(), addr_instruct.begin(), addr_instruct.end());

        // move D to ra
        i(0x84c8);
    }

    if (is_dest_ra) {
        // retrieve 0x2 and 0x3 to d
        // move 0x2 to b, left shift 8 bit
        i(0x2); i(0xb480);
        i(0x8); i(0xe300);

        // move 0x3 to a, do A + B to d
        i(0x3); i(0x94a0);
        i(0xc410);
    }

    if (!is_dest_a) {
        // we can now retrieve 0x0 and 0x1 to A
        // move 0x0 to b, left shift 8 bit
        i(0x0); i(0xb480);
        i(0x8); i(0xe300);

        // move 0x1 to a, do A + B to a
        i(0x1); i(0x94a0);
        i(0xc420);
    }

    return instructions;
}