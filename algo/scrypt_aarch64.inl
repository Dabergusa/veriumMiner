#ifndef __scrypt_armv8_inl__
#define __scrypt_armv8_inl__

#include <arm_neon.h>

static inline void scrypt_shuffle(uint32_t B[16])
{
	uint32_t x0 = 	B[0]; 
	uint32_t x1 = 	B[1]; 
	uint32_t x2 = 	B[2];
	uint32_t x3 = 	B[3]; 
	uint32_t x4 = 	B[4];
	uint32_t x5 = 	B[5]; 
	uint32_t x6 = 	B[6]; 
	uint32_t x7 = 	B[7]; 
	uint32_t x8 = 	B[8]; 
	uint32_t x9 = 	B[9]; 
	uint32_t x10 = B[10]; 
	uint32_t x11 = B[11]; 
	uint32_t x12 = B[12];
	uint32_t x13 = B[13]; 
	uint32_t x14 = B[14]; 
	uint32_t x15 = B[15]; 

	B[0] = x0;  B[1] = x5;  B[2] = x10;  B[3] = x15;
	B[4] = x12; B[5] = x1;  B[6] = x6;   B[7] = x11;
	B[8] = x8;  B[9] = x13; B[10] = x2;  B[11] = x7;
	B[12] = x4; B[13] = x9; B[14] = x14; B[15] = x3;
}

static inline void scrypt_core_3way(uint32_t B[32 * 3], uint32_t *V, uint32_t N)
{
	uint32_t* W = V;

	scrypt_shuffle(&B[0  + 0]);
	scrypt_shuffle(&B[16 + 0]);
	scrypt_shuffle(&B[0 + 32]);
	scrypt_shuffle(&B[16 + 32]);
	scrypt_shuffle(&B[0 + 64]);
	scrypt_shuffle(&B[16 + 64]);

	uint32x4x4_t q_a, q_b, q_c, q_tmp;
	uint32x4x4_t ba_a, bb_a, bc_a, ba_b, bb_b, bc_b;

	ba_a.val[0] = vld1q_u32(&B[( 0) / 4]);
	ba_a.val[1] = vld1q_u32(&B[(16) / 4]);
	ba_a.val[2] = vld1q_u32(&B[(32) / 4]);
	ba_a.val[3] = vld1q_u32(&B[(48) / 4]);
	ba_b.val[0] = vld1q_u32(&B[(0 + 64 + 0) / 4]);
	ba_b.val[1] = vld1q_u32(&B[(0 + 64 + 16) / 4]);
	ba_b.val[2] = vld1q_u32(&B[(0 + 64 + 32) / 4]);
	ba_b.val[3] = vld1q_u32(&B[(0 + 64 + 48) / 4]);

	bb_a.val[0] = vld1q_u32(&B[(128 +  0) / 4]);
	bb_a.val[1] = vld1q_u32(&B[(128 + 16) / 4]);
	bb_a.val[2] = vld1q_u32(&B[(128 + 32) / 4]);
	bb_a.val[3] = vld1q_u32(&B[(128 + 48) / 4]);
	bb_b.val[0] = vld1q_u32(&B[(128 + 64 + 0) / 4]);
	bb_b.val[1] = vld1q_u32(&B[(128 + 64 + 16) / 4]);
	bb_b.val[2] = vld1q_u32(&B[(128 + 64 + 32) / 4]);
	bb_b.val[3] = vld1q_u32(&B[(128 + 64 + 48) / 4]);
	
	bc_a.val[0] = vld1q_u32(&B[(256 + 0) / 4]);
	bc_a.val[1] = vld1q_u32(&B[(256 + 16) / 4]);
	bc_a.val[2] = vld1q_u32(&B[(256 + 32) / 4]);
	bc_a.val[3] = vld1q_u32(&B[(256 + 48) / 4]);
	bc_b.val[0] = vld1q_u32(&B[(256 + 64 + 0) / 4]);
	bc_b.val[1] = vld1q_u32(&B[(256 + 64 + 16) / 4]);
	bc_b.val[2] = vld1q_u32(&B[(256 + 64 + 32) / 4]);
	bc_b.val[3] = vld1q_u32(&B[(256 + 64 + 48) / 4]);

	// prep

	vst1q_u32(&V[( 0) / 4], ba_a.val[0]);
	vst1q_u32(&V[(16) / 4], ba_a.val[1]);
	vst1q_u32(&V[(32) / 4], ba_a.val[2]);
	vst1q_u32(&V[(48) / 4], ba_a.val[3]);
	vst1q_u32(&V[(64) / 4],  ba_b.val[0]);
	vst1q_u32(&V[(80) / 4],  ba_b.val[1]);
	vst1q_u32(&V[(96) / 4],  ba_b.val[2]);
	vst1q_u32(&V[(112) / 4], ba_b.val[3]);

	vst1q_u32(&V[(128 +  0) / 4], bb_a.val[0]);
	vst1q_u32(&V[(128 + 16) / 4], bb_a.val[1]);
	vst1q_u32(&V[(128 + 32) / 4], bb_a.val[2]);
	vst1q_u32(&V[(128 + 48) / 4], bb_a.val[3]);
	vst1q_u32(&V[(128 + 64) / 4],  bb_b.val[0]);
	vst1q_u32(&V[(128 + 80) / 4],  bb_b.val[1]);
	vst1q_u32(&V[(128 + 96) / 4],  bb_b.val[2]);
	vst1q_u32(&V[(128 + 112) / 4], bb_b.val[3]);

	vst1q_u32(&V[(256 +  0) / 4], bc_a.val[0]);
	vst1q_u32(&V[(256 + 16) / 4], bc_a.val[1]);
	vst1q_u32(&V[(256 + 32) / 4], bc_a.val[2]);
	vst1q_u32(&V[(256 + 48) / 4], bc_a.val[3]);
	vst1q_u32(&V[(256 + 64) / 4], bc_b.val[0]);
	vst1q_u32(&V[(256 + 80) / 4], bc_b.val[1]);
	vst1q_u32(&V[(256 + 96) / 4], bc_b.val[2]);
	vst1q_u32(&V[(256 + 112) / 4],bc_b.val[3]);

	V += 96;

	for (int n = 0; n < N; n++)
	{
		// loop 1 part a
		q_a.val[0] = veorq_u32(ba_b.val[0], ba_a.val[0]);
		q_a.val[1] = veorq_u32(ba_b.val[1], ba_a.val[1]);
		q_a.val[2] = veorq_u32(ba_b.val[2], ba_a.val[2]);
		q_a.val[3] = veorq_u32(ba_b.val[3], ba_a.val[3]);

		q_b.val[0] = veorq_u32(bb_b.val[0], bb_a.val[0]);
		q_b.val[1] = veorq_u32(bb_b.val[1], bb_a.val[1]);
		q_b.val[2] = veorq_u32(bb_b.val[2], bb_a.val[2]);
		q_b.val[3] = veorq_u32(bb_b.val[3], bb_a.val[3]);

		q_c.val[0] = veorq_u32(bc_b.val[0], bc_a.val[0]);
		q_c.val[1] = veorq_u32(bc_b.val[1], bc_a.val[1]);
		q_c.val[2] = veorq_u32(bc_b.val[2], bc_a.val[2]);
		q_c.val[3] = veorq_u32(bc_b.val[3], bc_a.val[3]);

		ba_a = q_a;
		bb_a = q_b;
		bc_a = q_c;

		for (int i = 0; i < 4; i ++)
		{
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[1]);  	
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);	
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[1]);  	
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[1]); 
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7); 				
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);				
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 3);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 3);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 3);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 1);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 1);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 1);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);

			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 3);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 3);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 3);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 1);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 1);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 1);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
		}
		ba_a.val[0] = vaddq_u32(ba_a.val[0], q_a.val[0]);
		ba_a.val[1] = vaddq_u32(ba_a.val[1], q_a.val[1]);
		ba_a.val[2] = vaddq_u32(ba_a.val[2], q_a.val[2]);
		ba_a.val[3] = vaddq_u32(ba_a.val[3], q_a.val[3]);

		q_a = ba_a;

		bb_a.val[0] = vaddq_u32(bb_a.val[0], q_b.val[0]);
		bb_a.val[1] = vaddq_u32(bb_a.val[1], q_b.val[1]);
		bb_a.val[2] = vaddq_u32(bb_a.val[2], q_b.val[2]);
		bb_a.val[3] = vaddq_u32(bb_a.val[3], q_b.val[3]);

		q_b = bb_a;

		bc_a.val[0] = vaddq_u32(bc_a.val[0], q_c.val[0]);
		bc_a.val[1] = vaddq_u32(bc_a.val[1], q_c.val[1]);
		bc_a.val[2] = vaddq_u32(bc_a.val[2], q_c.val[2]);
		bc_a.val[3] = vaddq_u32(bc_a.val[3], q_c.val[3]);

		q_c = bc_a;
		
		for (int i = 0; i < 4; i++)
		{
			vst1q_u32(&V[      (i * 4) ], ba_a.val[i]);
			vst1q_u32(&V[(32 + (i * 4))], bb_a.val[i]);
			vst1q_u32(&V[(64 + (i * 4))], bc_a.val[i]);
		}

		// loop 1 part b

		q_a.val[0] = veorq_u32(ba_b.val[0], q_a.val[0]);
		q_a.val[1] = veorq_u32(ba_b.val[1], q_a.val[1]);
		q_a.val[2] = veorq_u32(ba_b.val[2], q_a.val[2]);
		q_a.val[3] = veorq_u32(ba_b.val[3], q_a.val[3]);
		ba_b = q_a;

		q_b.val[0] = veorq_u32(bb_b.val[0], q_b.val[0]);
		q_b.val[1] = veorq_u32(bb_b.val[1], q_b.val[1]);
		q_b.val[2] = veorq_u32(bb_b.val[2], q_b.val[2]);
		q_b.val[3] = veorq_u32(bb_b.val[3], q_b.val[3]);
		bb_b = q_b;

		q_c.val[0] = veorq_u32(bc_b.val[0], q_c.val[0]);
		q_c.val[1] = veorq_u32(bc_b.val[1], q_c.val[1]);
		q_c.val[2] = veorq_u32(bc_b.val[2], q_c.val[2]);
		q_c.val[3] = veorq_u32(bc_b.val[3], q_c.val[3]);
		bc_b = q_c;


		for (int i = 0; i < 4; i ++)
		{
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[1]);  	
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);	
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[1]);  	
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[1]); 
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7); 				
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);				
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 3);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 3);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 3);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 1);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 1);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 1);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);

			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 3);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 3);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 3);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 1);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 1);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 1);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
		}

		ba_b.val[0] = vaddq_u32(q_a.val[0], ba_b.val[0]);
		ba_b.val[1] = vaddq_u32(q_a.val[1], ba_b.val[1]);
		ba_b.val[2] = vaddq_u32(q_a.val[2], ba_b.val[2]);
		ba_b.val[3] = vaddq_u32(q_a.val[3], ba_b.val[3]);
		bb_b.val[0] = vaddq_u32(q_b.val[0], bb_b.val[0]);
		bb_b.val[1] = vaddq_u32(q_b.val[1], bb_b.val[1]);
		bb_b.val[2] = vaddq_u32(q_b.val[2], bb_b.val[2]);
		bb_b.val[3] = vaddq_u32(q_b.val[3], bb_b.val[3]);
		bc_b.val[0] = vaddq_u32(q_c.val[0], bc_b.val[0]);
		bc_b.val[1] = vaddq_u32(q_c.val[1], bc_b.val[1]);
		bc_b.val[2] = vaddq_u32(q_c.val[2], bc_b.val[2]);
		bc_b.val[3] = vaddq_u32(q_c.val[3], bc_b.val[3]);
		for (int i = 0; i < 4; i++)
		{
			vst1q_u32(&V[(     16 + (i * 4))], ba_b.val[i]);
			vst1q_u32(&V[(32 + 16 + (i * 4))], bb_b.val[i]);
			vst1q_u32(&V[(64 + 16 + (i * 4))], bc_b.val[i]);
		}
		V += 96;
	}
	V = W;

    // loop 2

	uint32x4x4_t x;

	uint32_t one =   32 * (3 * (ba_b.val[0][0] & (N - 1)) + 0);
	uint32_t two =   32 * (3 * (bb_b.val[0][0] & (N - 1)) + 1);
	uint32_t three = 32 * (3 * (bc_b.val[0][0] & (N - 1)) + 2);
	x.val[0] = vld1q_u32(&W[one +  0]);
	x.val[1] = vld1q_u32(&W[one +  4]);
	x.val[2] = vld1q_u32(&W[one +  8]);
	x.val[3] = vld1q_u32(&W[one + 12]);

	for (int n = 0; n < N; n++)
	{
		// loop 2 part a

		ba_a.val[0] = veorq_u32(ba_a.val[0], x.val[0]);
			x.val[0] = vld1q_u32(&W[one + 16 +  0]);
		ba_a.val[1] = veorq_u32(ba_a.val[1], x.val[1]);
			x.val[1] = vld1q_u32(&W[one + 16 +  4]);
		ba_a.val[2] = veorq_u32(ba_a.val[2], x.val[2]);
			x.val[2] = vld1q_u32(&W[one + 16 +  8]);
		ba_a.val[3] = veorq_u32(ba_a.val[3], x.val[3]);

			ba_b.val[0] = veorq_u32(ba_b.val[0], x.val[0]);
			ba_b.val[1] = veorq_u32(ba_b.val[1], x.val[1]);
			x.val[3] = vld1q_u32(&W[one + 16 + 12]);
			ba_b.val[2] = veorq_u32(ba_b.val[2], x.val[2]);
			ba_b.val[3] = veorq_u32(ba_b.val[3], x.val[3]);
		x.val[0] = vld1q_u32(&W[two +  0]);
				q_a.val[0] = veorq_u32(ba_b.val[0], ba_a.val[0]);
				q_a.val[1] = veorq_u32(ba_b.val[1], ba_a.val[1]);
		x.val[1] = vld1q_u32(&W[two +  4]);
				q_a.val[2] = veorq_u32(ba_b.val[2], ba_a.val[2]);
				q_a.val[3] = veorq_u32(ba_b.val[3], ba_a.val[3]);
		x.val[2] = vld1q_u32(&W[two +  8]);
		ba_a = q_a;

		x.val[3] = vld1q_u32(&W[two + 12]);

		bb_a.val[0] = veorq_u32(bb_a.val[0], x.val[0]);
			x.val[0] = vld1q_u32(&W[two + 16 +  0]);
		bb_a.val[1] = veorq_u32(bb_a.val[1], x.val[1]);
			x.val[1] = vld1q_u32(&W[two + 16 +  4]);
		bb_a.val[2] = veorq_u32(bb_a.val[2], x.val[2]);
			x.val[2] = vld1q_u32(&W[two + 16 +  8]);
		bb_a.val[3] = veorq_u32(bb_a.val[3], x.val[3]);
			bb_b.val[0] = veorq_u32(bb_b.val[0], x.val[0]);
			x.val[3] = vld1q_u32(&W[two + 16 + 12]);
			bb_b.val[1] = veorq_u32(bb_b.val[1], x.val[1]);
		x.val[0] = vld1q_u32(&W[three +  0]);
			bb_b.val[2] = veorq_u32(bb_b.val[2], x.val[2]);
			bb_b.val[3] = veorq_u32(bb_b.val[3], x.val[3]);
		x.val[1] = vld1q_u32(&W[three +  4]);
				q_b.val[0] = veorq_u32(bb_b.val[0], bb_a.val[0]);
				q_b.val[1] = veorq_u32(bb_b.val[1], bb_a.val[1]);
		x.val[2] = vld1q_u32(&W[three +  8]);
				q_b.val[2] = veorq_u32(bb_b.val[2], bb_a.val[2]);
				q_b.val[3] = veorq_u32(bb_b.val[3], bb_a.val[3]);
		x.val[3] = vld1q_u32(&W[three + 12]);
		bb_a = q_b;

		bc_a.val[0] = veorq_u32(bc_a.val[0], x.val[0]);
			x.val[0] = vld1q_u32(&W[three + 16 +  0]);
		bc_a.val[1] = veorq_u32(bc_a.val[1], x.val[1]);
			x.val[1] = vld1q_u32(&W[three + 16 +  4]);
		bc_a.val[2] = veorq_u32(bc_a.val[2], x.val[2]);
			x.val[2] = vld1q_u32(&W[three + 16 +  8]);
		bc_a.val[3] = veorq_u32(bc_a.val[3], x.val[3]);
			bc_b.val[0] = veorq_u32(bc_b.val[0], x.val[0]);
			x.val[3] = vld1q_u32(&W[three + 16 + 12]);
			bc_b.val[1] = veorq_u32(bc_b.val[1], x.val[1]);
			bc_b.val[2] = veorq_u32(bc_b.val[2], x.val[2]);
			bc_b.val[3] = veorq_u32(bc_b.val[3], x.val[3]);
				q_c.val[0] = veorq_u32(bc_b.val[0], bc_a.val[0]);
				q_c.val[1] = veorq_u32(bc_b.val[1], bc_a.val[1]);
				q_c.val[2] = veorq_u32(bc_b.val[2], bc_a.val[2]);
				q_c.val[3] = veorq_u32(bc_b.val[3], bc_a.val[3]);
		bc_a = q_c;

		for (int i = 0; i < 4; i++)
		{
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[1]);  	
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);	
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[1]);  	
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[1]); 
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7); 				
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);				
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 3);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 3);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 3);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 1);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 1);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 1);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);

			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 3);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 3);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 3);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 1);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 1);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 1);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
		}
		ba_a.val[0] = vaddq_u32(ba_a.val[0], q_a.val[0]);
		ba_a.val[1] = vaddq_u32(ba_a.val[1], q_a.val[1]);
		ba_a.val[2] = vaddq_u32(ba_a.val[2], q_a.val[2]);
		ba_a.val[3] = vaddq_u32(ba_a.val[3], q_a.val[3]);

		q_a = ba_a;

		bb_a.val[0] = vaddq_u32(bb_a.val[0], q_b.val[0]);
		bb_a.val[1] = vaddq_u32(bb_a.val[1], q_b.val[1]);
		bb_a.val[2] = vaddq_u32(bb_a.val[2], q_b.val[2]);
		bb_a.val[3] = vaddq_u32(bb_a.val[3], q_b.val[3]);
		q_b = bb_a;

		bc_a.val[0] = vaddq_u32(bc_a.val[0], q_c.val[0]);
		bc_a.val[1] = vaddq_u32(bc_a.val[1], q_c.val[1]);
		bc_a.val[2] = vaddq_u32(bc_a.val[2], q_c.val[2]);
		bc_a.val[3] = vaddq_u32(bc_a.val[3], q_c.val[3]);
		q_c = bc_a;

		// loop 2 b

		q_a.val[0] = veorq_u32(ba_b.val[0], q_a.val[0]);
		q_a.val[1] = veorq_u32(ba_b.val[1], q_a.val[1]);
		q_a.val[2] = veorq_u32(ba_b.val[2], q_a.val[2]);
		q_a.val[3] = veorq_u32(ba_b.val[3], q_a.val[3]);
		ba_b = q_a;

		q_b.val[0] = veorq_u32(bb_b.val[0], q_b.val[0]);
		q_b.val[1] = veorq_u32(bb_b.val[1], q_b.val[1]);
		q_b.val[2] = veorq_u32(bb_b.val[2], q_b.val[2]);
		q_b.val[3] = veorq_u32(bb_b.val[3], q_b.val[3]);
		bb_b = q_b;

		q_c.val[0] = veorq_u32(bc_b.val[0], q_c.val[0]);
		q_c.val[1] = veorq_u32(bc_b.val[1], q_c.val[1]);
		q_c.val[2] = veorq_u32(bc_b.val[2], q_c.val[2]);
		q_c.val[3] = veorq_u32(bc_b.val[3], q_c.val[3]);
		bc_b = q_c;


		for (int i = 0; i < 3; i++)
		{
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[1]);  	
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);	
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[1]);  	
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[1]); 
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7); 				
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);				
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 3);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 3);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 3);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 1);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 1);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 1);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);

			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);

			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 3);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 3);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 3);

			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 1);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 1);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 1);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
		}
		{
			//1
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[1]);  	
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);	
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[1]);  	
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[1]); 
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7); 				
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);				
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			//2
			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);
			//3
			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 3);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 3);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 3);
			//4
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			//5
			q_tmp.val[0] = vaddq_u32(q_a.val[0], q_a.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 7);
			q_tmp.val[2] = vaddq_u32(q_b.val[0], q_b.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 25);
			q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 1);
			q_a.val[1] = veorq_u32(q_tmp.val[1], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 7);
			q_tmp.val[3] = vaddq_u32(q_c.val[0], q_c.val[3]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 25);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 1);
			q_b.val[1] = veorq_u32(q_tmp.val[1], q_b.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 7);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 25);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 1);
			q_c.val[1] = veorq_u32(q_tmp.val[1], q_c.val[1]);
			//6
			q_tmp.val[0] = vaddq_u32(q_a.val[1], q_a.val[0]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 9);
			q_tmp.val[2] = vaddq_u32(q_b.val[1], q_b.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 23);
			q_a.val[2] = veorq_u32(q_tmp.val[1], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 9);
			q_tmp.val[3] = vaddq_u32(q_c.val[1], q_c.val[0]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 23);
			q_b.val[2] = veorq_u32(q_tmp.val[1], q_b.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 9);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 23);
			q_c.val[2] = veorq_u32(q_tmp.val[1], q_c.val[2]);
			//7
			q_tmp.val[0] = vaddq_u32(q_a.val[2], q_a.val[1]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 13);
			q_tmp.val[2] = vaddq_u32(q_b.val[2], q_b.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 19);
			q_a.val[3] = veorq_u32(q_tmp.val[1], q_a.val[3]);
				q_a.val[1] = vextq_u32(q_a.val[1], q_a.val[1], 3);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 13);
			q_tmp.val[3] = vaddq_u32(q_c.val[2], q_c.val[1]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 19);
			q_b.val[3] = veorq_u32(q_tmp.val[1], q_b.val[3]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 13);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 19);
			q_c.val[3] = veorq_u32(q_tmp.val[1], q_c.val[3]);
			q_b.val[1] = vextq_u32(q_b.val[1], q_b.val[1], 3);
			q_c.val[1] = vextq_u32(q_c.val[1], q_c.val[1], 3);

			//8
			q_tmp.val[0] = vaddq_u32(q_a.val[3], q_a.val[2]);
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[0], 18);
			q_tmp.val[2] = vaddq_u32(q_b.val[3], q_b.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[0], 14);
			q_a.val[0] = veorq_u32(q_tmp.val[1], q_a.val[0]);
				ba_b.val[0] = vaddq_u32(q_a.val[0], ba_b.val[0]);
					one =	32 * (3 * (ba_b.val[0][0] & (N - 1)) + 0);
					__builtin_prefetch(&W[one + 0]);
					__builtin_prefetch(&W[one + 8]);
					__builtin_prefetch(&W[one + 16]);
					__builtin_prefetch(&W[one + 24]);
			
			q_a.val[2] = vextq_u32(q_a.val[2], q_a.val[2], 2);
			q_b.val[2] = vextq_u32(q_b.val[2], q_b.val[2], 2);
			
			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[2], 18);
			q_tmp.val[3] = vaddq_u32(q_c.val[3], q_c.val[2]);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[2], 14);
			q_c.val[2] = vextq_u32(q_c.val[2], q_c.val[2], 2);
			q_b.val[3] = vextq_u32(q_b.val[3], q_b.val[3], 1);
			q_b.val[0] = veorq_u32(q_tmp.val[1], q_b.val[0]);
				bb_b.val[0] = vaddq_u32(q_b.val[0], bb_b.val[0]);
					two =	32 * (3 * (bb_b.val[0][0] & (N - 1)) + 1);
					__builtin_prefetch(&W[two + 0]);
					__builtin_prefetch(&W[two + 8]);
					__builtin_prefetch(&W[two + 16]);
					__builtin_prefetch(&W[two + 24]);

			q_tmp.val[1] = vshlq_n_u32(q_tmp.val[3], 18);
			q_tmp.val[1] = vsriq_n_u32(q_tmp.val[1], q_tmp.val[3], 14);
			q_a.val[3] = vextq_u32(q_a.val[3], q_a.val[3], 1);
			q_c.val[3] = vextq_u32(q_c.val[3], q_c.val[3], 1);
			q_c.val[0] = veorq_u32(q_tmp.val[1], q_c.val[0]);
				bc_b.val[0] = vaddq_u32(q_c.val[0], bc_b.val[0]);
					three = 32 * (3 * (bc_b.val[0][0] & (N - 1)) + 2);
					__builtin_prefetch(&W[three + 0]);
					__builtin_prefetch(&W[three + 8]);
					__builtin_prefetch(&W[three + 16]);
					__builtin_prefetch(&W[three + 24]);
		}

		x.val[0] = vld1q_u32(&W[one +  0]);
		ba_b.val[1] = vaddq_u32(q_a.val[1], ba_b.val[1]);
		ba_b.val[2] = vaddq_u32(q_a.val[2], ba_b.val[2]);
		ba_b.val[3] = vaddq_u32(q_a.val[3], ba_b.val[3]);
		x.val[1] = vld1q_u32(&W[one +  4]);
		bb_b.val[1] = vaddq_u32(q_b.val[1], bb_b.val[1]);
		bb_b.val[2] = vaddq_u32(q_b.val[2], bb_b.val[2]);
		bb_b.val[3] = vaddq_u32(q_b.val[3], bb_b.val[3]);
		x.val[2] = vld1q_u32(&W[one +  8]);
		bc_b.val[1] = vaddq_u32(q_c.val[1], bc_b.val[1]);
		bc_b.val[2] = vaddq_u32(q_c.val[2], bc_b.val[2]);
		bc_b.val[3] = vaddq_u32(q_c.val[3], bc_b.val[3]);
		x.val[3] = vld1q_u32(&W[one + 12]);
	}

	vst1q_u32(&B[0],       ba_a.val[0]);
	vst1q_u32(&B[4],       ba_a.val[1]);
	vst1q_u32(&B[8],       ba_a.val[2]);
	vst1q_u32(&B[12],      ba_a.val[3]);
	vst1q_u32(&B[16 + 0],  ba_b.val[0]);
	vst1q_u32(&B[16 + 4],  ba_b.val[1]);
	vst1q_u32(&B[16 + 8],  ba_b.val[2]);
	vst1q_u32(&B[16 + 12], ba_b.val[3]);

	vst1q_u32(&B[32 + 0],  		bb_a.val[0]);
	vst1q_u32(&B[32 + 4],  		bb_a.val[1]);
	vst1q_u32(&B[32 + 8],  		bb_a.val[2]);
	vst1q_u32(&B[32 + 12], 		bb_a.val[3]);
	vst1q_u32(&B[32 + 16 + 0],  bb_b.val[0]);
	vst1q_u32(&B[32 + 16 + 4],  bb_b.val[1]);
	vst1q_u32(&B[32 + 16 + 8],  bb_b.val[2]);
	vst1q_u32(&B[32 + 16 + 12], bb_b.val[3]);

	vst1q_u32(&B[64 + 0],  		bc_a.val[0]);
	vst1q_u32(&B[64 + 4],  		bc_a.val[1]);
	vst1q_u32(&B[64 + 8],  		bc_a.val[2]);
	vst1q_u32(&B[64 + 12], 		bc_a.val[3]);
	vst1q_u32(&B[64 + 16 + 0],  bc_b.val[0]);
	vst1q_u32(&B[64 + 16 + 4],  bc_b.val[1]);
	vst1q_u32(&B[64 + 16 + 8],  bc_b.val[2]);
	vst1q_u32(&B[64 + 16 + 12], bc_b.val[3]);

	scrypt_shuffle(&B[0  + 0]);
	scrypt_shuffle(&B[16 + 0]);
	scrypt_shuffle(&B[0 + 32]);
	scrypt_shuffle(&B[16 + 32]);
	scrypt_shuffle(&B[0 + 64]);
	scrypt_shuffle(&B[16 + 64]);
}

#endif 
