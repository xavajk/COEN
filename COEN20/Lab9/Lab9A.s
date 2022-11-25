				.syntax		unified
				.cpu		cortex-m4
				.text
				.global		Discriminant
				.thumb_func
				.align
				// float Discriminant(float a, float b, float c) ;
Discriminant:	VMUL.F32	S1,S1,S1					// b*b -> S1
				VMUL.F32	S0,S0,S2					// a*c -> S0
				VMOV		S2,4.0						// 4.0 -> S2
				VMUL.F32	S0,S0,S2					// 4.0*a*c -> S0
				VSUB.F32	S0,S1,S0					// b*b - 4.0*a*c -> S0
				BX			LR

				.global		Root1
				.thumb_func
				.align
				// float Root1(float a, float b, float c) ;
Root1:			PUSH		{R4,LR}						// preserve LR
				VMOV		R0,S0						// save a in R0
				VMOV		R1,S1						// save b in R1
				BL			Discriminant				// call Discriminant function, result in S0
				VMOV		S1,R0						// move a into S1
				VMOV		S2,R1						// move b into S2
				VSQRT.F32	S0,S0						// sqrt(Discriminant) -> S0
				VNEG.F32	S2,S2						// -b -> S2
				VMOV		S3,2.0						// 2.0 -> S3
				VMUL.F32	S1,S1,S3					// 2.0*a -> S1
				VADD.F32	S0,S2,S0					// -b + sqrt(Discriminant) -> S0
				VDIV.F32	S0,S0,S1					// -b + sqrt(Discriminant) / 2.0*a -> S0
				POP			{R4,PC}						// return

				.global		Root2
				.thumb_func
				.align
				// float Root2(float a, float b, float c) ;
Root2:			PUSH		{R4,LR}						// preserve LR
				VMOV		R0,S0						// save a in R0
				VMOV		R1,S1						// save b in R1
				BL			Discriminant				// call Discriminant function, result in S0
				VMOV		S1,R0						// move a into S1
				VMOV		S2,R1						// move b into S2
				VSQRT.F32	S0,S0						// sqrt(Discriminant) -> S0
				VNEG.F32	S2,S2						// -b -> S2
				VMOV		S3,2.0						// 2.0 -> S3
				VMUL.F32	S1,S1,S3					// 2.0*a -> S1
				VSUB.F32	S0,S2,S0					// -b - sqrt(Discriminant) -> S0
				VDIV.F32	S0,S0,S1					// -b - sqrt(Discriminant) / 2.0*a -> S0
				POP			{R4,PC}						// return

				.global		Quadratic
				.thumb_func
				.align
				// float Quadratic(float x, float a, float b, float c) ;
Quadratic:		VMUL.F32	S1,S0,S1					// x*a -> S1
				VADD.F32	S1,S2,S1					// b + x*a -> S1
				VMUL.F32	S1,S0,S1					// x(b + x*a) -> S1
				VADD.F32	S0,S1,S3					// c + x(b + x*a) -> S0
				BX			LR
				.end
