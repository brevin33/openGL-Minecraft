#include <cstdint>
#ifndef NIBBLE_H
#define NIBBLE_H

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

inline char setLowerNibble(char orig, char nibble) {
    char res = orig;
    res &= 0xF0; // Clear out the lower nibble
    res |= (nibble & 0x0F); // OR in the desired mask
    return res;
}

inline char setUpperNibble(char orig, char nibble) {
    char res = orig;
    res &= 0x0F; // Clear out the upper nibble
    res |= ((nibble << 4) & 0xF0); // OR in the desired mask
    return res;
}

#endif