# CPU Simulation Assembly Syntax Documentation

## Introduction
This document provides the syntax documentation for the custom assembly language used in the CPU simulation. This simulation uses its own syntax to interact with virtual registers and memory.

## Syntax Overview

The language is semicolon-separated and is case insensitive. Currently, it currently supports basic operations such as setting values and moving data between registers and memory addresses. More will be added in the future.

### Registers

The simulation supports two registers:

- `A`
- `D`

### Memory Addresses

Memory addresses can be represented by either a number or a hexadecimal values starting with `0x`.

### Data Representation

- Register values are 2 byte wide (16 bit).
- Memory (RAM) values are 1 byte wide (8 bit).
- Data can be represented in hexadecimal, decimal, or as a single-byte character in a single quote (e.g., `'A'`, `'\n'`, `'\t'`).

### Commands

#### SET

The `SET` command is used to assign values to registers or memory addresses.

Syntax:
```
SET TARGET DATA;
```

- `target`: Can be either of the registers (`A`, `D`) or a memory address (e.g., `0x2F`).
- `data`: The value to set. For memory addresses, it's a 1-byte value. For registers, it's a 2-byte value. Data can be in hexadecimal, decimal, or character format.

Example:
```
SET A 0xFF;     // Sets register A with the value 0xFF
SET 0x23 'A';   // Sets memory address 0x23 with the ASCII value of 'A'
```

#### MOV

The `MOV` command copies data from the source to the destination without clearing the source.

Syntax:
```
MOV DEST SOURCE;
```

- `dest`: The destination, which can be a register (`A`, `D`) or a memory address.
- `source`: The source, which can follow the same format as `dest`. The value at `source` is copied to `dest`.

Example:
```
MOV D A;        // Copies the value from register A to register D
MOV 0x30 0x23;  // Copies the value from memory address 0x23 to 0x30
```