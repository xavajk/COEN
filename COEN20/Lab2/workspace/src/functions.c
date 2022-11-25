#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// Add 1 to value represented by bit pattern
void Increment(int8_t bits[8]) {
    // loop thorugh bit array and change accordingly
    for (int i = 0; i < 8; ++i) {
        if (bits[i] == 0) {
            bits[i] = 1;
            return;
        }
        else bits[i] = 0;
    }
}

// Convert array of bits to unsigned int
uint32_t Bits2Unsigned(int8_t bits[8]) {
    // set up variables
    uint32_t val = 0;

    // loop through bit array and calculate value of binary number
    for (int i = 0; i < 8; ++i) {
        val += bits[i] * pow(2,i);
    }
    return val;
}

// Convert array of bits to signed int using method 2 from lec slides
int32_t Bits2Signed(int8_t bits[8]) {
    return bits[7] == 1 ? -1 * Bits2Unsigned(bits) - 256 : Bits2Unsigned(bits);
}

// Opposite of Bits2Unsigned
void Unsigned2Bits(uint32_t n, int8_t bits[8]) {
    // set up variables
    int rem, prevr;

    // loop thorugh bit array
    for (int i = 7; i >= 0; --i) {
        // set remainder values
        prevr = rem;
        rem = n % (int)pow(2,i);
        if (rem == n) {                             // if n % 2^i == n then 2^i > n --> bit[i] = 0
            bits[i] = 0;
            continue;
        }
        else if(rem != prevr) bits[i] = 1;          // if rem != prevr then 2^i < rem --> bit[i] = 1
        else bits[i] = 0;
    }

    // print bits for main function check
    for(int i = 7; i >= 0; --i) {
        printf("%u", bits[i]);
    }
}

/*
CHECK Unsigned2Bits step-by-step
n = 128
i = 7 : rem = 0, prevr = X,         bits[7] = 1
i = 6 : rem = 0, prevr = 0,         bits[6] = 0
i = 5 : rem = 22, prevr = 54,       bits[5] = 0
i = 4 : rem = 6, prevr = 22,        bits[4] = 0
i = 3 : rem = 6, prevr = 6,         bits[3] = 0
i = 2 : rem = 2, prevr = 6,         bits[2] = 0
i = 1 : rem = 0, prevr = 2,         bits[1] = 0
i = 0 : rem = 0, prevr = 0,         bits[0] = 0
*/

int main(int argc, char const *argv[]) {
    // set up variables
    int8_t bits1[8], bits2[8];
    u_int32_t n;
    char buf1[8];
    char buf2[3];

    // get user input
    printf("enter 8-bit number: ");
    gets(buf1);
    printf("enter positive decimal number (up to 255): ");
    gets(buf2);

    // convert user string to int arrays
    for (int i = 7, j = 0; i >= 0; --i, ++j) {
        bits1[i] = buf1[j] - 48;
        bits2[i] = buf1[j] - 48;
    }

    // convert user string to uint; check if within range [0,255]
    n = atoi(buf2);
    if (n > 255) exit(1);

    // print usage of functions to ensure proper functionality
    printf("\nBits2Signed:\t\t%s\t-->\t%d", buf1, Bits2Signed(bits1));
    // printf("\nBits2Unsigned:\t\t%s\t-->\t%d", buf1, Bits2Unsigned(bits2));
    // int8_t bits3[8];
    // printf("\nUnsigned2Bits:\t\t%u\t\t-->\t", n);
    // Unsigned2Bits(n, bits3);
    return 0;
}