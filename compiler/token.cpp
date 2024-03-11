#include "token.h"
#include "set.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
using namespace boost::algorithm;

std::vector<std::string> token::string::split(std::string s, std::string sep) {
    std::vector<std::string> data;
    size_t index = 0;
    while (1) {
        size_t length_until_end = s.length() - index;
        size_t pos = s.substr(index, length_until_end).find(sep);
        std::string token = s.substr(index, pos == -1 ? length_until_end : pos);
        if (token != sep) {
            data.push_back(token);
        }

        index += pos + 1;
        if (pos == -1) break;
    }

    return data;
}

std::string token::string::get_first_keyword(std::string input, std::vector<std::string> sep) {
    size_t first_sep_pos = std::string::npos;
    for (std::string &s: sep) {
        size_t pos = input.find(s);
        first_sep_pos = std::min(first_sep_pos, pos);
    }

    return input.substr(0, first_sep_pos);
}

std::string token::string::get_first_keyword(std::string input, std::string sep) {
    std::vector<std::string> sep_copy {sep};
    return get_first_keyword(input, sep_copy);
}

size_t token::string::find_whitespace(std::string input) {
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        std::array<char, 6>::const_iterator it = std::find(whitespaces.begin(), whitespaces.end(), c);

        if (it != whitespaces.end()) return i;
    }

    return std::string::npos;
}

size_t token::string::find_not_whitespace(std::string input) {
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        std::array<char, 6>::const_iterator it = std::find(whitespaces.begin(), whitespaces.end(), c);

        if (it == whitespaces.end()) return i;
    }

    return std::string::npos;
}

std::vector<char> token::conversion::short_to_char(std::vector<short> &source) {
    std::vector<char> output;
    for (short &d: source) {
        char start = (d >> 8) & 255;
        char end = d & 255;

        output.push_back(start);
        output.push_back(end);
    }

    return output;
}

short token::conversion::parse16(std::string input, std::vector<std::string> sep, size_t *end) {
    short output = 0;
    input = input.substr(token::string::find_not_whitespace(input));

    size_t hex_pos = input.find("0x");
    size_t char_pos = input.find("'");

    if (hex_pos == 0) {
        output = parse_hex(input, sep, end);
    } else if (char_pos == 0) {
        output = parse_char(input, sep, end);
    } else {
        output = parse_num(input, sep, end);
    }

    return output;
}

char token::conversion::parse8(std::string input, std::vector<std::string> sep, size_t *end) {
    unsigned short o = parse16(input, sep, end);
    if (o > 255) {
        auto err_message = boost::str(boost::format("At line %2%: input %1% (%3%), is more than the allowed of 8B (255)") % input % "%1%" % o);
        throw err_message;
    }

    return (char)o;
}

short token::conversion::parse_hex(std::string input, std::vector<std::string> sep, size_t *end) {
    size_t after_num_ends = std::min(std::string::npos, token::string::find_whitespace(input));
    for (std::string &s: sep) {
        after_num_ends = std::min(after_num_ends, input.find(s));
    }
    auto err_message = boost::str(boost::format("At line %2%: Invalid hex number %1%") % input.c_str() % "%1%");

    if (after_num_ends < 3 || input.size() < 3) {
        throw err_message;
    }

    if (after_num_ends == std::string::npos) {
        *end = after_num_ends;
    } else {
        std::string input_after_num = input.substr(after_num_ends);
        size_t char_pos = token::string::find_not_whitespace(input_after_num);
        size_t sep_pos = std::string::npos;

        for (std::string &s: sep) {
            sep_pos = std::min(sep_pos, input_after_num.find(s));
        }

        if (char_pos < sep_pos) {
            throw err_message;
        }

        if (char_pos == std::string::npos) {
            *end = char_pos;
        } else {
            *end = after_num_ends + char_pos;
        }
    }


    // we don't want an invalid hex
    for (int i = 2; i < (after_num_ends == std::string::npos ? input.size() : after_num_ends); i++) {
        int ishex = isxdigit(input[i]);
        if (!ishex) {
            throw err_message;
        }
    }

    short output = 0;
    try {
        output = (short)stoul(input, nullptr, 16);
    } catch (...) {
        throw err_message;
    }

    return output;
}

short token::conversion::parse_char(std::string input, std::vector<std::string> sep, size_t *end) {
    input = input.substr(1);
    char output = 0;
    bool added = false;
    bool ignore = false;
    bool finish = false;

    auto err_message = boost::str(boost::format("At line %2%: Invalid char format %1%") % ("'" + input) % "%1%");


    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        // end of string
        if (c == '\'' && !ignore) {
            if (finish) {
                throw err_message;
            } else {
                finish = true;
                continue;
            }
        }

        if (finish) {
            std::string current = input.substr(i);
            
            // if there are any characters (excluding whitespaces) that comes before sep,
            // that is a syntax error
            size_t char_pos = token::string::find_not_whitespace(current);
            size_t sep_pos = std::string::npos;

            for (std::string &s: sep) {
                sep_pos = std::min(sep_pos, current.find(s));
            }

            if (char_pos < sep_pos) {
                throw err_message;
            }

            *end = i + 1 + char_pos;
            return output;
        }

        switch (c) {
            case '\\': {
                if (ignore) {
                    output = '\\';
                    ignore = false;
                    added = true;
                } else {
                    ignore = true;
                }
                break;
            }
            case '\'': {
                output = '\'';
                ignore = false;
                added = true;
            }
            default: {
                if (added) {
                    throw err_message;
                }
                if (ignore) {
                    output = parse_esc(c);
                    ignore = false;
                    added = true;
                } else {
                    output = c;
                    added = true;
                }
            }
        }
    }

    *end = std::string::npos;
    return output;
}

short token::conversion::parse_num(std::string input, std::vector<std::string> sep, size_t *end) {
    size_t after_num_ends = std::min(std::string::npos, token::string::find_whitespace(input));
    for (std::string &s: sep) {
        after_num_ends = std::min(after_num_ends, input.find(s));
    }
    auto err_message = boost::str(boost::format("At line %2%: Invalid number %1%") % input % "%1%");

    if (after_num_ends < 2) {
        throw err_message;
    }

    if (after_num_ends == std::string::npos) {
        *end = after_num_ends;
    } else {
        std::string input_after_num = input.substr(after_num_ends);
        size_t char_pos = token::string::find_not_whitespace(input_after_num);
        size_t sep_pos = std::string::npos;

        for (std::string &s: sep) {
            sep_pos = std::min(sep_pos, input_after_num.find(s));
        }

        if (char_pos < sep_pos) {
            throw err_message;
        }

        if (char_pos == std::string::npos) {
            *end = char_pos;
        } else {
            *end = after_num_ends + char_pos;
        }
    }

    short output = 0;
    size_t sep_index = std::string::npos;
    for (std::string &s: sep) {
        sep_index = std::min(sep_index, input.find(s));
    }

    if (sep_index != std::string::npos) {
        input = input.substr(0, sep_index);
    }


    // we don't want an invalid number
    for (int i = 0; i < (after_num_ends == std::string::npos ? input.size() : after_num_ends); i++) {
        int isd = isdigit(input[i]);
        if (!isd) {
            throw err_message;
        }
    }

    try {
        output = (short)stoul(input);
    } catch (...) {
        throw err_message;
    }

    return output;
}

char token::conversion::parse_esc(char input) {
    switch(input) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '"':
            return '"';
        default:
            auto err_message = boost::str(boost::format("At line %2%: Invalid escape sequence %1%") % input % "%1%");
            throw err_message;
            return 0;
    }
}