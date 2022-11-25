		.syntax     unified
		.cpu        cortex-m4
        .text

// ----------------------------------------------------------
// int32_t Return32Bits(void) { return (int32_t) +10; }
// ----------------------------------------------------------

		.global		ret32
        .align
        .thumb_func
Return32Bits: // return 10 as 32 bit signed integer
		LDR 	R0,=10
		BX 		LR

// ----------------------------------------------------------
// int64_t Return64Bits(void) { return (int64_t) -10; }
// ----------------------------------------------------------

		.global		ret64
        .align
        .thumb_func
Return64Bits:	// return -10 as 64 bit signed integer
		LDR 	R0,=-10
		LDR 	R1,=1
		BX 		LR


// ----------------------------------------------------------
// uint8_t Add8Bits(uint8_t x, uint8_t y) { return (uint8_t) (x + y); }
// ----------------------------------------------------------

		.global		add8
        .align
        .thumb_func
Add8Bits: // add 2 8-bit unsigned integers (x and y)
		ADD 	RO,R0,R1
		BX 		LR


// ----------------------------------------------------------
// uint32_t FactSum32(uint32_t x, uint32_t y) { return (uint32_t) Factorial(x + y); }
// ----------------------------------------------------------

		.global		fsum32
        .align
        .thumb_func
FactSum32: // return factorial of sum (x + y)
		
		BX 		LR


// ----------------------------------------------------------
// uint32_t XPlusGCD(uint32_t x, uint32_t y, uint32_t z) { return (uint32_t) (x + gcd(y, z)); }
// ----------------------------------------------------------

		.global		xgcd
        .align
        .thumb_func
XPlusGCD: // return x + greatest common denominator between y and z
		
		BX 		LR

		.end

