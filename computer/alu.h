#ifndef ALU_H
#define ALU_H

class LU {
public:
	/**
	 * 0,0: X NAND Y
	 * 0,1: X AND Y
	 * 1,0: X >> Y
	 * 1,1: X << Y
	*/
	void set(bool op0, bool op1, unsigned short X, unsigned short Y);
	unsigned short output();
private:
	unsigned short result;
};

class AU {
public:
	/**
	 * 0,0: X + Y
	 * 0,1: X + 1
	 * 1,0: X - Y
	 * 1,1: X - 1
	*/
	void set(bool op0, bool op1, unsigned short X, unsigned short Y);
	unsigned short output();
private:
	unsigned short result;
};

class ALU {
public:
	/**
	 * u: AU
	 * sw: swap X and Y
	 * zx: makes X zero (if used with sw then Y is zero)
	*/
	void set(bool u, bool op0, bool op1, bool sw, bool zx, unsigned short X, unsigned short Y);
	unsigned short output();
private:
	unsigned short result;
	LU lu;
	AU au;
};

class Condition {
public:
	void set(bool lt, bool eq, bool gt, unsigned short X);
	bool output();
private:
	bool result;
};

#endif