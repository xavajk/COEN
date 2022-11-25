			.syntax     unified
			.cpu        cortex-m4
			.text
			// #define ASR31(x) ((x < 0) ? -1 : 0)							// Same as arithmetic shift right by 31 bits	
			// rounding = ((((ASR31(dvnd*dvsr)) * dvsr) << 1) + dvsr) / 2 ;	// rounding (where dvnd = x*mtop, dvsr = mbtm)
			// quotient = (dvnd + rounding) / dvsr ;						// quotient = (x*mtop + rounding) / mbtm
			/* ------------------------------------------------------------------------------------------------------------------- */
			// Note: The purpose of ASR31(dvnd*dvsr) is to produce a result that will be -1 if dvnd/dvsr is negative, else 0. 
			// Multiplying by dvsr and shifting left once produces -2*dvsr if dvnd < 0, else 0. To this we add dvsr, which yields
			// -dvsr if dvnd < 0, and +dvsr if dvnd >= 0. Finally, we divide by 2 to produce a rounding that has the sign of
			// dvnd and magnitude |dvsr/2|. 
			//
			// The 2nd product (dvnd*dvsr) below may overflow if MUL is used, generating a positive result when it should be negative, 
			// or vice-versa. The solution below avoids this problem by using the most-significant half of a 64-bit product that  
			// cannot overflow. There are two alternatives: (1) Use SMMUL R0,R1,R2 (covered in Chapter 8) that computes only the 
			// most-signficant half of a 64-bit product, or (2) use XOR R0,R1,R2 (covered in Chapter 7).
			/* ------------------------------------------------------------------------------------------------------------------- */

			// int32_t MxPlusB(int32_t x, int32_t mtop, int32_t mbtm, int32_t b) ;
			.global		MxPlusB
			.thumb_func
			.align
MxPlusB:	PUSH		{R4-R6,LR}		// R4, R5 safe to use
			MUL			R4,R0,R1		// x*mtop --> R4
			SMMUL		R4,R2,R4		// x*mtop*mbtm (dvnd*dvsr) --> R4
			ASR 		R4,R4,31		// (dvnd*dvsr) >> 31 --> R4
			MUL			R4,R2,R4		// R4 * dvsr --> R4
			LSL			R4,R4,1			// R4 << 1 --> R4
			ADD 		R4,R2,R4		// R4 + dvsr --> R4
			LDR			R6,=2			// load 3 into R6 to be divisor
			SDIV		R4,R4,R6		// R4 / 2 --> R4 (rounding value)
			MUL			R5,R0,R1		// x*mtop --> R5
			ADD 		R4,R4,R5		// R4 + R5 --> R4 (mtop*x + rounding)
			SDIV		R0,R4,R2		// R4 / dvsr --> R0
			ADD			R0,R0,R3		// R4 + b --> R0
			POP 		{R4-R6,PC}		// return R0
			.end
