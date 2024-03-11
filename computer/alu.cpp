#include "alu.h"

void LU::set(bool op0, bool op1, unsigned short X, unsigned short Y) {
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
unsigned short LU::output() {
	return result;
}

void AU::set(bool op0, bool op1, unsigned short X, unsigned short Y) {
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
unsigned short AU::output() {
	return result;
}

void ALU::set(bool u, bool op0, bool op1, bool sw, bool zx, unsigned short X, unsigned short Y) {
	unsigned short A, B;
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
unsigned short ALU::output() {
	return result;
}

void Condition::set(bool lt, bool eq, bool gt, unsigned short X) {
	result = false;
	if (lt) result = result || (X < 0);
	if (eq) result = result || (X == 0);
	if (gt) result = result || (X > 0);
}
bool Condition::output() {
	return result;
}