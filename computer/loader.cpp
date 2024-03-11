#include "loader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

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

std::vector<unsigned short> strtovec16(std::string input) {
    std::vector<unsigned short> output;
    for (int i = 0; i < input.size(); i++) {
        if (!(i % 2)) {
            output.push_back((input[i] & 255) << 8);
        }
        else {
            output.back() |= (input[i] & 255);
        }
    }

    return output;
}

std::vector<unsigned short> parse(std::string input, std::string sep) {
    std::vector<unsigned short> data;
    size_t index = 0;
    while (1) {
        size_t length_until_end = input.length() - index;
        size_t pos = input.substr(index, length_until_end).find(sep);
        std::string token = input.substr(index, pos == std::string::npos ? length_until_end : pos);
        if (token != sep) {
            unsigned short x = (unsigned short)strtoul(token.c_str(), nullptr, 16);

            data.push_back(x);
        }

        index += pos + 1;
        if (pos == -1) break;
    }

    return data;
}