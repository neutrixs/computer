#include "memory.h"

RAM::RAM(unsigned int size) {
    memory = std::vector<unsigned char>(size);
    outval = 0;
}
void RAM::set(unsigned short ad, bool st, bool cl, unsigned char d) {
    if (cl) {
        outval = memory[ad];
    } else if (st) {
        memory[ad] = d;
    }
}
unsigned char RAM::output() {
    return outval;
}

Register16::Register16() {
    reg = 0;
    outval = 0;
}
void Register16::set(bool st, bool cl, unsigned short d, unsigned char pos, bool clear) {
    if (cl) {
        outval = reg;
    } else if (st) {
        if (clear) reg = 0;
        reg |= d << pos;
    }
}
unsigned short Register16::output(int shift_right) {
    return outval >> shift_right;
}

Counter::Counter() {
    reg.set(1,1,0);
}
void Counter::set(bool st, bool cl, unsigned short d) {
    unsigned short input;
    if (st) input = d;
    else input = reg.output() + 1;

    reg.set(1, cl, input);
}
unsigned short Counter::output() {
    return reg.output();
}