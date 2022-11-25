        .syntax     unified
		.cpu        cortex-m4
        .text

// void OffBy0(void *dst, const void *src) ;
		.global		OffBy0
        .thumb_func
        .align

OffBy0:	rept.	250 			// repeat LDR/STR instructions 250 times
		LDR		R2,[R1],4		// LDR src file into R2, 4-byte offset
		STR		R2,[R0],4		// STR R2 (src file data) into R0 (dst file), 4-byte offset
		endr.					// end repeat cycle
		BX		LR				// return --> [X][X][X][X]
		

// void OffBy1(void *dst, const void *src) ;
		.global		OffBy1
        .thumb_func
        .align

OffBy1:	rept.	3				// repeat LDR/STR instructions 3 times (3 * 1 byte = 3 bytes)
		LDR		R2,[R1],1		// LDR src file into R2, 1-byte offset
		STR		R2,[R0],1		// STR R2 (src file data) into R0 (dst file), 1-byte offset
		endr.					// end repeat cycle

		rept.	249 			// repeat LDR/STR instructions 249 times (249 * 4 bytes = 996 bytes)
		LDR		R2,[R1],4		// LDR src file into R2, 4-byte offset
		STR		R2,[R0],4		// STR R2 (src file data) into R0 (dst file), 4-byte offset
		endr.					// end repeat cycle

		LDR		R2,[R1],1		// 1 byte left over to copy
		STR		R2,[R0],1
		BX		LR				// return --> [X][X][X][]


// void OffBy2(void *dst, const void *src) ;
		.global		OffBy2
        .thumb_func
        .align

OffBy2:	rept.	2				// repeat LDR/STR instructions 2 times (2 * 1 byte = 2 bytes)
		LDR		R2,[R1],1		// LDR src file into R2, 1-byte offset
		STR		R2,[R0],1		// STR R2 (src file data) into R0 (dst file), 1-byte offset
		endr.					// end repeat cycle

		rept.	249 			// repeat LDR/STR instructions 249 times (249 * 4 bytes = 996 bytes)
		LDR		R2,[R1],4		// LDR src file into R2, 4-byte offset
		STR		R2,[R0],4		// STR R2 (src file data) into R0 (dst file), 4-byte offset
		endr.					// end repeat cycle

		rept.	2				// repeat LDR/STR instructions 2 times (2 bytes left over to copy)
		LDR		R2,[R1],1		
		STR		R2,[R0],1
		endr.
		BX			LR			// return --> [X][X][][]


		// void OffBy3(void *dst, const void *src) ;
		.global		OffBy3
        .thumb_func
        .align

OffBy3:	LDR		R2,[R1],1		// LDR src file into R2, 1-byte offset
		STR		R2,[R0],1		// STR R2 (src file data) into R0 (dst file), 1-byte offset

		rept.	249 			// repeat LDR/STR instructions 249 times (249 * 4 bytes = 996 bytes)
		LDR		R2,[R1],4		// LDR src file into R2, 4-byte offset
		STR		R2,[R0],4		// STR R2 (src file data) into R0 (dst file), 4-byte offset
		endr.					// end repeat cycle

		rept.	3				// repeat LDR/STR instructions 3 times (3 bytes left over to copy)
		LDR		R2,[R1],1		
		STR		R2,[R0],1
		endr.
		BX			LR			// return --> [X][][][]
		.end