#include "token.h"
#include "set.h"
#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/case_conv.hpp>
using namespace std;
using namespace boost::algorithm;

vector<string> split(string s, string sep) {
    vector<string> data;
    int index = 0;
    while (1) {
        int length_until_end = s.length() - index;
        int pos = s.substr(index, length_until_end).find(sep);
        string token = s.substr(index, pos == -1 ? length_until_end : pos);
        if (token != sep) {
            data.push_back(token);
        }

        index += pos + 1;
        if (pos == -1) break;
    }

    return data;
}

string get_first_keyword(string input) {
    size_t spacepos = input.find(' ');
    size_t tabpos = input.find('\t');
    size_t eqpos = input.find('=');

    size_t first = min(spacepos, tabpos);
    first = min(first, eqpos);

    return input.substr(0, first);
}

// returns position of the first non-whitespace char
// if it's just whitespace then npos
size_t skip_whitespace_until_char(string input) {
    const vector<char> whitespaces {' ', '\t', '\n', '\r'};
    for (int i = 0; i < input.size(); i++) {
        vector<char>::const_iterator it = find(whitespaces.begin(), whitespaces.end(), input[i]);
        if (it == whitespaces.end()) return i;
    }
    return string::npos;
}

size_t skip_char_until_whitespace(string input) {
    const vector<char> whitespaces {' ', '\t', '\n', '\r'};
    for (int i = 0; i < input.size(); i++) {
        vector<char>::const_iterator it = find(whitespaces.begin(), whitespaces.end(), input[i]);
        if (it != whitespaces.end()) return i;
    }
    return string::npos;
}

void append_short_to_char(std::vector<char> &dest, std::vector<short> source) {
    for(short &d: source) {
        char start = d >> 8;
        char end = d & 255;

        dest.push_back(start);
        dest.push_back(end);
    }
}

short parse_value(string input, int current_line) {
    short output = 0;
    size_t hex_pos = input.find("0x");
    size_t char_pos = input.find("'");

    if (!hex_pos) {
        size_t next_ws = skip_char_until_whitespace(input);
        if (next_ws != string::npos) {
            string next = input.substr(next_ws);
            size_t next_char = skip_whitespace_until_char(next);

            if (next_char != string::npos) {
                printf("At line %d: Invalid syntax \"%s\"\n", current_line, next.substr(next_char).c_str());
                exit(1);
            }
        }

        output = stoul(input, nullptr, 16);
    } else if(!char_pos) {
        string cut = input.substr(1);
        bool ignore_next = false;
        bool char_finish = false;
        bool closed = false;

        for (int i = 0; i < cut.size(); i++) {
            char c = cut[i];
            switch(c) {
                case '\\':
                    if (ignore_next) {
                        char_finish = true;
                        output = '\\';
                        ignore_next = false;
                    } else {
                        ignore_next = true;
                    }
                    break;
                case '\'':
                    if (ignore_next) {
                        char_finish = true;
                        output = '\'';
                        ignore_next = false;
                    } else {
                        char_finish = true;
                        closed = true;
                    }
                    break;
                default:
                    size_t next_char = skip_whitespace_until_char(cut.substr(i));
                    // if the syntax is closed already, if there are any more characters (except whitespaces), return err
                    if (next_char != string::npos && closed) {
                        printf("At line %d: Invalid syntax \"%s\"\n", current_line, cut.substr(i + next_char).c_str());
                        exit(1);
                    }

                    if (ignore_next) {
                        char_finish = true;
                        output = parse_escape(c);
                        ignore_next = false;
                    } else {
                        char_finish = true;
                        output = c;
                    }
                    break;
                
            }
        }
    } else {
        try {
            output = stoi(input.c_str());
        } catch (...) {
            printf("At line %d: Invalid syntax \"%s\"\n", current_line, input.c_str());
            exit(1);
        }
    }

    return output;
}

char parse_escape(char input, int current_line) {
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
            printf("At line %d: Invalid escape sequence \\%c\n", current_line, input);
            exit(1);
            return 0;
    }
}