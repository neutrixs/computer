#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "processor.h"
#include "program.h"
using namespace std;

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

vector<unsigned short> strtovec16(string input) {
    vector<unsigned short> output;
    for (int i = 0; i < input.size(); i++) {
        if (!(i % 2)) {
            output.push_back((input[i] & 255) << 8);
        } else {
            output.back() |= (input[i] & 255);
        }
    }

    return output;
}

vector<unsigned short> parse(string input, string sep) {
    vector<unsigned short> data;
    int index = 0;
    while (1) {
        int length_until_end = input.length() - index;
        int pos = input.substr(index, length_until_end).find(sep);
        string token = input.substr(index, pos == -1 ? length_until_end : pos);
        if (token != sep) {
            unsigned short x = strtoul(token.c_str(), nullptr, 16);

            data.push_back(x);
        }

        index += pos + 1;
        if (pos == -1) break;
    }

    return data;
}

int main() {
    string executable = read("executable");
    vector<unsigned short> ROM = strtovec16(executable);

    Computer comp(ROM);

    for (int i = 0; i < ROM.size() * 2; i++) {
        comp.pulse();
    }

    return 0;
}