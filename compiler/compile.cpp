#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <fstream>
#include <locale>
#include "token.h"
#include "compile.h"
#include "keywords/set.h"
#include "keywords/mov.h"

//0x0 - 0x03: reserved address for 'backup'
std::vector<char> compile(std::string input) {
    std::vector<char> output;
    std::vector<std::string> statements = token::string::split(input, ";");
    int current_line = 0;
    for (int i = 0; i < statements.size(); i++) {
        std::string s = statements[i];
        if (s == "") continue;

        int index = 0;

        size_t skip_ws = token::string::find_not_whitespace(s);
        size_t substr_i = skip_ws == std::string::npos ? 0 : skip_ws;
        s = s.substr(substr_i);

        std::string low = s;
        boost::algorithm::to_lower(low);

        std::vector<std::string> sep{ " ", "\t", "\n", "\r", "\f", "\v" };
        std::string first_keyword = token::string::get_first_keyword(low, sep);
        if (first_keyword == "set") {
            try {
                std::vector<char> instruction = set::compile(s, current_line + 1);
                for (char& byte : instruction) {
                    output.push_back(byte);
                }
            }
            catch (std::string e) {
                std::cerr << e << std::endl;
                std::exit(1);
            }
        }
        else if (first_keyword == "mov") {
            try {
                std::vector<char> instruction = mov::compile(s, current_line + 1);
                for (char& byte : instruction) {
                    output.push_back(byte);
                }
            }
            catch (std::string& e) {
                std::cerr << e << std::endl;
                std::exit(1);
            }
        }

        current_line += token::string::count_occurences(statements[i], std::string("\n"));
    }

    return output;
}

std::string read(std::string path) {
    std::ifstream infile(path, std::ios::binary);

    if (infile.is_open()) {
        infile.seekg(0, std::ios::end);
        std::streampos size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        infile.read(buffer.data(), size);
        infile.close();

        std::string fileContents(buffer.data(), size);
        return fileContents;
    }
    else {
        std::cerr << "Unable to open file for reading" << std::endl;
        exit(1);
        return "";
    }
}