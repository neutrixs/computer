#include <iostream>
#include <sstream>
#include <vector>
#include "processor.h"
#include "program.h"
using namespace std;

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
    vector<unsigned short> ROM = parse(program, "\n");
    Computer comp(ROM);

    for (int i = 0; i < ROM.size() * 2; i++) {
        comp.pulse();
    }

    return 0;
}