#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <fstream>
#include "token.h"
#include "compile.h"
#include "keywords/set.h"

//0x0 - 0x03: reserved address for 'backup'
std::vector<char> compile(std::string input) {
    std::vector<char> output;
    std::vector<std::string> lines = token::string::split(input, ";");
    for (int i = 0; i < lines.size(); i++) {
        std::string s = lines[i];
        if (s == "") continue;

        int index = 0;
        std::string low = s;
        boost::algorithm::to_lower(low);

        size_t skip_ws = token::string::find_not_whitespace(s);
        size_t substr_i = skip_ws == std::string::npos ? 0 : skip_ws;
        low = low.substr(substr_i);

        std::vector<std::string> sep{ " ", "\t", "\n", "\r", "\f", "\v" };
        std::string first_keyword = token::string::get_first_keyword(low, sep);
        switch (token::conversion::str2int(first_keyword.c_str())) {
        case token::conversion::str2int("set"): {
            try {
                std::vector<char> instruction = set::compile(low, i + 1);
                for (char& byte : instruction) {
                    output.push_back(byte);
                }
            }
            catch (std::string e) {
                std::cout << e << std::endl;
                std::exit(1);
            }
            break;
        }
        }
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