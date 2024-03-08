#include <vector>
#include <string>
#include <iostream>
#include "set.h"
#include "token.h"
using namespace std;

namespace set {
    vector<char> compile(string input, int current_line) {
        vector<char> instructions;
        // we know the command is 'set', so extract the first part
        size_t poss = input.find(' ');
        size_t postab = input.find('\t');
        size_t pos = min(poss, postab);

        string copy = input.substr(pos + 1);

        poss = copy.find(' ');
        postab = input.find('\t');
        pos = min(poss, postab);

        string dest = copy.substr(0, pos);
        string data = copy.substr(pos + skip_whitespace_until_char(copy.substr(pos)), string::npos);

        short dest_instruction;
        if (dest == "a") {
            dest_instruction = 1 << 5;
        } else if (dest == "d") {
            dest_instruction = 1 << 4;
        } else {
            printf("At line %d: set command to reg %s not supported.\n", current_line, dest.c_str());
            exit(1);
        }

        if (data.find('"') == 0) {
            printf("At line %d: assigning string to registers are not allowed.\n", current_line);
            exit(1);
        }

        short data_value = parse_value(data, current_line);
        vector<short> new_instruction = gen_instructions(dest_instruction, data_value);
        append_short_to_char(instructions, new_instruction);

        return instructions;
    }

    vector<short> gen_instructions(short dest_instruction, short data) {
        vector<short> instructions;
        bool is_dest_a = dest_instruction == 1 << 5;
        #define i(a) instructions.push_back(a)

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

        // we cannot put data where bit 15 is 1.
        // we will copy the data. data 1 will be data from bit 8-15, data 2 will be from bit 0-7 (8 smallest bit)
        short data1 = (data >> 8) & 255;
        short data2 = data & 255;

        // place data 1 to a and move it to b, shift left by 8
        i(data1); i(0xa480);
        i(0x8); i(0xe300);

        // place data 2 to a, add A + B, output to dest
        i(data2); i(0xc400 | dest_instruction);

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
}