#ifndef TOKEN_H
#define TOKEN_H
#include <vector>
#include <array>
#include <string>
#include <stdint.h>

namespace token {
    namespace constants {
        constexpr uint16_t SRC_RAM = 1 << 12 | 1 << 10 | 1 << 7;
        constexpr uint16_t SRC_A = 1 << 10 | 1 << 7;
        constexpr uint16_t SRC_D = 1 << 10 | 1 << 7 | 1 << 6;
        constexpr uint16_t DEST_RAM = 1 << 3;
        constexpr uint16_t DEST_A = 1 << 5;
        constexpr uint16_t DEST_D = 1 << 4;
        constexpr uint16_t DEST_PRT = 1 << 11;
    }

    namespace string {
        constexpr std::array<char, 6> whitespaces{ ' ', '\t', '\n', '\r', '\f', '\v' };
        extern std::vector<std::string> sep;
        std::vector<std::string> split(std::string s, std::string sep);
        std::string get_first_keyword(std::string input, std::vector<std::string> sep);
        std::string get_first_keyword(std::string input, std::string sep);
        size_t find_whitespace(std::string input);
        size_t find_not_whitespace(std::string input);
        size_t find_next_syntax(std::string input, std::vector<std::string> sep);
        int count_occurences(std::string& str, std::string& what);
    }

    namespace conversion {
        std::vector<uint8_t> short_to_char(std::vector<uint16_t>& source);
        uint16_t parse16(std::string input, std::vector<std::string> sep, size_t* end);
        uint8_t parse8(std::string input, std::vector<std::string> sep, size_t* end);
        uint16_t parse_hex(std::string input, std::vector<std::string> sep, size_t* end);
        uint16_t parse_char(std::string input, std::vector<std::string> sep, size_t* end);
        uint16_t parse_num(std::string input, std::vector<std::string> sep, size_t* end);
        char parse_esc(char input);
    }
}

#endif