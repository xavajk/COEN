			.syntax     unified
			.cpu        cortex-m4
			.text
			.global		PutNibble
			.thumb_func
			.align
			// void PutNibble(void *nibbles_R0, uint32_t index_R1, uint32_t nibble_R2);
PutNibble:	PUSH		{R4-R5,LR}
			LDRB		R3,[R0]
			ADD			R3,R3,R1,LSR 1 
			AND			R4,R1,1
			LDRB		R5,[R3]
			CMP 		R4,1
			ITTEE		EQ
			ANDEQ		R5,R5,0b1111
			ORREQ		R5,R5,R2,LSL 4
			ANDNE		R5,R5,0b11110000
			ORRNE		R5,R5,R2
			STRB		R5,[R0]
			POP			{PC,LR}
			BX			LR
			
			.global		GetNibble
			.thumb_func
			.align
			// uint32_t GetNibble(void *nibbles_R0, uint32_t index_R1);
GetNibble:	LDRB		R2,[R0]
			MUL			R2,R2,R1,LSR 1
			AND			R3,R1,1
			CMP 		R3,1
			IT			EQ
			LSREQ		R2,R2,4
			AND			R0,R2,0b1111
			BX			LR
			.end
