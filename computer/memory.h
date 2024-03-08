#ifndef MEMORY_H
#define MEMORY_H
#include <vector>

class RAM {
    public:
        RAM(unsigned int size);
        void set(unsigned short ad, bool st, bool cl, unsigned char d);
        unsigned char output();
    private:
        std::vector<unsigned char> memory;
        unsigned char outval;
};

class Register16 {
    public:
        Register16();
        void set(bool st, bool cl, unsigned short d, unsigned char pos = 0, bool clear = 1);
        unsigned short output(int shift_right = 0);
    private:
        unsigned short reg;
        unsigned short outval;
};

class Counter {
    public:
        Counter();
        void set(bool st, bool cl, unsigned short d);
        unsigned short output();
    private:
        Register16 reg;

};

#endif