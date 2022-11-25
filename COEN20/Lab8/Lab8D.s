				.syntax     unified
				.cpu        cortex-m4
				.text
				.global     Time2Msecs
				.thumb_func
				.align

				// uint32_t Time2Msecs(uint32_t hour, uint32_t mins, uint32_t secs) ;
Time2Msecs:		// R0 = hour
				// R1 = mins
				// R2 = secs

				// compute hours in minutes
				LSL		R3,R0,6					// R3 <- 64*hour
				SUB		R0,R3,R0,LSL 2			// R0 <- (64*hour) - (4*hour) = 60 * hour

				// compute total minutes
				ADD		R0,R0,R1				// R0 <- mins + (60*hour)

				// compute seconds from minutes
				LSL		R3,R0,6					// R3 <- 64*(mins + (60*hour))
				SUB		R0,R3,R0,LSL 2			// R0 <- (64*(mins + (60*hour))) - (4*(mins + (60*hour))) = 60*(mins + (60*hour))

				// compute total seconds
				ADD		R0,R0,R2				// R0 <- secs + 60*(mins + (60*hour))

				// compute mseconds from seconds
				LSL		R3,R0,10				// R3 <- 1028*(secs + 60*(mins + (60*hour)))
				SUB		R3,R3,R0,LSL 5			// R3 <- (1028 - 32)*(secs + 60*(mins + (60*hour)))
				ADD		R0,R3,R0,LSL 2			// R0 <- (1028 - 32 + 4)*(secs + 60*(mins + (60*hour)))
				
				BX		LR						// return 1000*(secs + 60*(mins + (60*hour)))

				.global     Msecs2Time
				.thumb_func
				.align
			
				// void Msecs2Time(uint32_t msec, uint32_t *hour, uint32_t *mins, uint32_t *secs) ;
Msecs2Time:		// R0 = msec
				// R1 = ptr to hour
				// R2 = ptr to mins
				// R3 = ptr to secs
				
				PUSH	{R4-R12}						// preserve R4-R12

				// computing seconds from mseconds
				LDR		R4,=274877907					// magic constant = 274877907
				UMULL	R5,R4,R4,R0						// R5 <- (274877907 * msec)[63..32]
				LSRS	R4,R4,6							// R4 <- msec / 1000 = s

				// computing hour from mseconds
				LDR		R5,=2443359173					// magic constant = 2443359173
				UMULL	R6,R5,R5,R4						// R6 <- (2443359173 * s)[63..32]
				LSRS	R5,R5,11						// R5 <- secs / 3600 = h
				STR		R5,[R1]							// R4 -> [R1] (*hour = h)
				
				// updating seconds value
				LSL		R6,R5,11						// R6 <- 2048*h
				ADD		R6,R6,R5,LSL 10					// R6 <- 2048*h + 1024*h = 3072*h
				ADD		R6,R6,R5,LSL 9					// R6 <- 3072*h + 512*h = 3584*h
				ADD		R6,R6,R6,LSL 4					// R6 <- 3584*h + 16*h = 3600*h
				SUB		R4,R4,R6						// R4 <- s - 3600*h = s

				// computing minutes from seconds
				LDR		R6,=2290649225					// magic constant = 2290649225
				UMULL	R7,R6,R6,R4						// R7 <- (2290649225 * s)[63..32]
				LSRS	R6,R6,5							// R6 <- s / 60 = m
				STR		R6,[R2]							// R6 -> [R2] (*mins = m)

				// computing seconds
				LSL		R7,R6,6							// R7 <- 64*m
				SUB		R7,R7,R6,LSL 2					// R7 <- 64*m - 4*m = 60*m
				SUB		R4,R4,R7						// R4 <- s - 60*m = s
				STR		R4,[R3]							// R4 -> [R3] (*secs = s)

				POP		{R4-R12,PC}						// return
				.end
