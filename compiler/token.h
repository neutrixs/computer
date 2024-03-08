#ifndef TOKEN_H
#define TOKEN_H
#include <vector>
#include <string>

constexpr unsigned int str2int(const char* str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

std::vector<std::string> split(std::string s, std::string sep);
size_t skip_whitespace_until_char(std::string input);
size_t skip_char_until_whitespace(std::string input);
std::string get_first_keyword(std::string input);
void append_short_to_char(std::vector<char> &dest, std::vector<short> source);
short parse_value(std::string input, int current_line = 0);
char parse_escape(char input, int current_line = 0);

#endif