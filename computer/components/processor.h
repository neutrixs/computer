#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "memory.h"
#include "alu.h"
#include <vector>
#include <stdint.h>

/** do not actually use the b register. only used for some operations :) data might be lost if you try to do so.
 * put it in ram instead or a and d
 *
 * 0x0 - 0x3: reserved ram addresses:
 *
 * Instructions bit:
 * conditions:
 * 0: gt, 1: eq, 2: lt
 * destinations:
 * 3: ra, 4: d, 5: a
 * ALU:
 * 6: sw, 7: zx, 8: op0, 9: op1, 10: u
 * 11: print
 * 12: Y as rA instead of A
 * 13: b
 * 14: X as B instead of D
 * 15: data instruction
*/

class Memory {
public:
    Memory();
    Register16 A;
    Register16 B;
    Register16 D;
    uint16_t rA;
    void set(bool a, bool b, bool d, bool ra, bool prt, bool cl, uint16_t X);
private:
    RAM ram;
};

class Instruction {
public:
    uint16_t R;
    bool a;
    bool b;
    bool d;
    bool ra;
    bool j;
    bool prt;
    void set(uint16_t I, uint16_t A, uint16_t B, uint16_t D, uint16_t rA);
private:
    Condition cond;
    ALU alu;
};

class ControlUnit {
public:
    uint16_t R;
    bool a;
    bool b;
    bool d;
    bool ra;
    bool j;
    bool prt;
    void set(uint16_t I, uint16_t A, uint16_t B, uint16_t D, uint16_t rA);
private:
    Instruction instruction;
};

class Computer {
public:
    Computer(std::vector<uint16_t> R);
    std::vector<uint16_t> ROM;
    void pulse();
    // private:
    ControlUnit ctrl;
    Memory mem;
    bool cl = 0;
    uint16_t ROM_addr;
};

#endif