#ifndef MEMORY_H
#define MEMORY_H
#include <vector>
#include <stdint.h>

class RAM {
public:
    RAM(uint32_t size);
    void set(uint16_t ad, bool st, bool cl, uint8_t d);
    uint8_t output();
private:
    std::vector<uint8_t> memory;
    uint8_t outval;
};

class Register16 {
public:
    Register16();
    void set(bool st, bool cl, uint16_t d, uint8_t pos = 0, bool clear = 1);
    uint16_t output(uint32_t shift_right = 0);
private:
    uint16_t reg;
    uint16_t outval;
};

class Counter {
public:
    Counter();
    void set(bool st, bool cl, uint16_t d);
    uint16_t output();
private:
    Register16 reg;
};

#endif