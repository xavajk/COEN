			// void CopyCell(uint32_t *dst, uint32_t *src) ;		// R0 = dst, R1 = src
			.syntax     unified
			.cpu        cortex-m4
			.text
			.global		CopyCell
			.thumb_func
			.align

CopyCell:	PUSH 	{R4,LR}				// R4 safe to use
			LDR 	R2,=0				// row = 0
			LDR		R3,=0				// col = 0
			LDR 	R4,=60				// compare register = 60
Top1:		CMP 	R2,R4				// compare row, 60
			BHS		EndRows1			// end of outer loop if row >= 60
Top2:		CMP		R3,R4				// compare col, 60
			BHS		EndCols1			// end of inner loop if col >= 60
			LDR		R0,[R0,R3,4]		// load (dst + col) into R0
			STR		R0,[R1,R3,4]		// *(dst + col) = *(src + col)
			B		NextCol1			// inner loop body done, move to next col

NextRow1:	ADD 	R2,R2,1				// row += 1
			B		Top1				// jump to start of outer loop

NextCol1:	ADD		R3,R3,1				// col += 1
			B 		Top2				// jump to start of inner loop

EndCols1:	ADD		R0,R0,240			// dst += 240 (LSL by 2?)
			ADD		R1,R1,240			// src += 240 (LSL by 2?)
			B		NextRow1			// end of cols, move to next row

EndRows1:	POP		{PC}				// return

			// void FillCell(uint32_t *dst, uint32_t color) ;		// R0 = dst, R1 = color
			.global		FillCell
			.thumb_func
			.align

FillCell:	PUSH 	{R4,LR}				// R4 safe to use
			LDR 	R2,=0				// row = 0
			LDR		R3,=0				// col = 0
			LDR 	R4,=60				// compare register = 60
Top3:		CMP 	R2,R4				// compare row, 60
			BHS		EndRows2			// end of outer loop if row >= 60
Top4:		CMP		R3,R4				// compare col, 60
			BHS 	EndCols2			// end of inner loop if col >= 60
			STR		R1,[R0,R3,LSL 2]	// *(dst + col) = pixel
			B		NextCol2			// inner loop body done, move to next col
			
NextRow2:	ADD 	R2,R2,1				// row += 1
			B		Top3				// jump to start of outer loop

NextCol2:	ADD 	R3,R3,1				// col += 1
			B		Top4				// jump to start of inner loop

EndCols2:	ADD 	R0,R0,240			// dst += 240 (LSL by 2?)
			B		NextRow1			// end of cols, move to next row

EndRows2:	POP		{PC}				// return
			.end
