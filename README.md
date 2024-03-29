# 16-Bit CPU Simulation

# Overview
This project simulates a 16-bit CPU, by using both virtual registers and memory to mimic a real CPU. It is divided into two components: a compiler, and a simulated computer.

## Compiler

The compiler transforms the custom assembly language script into a custom binary executable, which contains the CPU instructions.

CLI Syntax:
```
compiler --input {INPUT_FILE} --output {OUTPUT_FILE}
```

## Computer

The computer simulation executes the binary executable, which contains the CPU instructions generated by the compiler.

CLI Syntax:
```
computer {EXECUTABLE_FILE}
```

# CPU Simulation Assembly Syntax

## Syntax Overview

The language is semicolon-separated and is case insensitive. Currently, it currently supports basic operations such as setting values and moving data between registers and memory addresses. More will be added in the future.

## Registers

The simulation supports three registers:

| Register | Read/Write Capabilities | Notes |
|----------|-------------------------|-------|
| A        | Read and Write          | -     |
| D        | Read and Write          | -     |
| PRT      | Write Only              | Can only be used as a destination (`DEST`) in instructions. |

## Memory Addresses

Memory addresses can be represented by either a number or a hexadecimal values starting with `0x`.

## Data Representation

- Register values are 2 byte wide (16 bit).
- Memory (RAM) values are 1 byte wide (8 bit).
- Data can be represented in hexadecimal, decimal, or as a single-byte character in a single quote (e.g., `'A'`, `'\n'`, `'\t'`).

## Commands

### SET

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

### MOV

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