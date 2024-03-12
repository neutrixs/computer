#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "compile.h"
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Usage:");
    desc.add_options()
        ("help", "help message")
        ("input", po::value<std::string>(), "input file")
        ("output", po::value<std::string>(), "output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.count("input") || !vm.count("output")) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::string input_path = boost::filesystem::absolute(vm["input"].as<std::string>()).string();
    std::string output_path = boost::filesystem::absolute(vm["output"].as<std::string>()).string();

    std::string input = read(input_path);
    std::vector<char> output = compile(input);

    std::ofstream outfile(output_path, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(output.data(), output.size());
        outfile.close();
    }
    else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }

    return 0;
}