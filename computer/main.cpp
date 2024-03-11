#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "loader.h"
#include "components/processor.h"
using namespace std;

int main() {
    string executable = read("executable");
    vector<unsigned short> ROM = strtovec16(executable);

    Computer comp(ROM);

    for (int i = 0; i < ROM.size() * 2; i++) {
        comp.pulse();
    }

    return 0;
}