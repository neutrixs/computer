#include <iostream>
#include <fstream>
#include <boost/algorithm/string/case_conv.hpp>
#include "token.h"
#include "set.h"
using namespace std;

//0x0 0x1: reserved address for 'backup'
vector<char> compile(string input) {
    vector<char> output;
    vector<string> lines = split(input, "\n");
    for (int i = 0; i < lines.size(); i++) {
        string s = lines[i];
        if (s == "") continue;

        int index = 0;
        string low = s;
        boost::algorithm::to_lower(low);

        size_t skip_ws = skip_whitespace_until_char(s);
        int substr_i = skip_ws == string::npos ? 0 : skip_ws;
        low = low.substr(substr_i);

        string first_keyword = get_first_keyword(low);
        switch (str2int(first_keyword.c_str())) {
            case str2int("set"): {
                vector<char> instruction = set::compile(low, i + 1);
                for (char &byte: instruction) {
                    output.push_back(byte);
                }
                break;
            }
        }
    }

    return output;
}

string read(string path) {
    ifstream infile(path, ios::binary);

    if (infile.is_open()) {
        infile.seekg(0, ios::end);
        streampos size = infile.tellg();
        infile.seekg(0, ios::beg);

        vector<char> buffer(size);
        infile.read(buffer.data(), size);
        infile.close();

        string fileContents(buffer.data(), size);
        return fileContents;
    } else {
        cerr << "Unable to open file for reading" << endl;
        exit(1);
        return "";
    }
}

int main(int argc, char* argv[]) {
    string input = read("program.txt");
    vector<char> output = compile(input);

    ofstream outfile("executable", ios::binary);
    if (outfile.is_open()) {
        outfile.write(output.data(), output.size());
        outfile.close();
    } else {
        cerr << "Unable to open file for writing" << endl;
    }

    return 0;
}