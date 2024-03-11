#ifndef TOKEN_H
#define TOKEN_H
#include <vector>
#include <array>
#include <string>

namespace token {
	namespace string {
		constexpr std::array<char, 6> whitespaces{ ' ', '\t', '\n', '\r', '\f', '\v' };
		std::vector<std::string> split(std::string s, std::string sep);
		std::string get_first_keyword(std::string input, std::vector<std::string> sep);
		std::string get_first_keyword(std::string input, std::string sep);
		size_t find_whitespace(std::string input);
		size_t find_not_whitespace(std::string input);
	}

	namespace conversion {
		constexpr unsigned int str2int(const char* str, int h = 0) {
			return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
		}
		std::vector<char> short_to_char(std::vector<short>& source);
		short parse16(std::string input, std::vector<std::string> sep, size_t* end);
		char parse8(std::string input, std::vector<std::string> sep, size_t* end);
		short parse_hex(std::string input, std::vector<std::string> sep, size_t* end);
		short parse_char(std::string input, std::vector<std::string> sep, size_t* end);
		short parse_num(std::string input, std::vector<std::string> sep, size_t* end);
		char parse_esc(char input);
	}
}

#endif