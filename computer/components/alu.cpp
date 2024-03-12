#include "alu.h"

void LU::set(bool op0, bool op1, uint16_t X, uint16_t Y) {
    switch (op0) {
    case 0:
        if (op1) result = X & Y;
        else result = ~(X & Y);
        break;
    case 1:
        if (op1) result = X << Y;
        else result = X >> Y;
        break;
    }
}
uint16_t LU::output() {
    return result;
}

void AU::set(bool op0, bool op1, uint16_t X, uint16_t Y) {
    switch (op0) {
    case 0:
        if (op1) result = X + 1;
        else result = X + Y;
        break;
    case 1:
        if (op1) result = X - 1;
        else result = X - Y;
    }
}
uint16_t AU::output() {
    return result;
}

void ALU::set(bool u, bool op0, bool op1, bool sw, bool zx, uint16_t X, uint16_t Y) {
    uint16_t A, B;
    if (sw) A = Y, B = X;
    else A = X, B = Y;
    if (zx) A = 0;

    if (u) {
        au.set(op0, op1, A, B);
        result = au.output();
    }
    else {
        lu.set(op0, op1, A, B);
        result = lu.output();
    }
}
uint16_t ALU::output() {
    return result;
}

void Condition::set(bool lt, bool eq, bool gt, uint16_t X) {
    result = false;
    if (lt) result = result || (X < 0);
    if (eq) result = result || (X == 0);
    if (gt) result = result || (X > 0);
}
bool Condition::output() {
    return result;
}