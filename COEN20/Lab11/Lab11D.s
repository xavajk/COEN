						.syntax		unified
						.cpu		cortex-m4
						.text
						// Q16 Normalize(Q16 divisor, int zeros);
						.global		Normalize
						.thumb_func
						.align
Normalize:				LDR			R2,=0
						CMP			R1,16
						ITTEE		GE
						SUBGE		R1,R1,16
						ADDGE		R2,R2,R0,LSL R1
						SUBLT		R1,16,R1
						ADDLT		R2,R2,R0,LSR R1
						MOV 		R0,R2
						BX			LR

						// Q16 Denormalize(Q16 estimate, int zeros);
						.global		Denormalize
						.thumb_func
						.align
Denormalize:			LDR			R2,=0
						CMP			R1,16
						ITTEE		GE
						SUBGE		R1,R1,16
						ADDGE		R2,R2,R0,LSR R1
						SUBLT		R1,16,R1
						ADDLT		R2,R2,R0,LSL R1
						MOV 		R0,R2
						BX			LR

						// Q16 NormalizedEstimate(Q16 divisor);
						.global		NormalizedEstimate
						.thumb_func
						.align		// R0 = divisor
NormalizedEstimate:		PUSH		{R4,LR}
						LDR			R2,=185043						// R2 <- 185043
						LDR			R1,=123362						// R1 <- 123362
						BL			Q16Product						// R0 <- Q16Product(divisor, 123362)
						SUB			R0,R1,R0						// 185043 - R0
						POP			{R4,PC}

						// Q16 InitialEstimate(Q16 divisor);
						.global		InitialEstimate
						.thumb_func
						.align		// R0 = original divisor
InitialEstimate:		PUSH		{R4,LR}
						CLZ			R1,R0							// R1 <- zeros
						BL			Normalize						// R0 <- divisor (normalized)
						BL			NormalizedEstimate				// R0 <- estimate
						BL			Denormalize						// R0 <- final result
						POP			{R4,PC}

						// Q16 Reciprocal(Q16 divisor);
						.global		Reciprocal
						.thumb_func
						.align		// R0 = divisor
Reciprocal:				PUSH		{R4-R9,LR}
						LDR			R2,=132							// R2 <- 2*FIXED(0.001)
						LDR			R3,=66							// R3 <- FIXED(0.001)
						LDR			R4,=131072						// R4 <- FIXED(2.0)
						MOV 		R5,R0							// R5 <- divisor
						BL			InitialEstimate					// R0 <- curr
						MOV			R7,R0							// R7 (curr) <- R0

						// R6 = prev, R7 = curr, R8 = diff
Do:						MOV			R6,R7							// prev = curr
						MOV			R1,R6							// R1 <- R6 (prev)
						MOV			R0,R5							// R0 <- R5 (divisor)
						BL			Q16Product						// R0 <- Q16Product(divisor, prev)
						SUB			R0,R4,R0						// temp = FIXED(2.0) - Q16Product(divisor, prev)
						MOV			R1,R6							// R1 <- R6 (prev)
						BL			Q16Product						// R0 <- Q16Product(temp, prev)
						MOV			R7,R0							// R7 (curr) <- R0
						SUB			R8,R7,R6						// R8 <- diff = curr - prev
						ADD			R9,R8,R3						// R9 <- diff + FIXED(0.001)
						CMP			R9,R2							// (diff + FIXED(0.001)) > 2*FIXED(0.001)
						BLE			Do								// continue loop
						
						MOV			R0,R7							// R0 <- R7
						POP			{R4-R5,PC}

Q16Product:				SMULL		R0,R1,R0,R1						// R1.R0 <- A*B 
						LSR			R0,R0,16						// extract middle
						ORR			R0,R0,R1,LSL 16					// 32 bits
						.end