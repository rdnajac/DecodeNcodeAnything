#include <array>

/**
 * @brief Define prime polynomials for GF^x
 */
constexpr std::array<unsigned int, 32> primes = {
    0,          // PRIM_0
    0,          // PRIM_1
    0,          // PRIM_2
    0xb,        // PRIM_3
    0x13,       // PRIM_4
    0x25,       // PRIM_5
    0x43,       // PRIM_6
    0x83,       // PRIM_7
    0x11d,      // PRIM_8
    0x211,      // PRIM_9
    0x409,      // PRIM_10
    0x805,      // PRIM_11
    0x1053,     // PRIM_12
    0x201b,     // PRIM_13
    0x402b,     // PRIM_14
    0x8003,     // PRIM_15
    0x1002d,    // PRIM_16
    0x20009,    // PRIM_17
    0x40027,    // PRIM_18
    0x80027,    // PRIM_19
    0x100009,   // PRIM_20
    0x200005,   // PRIM_21
    0x400003,   // PRIM_22
    0x800021,   // PRIM_23
    0x100001b,  // PRIM_24
    0x2000009,  // PRIM_25
    0x4000047,  // PRIM_26
    0x8000027,  // PRIM_27
    0x10000009, // PRIM_28
    0x20000005, // PRIM_29
    0x40000053, // PRIM_30
    0x80000009  // PRIM_31
};

