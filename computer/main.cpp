#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <string>
#include <boost/filesystem.hpp>
#include "loader.h"
#include "components/processor.h"

int main(int argc, char** argv) {
    std::string desc = "Usage: computer path/to/executable";
    if (argc != 2) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::string input(argv[1]);
    std::string input_abs = boost::filesystem::absolute(input).string();

    std::string executable = read(input_abs);
    std::vector<uint16_t> ROM = strtovec16(executable);

    Computer comp(ROM);

    for (int i = 0; i < ROM.size() * 2; i++) {
        comp.pulse();
    }

    return 0;
}