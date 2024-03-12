#include "memory.h"

RAM::RAM(uint32_t size) {
    memory = std::vector<uint8_t>(size);
    outval = 0;
}
void RAM::set(uint16_t ad, bool st, bool cl, uint8_t d) {
    if (cl) {
        outval = memory[ad];
    }
    else if (st) {
        memory[ad] = d;
    }
}
uint8_t RAM::output() {
    return outval;
}

Register16::Register16() {
    reg = 0;
    outval = 0;
}
void Register16::set(bool st, bool cl, uint16_t d, uint8_t pos, bool clear) {
    if (cl) {
        outval = reg;
    }
    else if (st) {
        if (clear) reg = 0;
        reg |= d << pos;
    }
}
uint16_t Register16::output(uint32_t shift_right) {
    return outval >> shift_right;
}

Counter::Counter() {
    reg.set(1, 1, 0);
}
void Counter::set(bool st, bool cl, uint16_t d) {
    uint16_t input;
    if (st) input = d;
    else input = reg.output() + 1;

    reg.set(1, cl, input);
}
uint16_t Counter::output() {
    return reg.output();
}