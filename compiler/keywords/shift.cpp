#include <ctype.h>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include "shift.h"
#include "set.h"
#include "mov.h"
#include "../token.h"

#define i(a) instructions.push_back(a)

std::vector<char> shift::compile(std::string input, int current_line) {
    std::vector<char> instructions;
    bool lshift;
    auto first_keyword = token::string::get_first_keyword(input, token::string::sep);
    boost::algorithm::to_lower(first_keyword);

    if (first_keyword == "lshift") lshift = true;
    else lshift = false;

    size_t next = token::string::find_next_syntax(input, token::string::sep);
    auto inputc = input.substr(next);

    auto from_to_reg_o = token::string::get_first_keyword(inputc, token::string::sep);
    auto from_to_reg = from_to_reg_o;
    boost::algorithm::to_lower(from_to_reg);

    bool from_to_ram = false;
    uint8_t amount = 0;
    uint16_t from_to_address = 0;
    uint16_t dest_reg_instruction = 0;
    uint16_t src_reg_instruction = 0;

    if (isdigit(inputc[0])) {
        from_to_ram = true;
        dest_reg_instruction = token::constants::DEST_RAM;
        src_reg_instruction = token::constants::SRC_RAM;

        try {
            from_to_address = token::conversion::parse16(inputc, token::string::sep, &next);
        }
        catch (std::string& e) {
            throw boost::str(boost::format(e) % current_line);
        }
    }
    else if (from_to_reg == "a") {
        dest_reg_instruction = token::constants::DEST_A;
        src_reg_instruction = token::constants::SRC_A;
    }
    else if (from_to_reg == "d") {
        dest_reg_instruction = token::constants::DEST_D;
        src_reg_instruction = token::constants::SRC_D;
    }
    else {
        throw boost::str(boost::format("At line %1%: Invalid register \"%2%\"") % current_line % from_to_reg_o);
    }

    if (!from_to_ram) {
        next = token::string::find_next_syntax(inputc, token::string::sep);
    }

    auto invalid_syntax = boost::str(boost::format("At line %1%: Invalid syntax \"%2%\"") % current_line % input);

    if (next == std::string::npos) throw invalid_syntax;

    inputc = inputc.substr(next);

    try {
        amount = token::conversion::parse16(inputc, token::string::sep, &next);
    }
    catch (std::string& e) {
        throw boost::str(boost::format(e) % current_line);
    }

    if (next != std::string::npos) throw invalid_syntax;

    auto raw_instructions = token::conversion::short_to_char(gen_instructions(lshift, from_to_ram, amount, dest_reg_instruction, src_reg_instruction, from_to_address));
    instructions.insert(instructions.end(), raw_instructions.begin(), raw_instructions.end());

    return instructions;
}

std::vector<uint16_t> shift::gen_instructions(bool lshift, bool from_to_ram, uint8_t amount, uint16_t dest_reg_instruction, uint16_t src_reg_instruction, uint16_t address) {
    std::vector<uint16_t> instructions;
    bool using_a = dest_reg_instruction == token::constants::DEST_A;
    bool using_d = dest_reg_instruction == token::constants::DEST_D;

    // move A to 0x4 and 0x5
    // copy A to b
    i(1 << 15 | 1 << 13 | 1 << 10 | 1 << 7);
    // move bit 0-7 of b to 0x1
    i(0x5); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
    // right shift B by 8, output to b
    i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 8);
    // move bit 0-7 of b to 0x0
    i(0x4); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);

    // backup D to 0x6 and 0x7
    if (!using_d) {
        // copy D to b
        i(1 << 15 | 1 << 13 | 1 << 10 | 1 << 7 | 1 << 6);
        // move bit 0-7 of b to 0x3
        i(0x7); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
        // right shift B by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 8);
        // move bit 0-7 of b to 0x2
        i(0x6); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
    }

    // restore 0x4 and 0x5 to a
    if (using_a) {
        // move 0x0 to b
        i(0x4);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
        // left shift b by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
        // do b + rA at 0x1, output to a
        i(0x5); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 5);
    }

    // move source to d
    auto ins = mov::gen_instructions(token::constants::DEST_D, 0, src_reg_instruction, address);
    instructions.insert(instructions.end(), ins.begin(), ins.end());

    // insert amount to a
    i(static_cast<uint16_t>(amount));

    // perform bitshift operation, output to d
    i(1 << 15 | (lshift ? 1 << 9 : 0) | 1 << 8 | 1 << 4);

    // move D to dest
    ins = mov::gen_instructions(dest_reg_instruction, address, token::constants::SRC_D, 0);
    instructions.insert(instructions.end(), ins.begin(), ins.end());

    if (using_a) {
        // move A to 0x4 and 0x5
        // copy A to b
        i(1 << 15 | 1 << 13 | 1 << 10 | 1 << 7);
        // move bit 0-7 of b to 0x5
        i(0x5); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
        // right shift B by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 8);
        // move bit 0-7 of b to 0x4
        i(0x4); i(1 << 15 | 1 << 14 | 1 << 10 | 1 << 7 | 1 << 6 | 1 << 3);
    }

    // restore 0x6 and 0x7 to d
    if (!using_d) {
        // move 0x6 to b
        i(0x6);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
        // left shift b by 8, output to b
        i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
        // do b + rA at 0x7, output to d
        i(0x7); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 4);
    }

    // restore 0x4 and 0x5 to a
    // move 0x4 to b
    i(0x4);  i(1 << 15 | 1 << 13 | 1 << 12 | 1 << 10 | 1 << 7);
    // left shift b by 8, output to b
    i(0x8); i(1 << 15 | 1 << 14 | 1 << 13 | 1 << 9 | 1 << 8);
    // do b + rA at 0x1, output to a
    i(0x5); i(1 << 15 | 1 << 14 | 1 << 12 | 1 << 10 | 1 << 5);

    return instructions;
}