#include "processor.h"
#include "memory.h"
#include <iostream>
#include <stdint.h>

Memory::Memory() : ram(65536) {
    rA = 0;
}
void Memory::set(bool a, bool b, bool d, bool ra, bool prt, bool cl, uint16_t X) {
    A.set(a, cl, X);
    B.set(b, cl, X);
    D.set(d, cl, X);
    ram.set(A.output(), ra, cl, (uint8_t)X);
    rA = ram.output();

    // printf("%x %x %x %x\n", A.output(), B.output(), D.output(), rA);

    if (cl && prt) {
        std::cout << (uint8_t)X;
    }
}

void Instruction::set(uint16_t I, uint16_t A, uint16_t B, uint16_t D, uint16_t rA) {
    uint16_t X;
    uint16_t Y;
    if (I & 1 << 14) X = B;
    else X = D;
    if (I & 1 << 12) Y = rA;
    else Y = A;

    alu.set(I & 1 << 10, I & 1 << 8, I & 1 << 9, I & 1 << 6, I & 1 << 7, X, Y);
    R = alu.output();
    a = I & 1 << 5;
    b = I & 1 << 13;
    d = I & 1 << 4;
    ra = I & 1 << 3;
    prt = I & 1 << 11;

    cond.set(I & 1 << 2, I & 1 << 1, I & 1, alu.output());
    j = cond.output();
}

void ControlUnit::set(uint16_t I, uint16_t A, uint16_t B, uint16_t D, uint16_t rA) {
    // printf("0x%x: ", I);
    switch (I & 1 << 15) {
    case 0:
        R = I;
        a = 1;
        b = 0;
        d = 0;
        ra = 0;
        j = 0;
        prt = 0;
        break;
    default:
        instruction.set(I, A, B, D, rA);
        R = instruction.R;
        a = instruction.a;
        b = instruction.b;
        d = instruction.d;
        ra = instruction.ra;
        j = instruction.j;
        prt = instruction.prt;
        break;
    }
}

Computer::Computer(std::vector<uint16_t> R) {
    ROM = R;
    ROM_addr = 0;
    ctrl = ControlUnit();
}
void Computer::pulse() {
    uint16_t ROM_val = ROM.at(ROM_addr);
    ctrl.set(ROM_val, mem.A.output(), mem.B.output(), mem.D.output(), mem.rA);
    mem.set(ctrl.a, ctrl.b, ctrl.d, ctrl.ra, ctrl.prt, cl, ctrl.R);

    if (cl) {
        if (ctrl.j) {
            ROM_addr = mem.A.output();
        }
        else {
            ROM_addr++;
        }
    }

    cl = !cl;
}