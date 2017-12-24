#include "tistdtypes.h"
#include "gen_sinus.h"
#include "sine_table.h"
#include "math.h"

#define PI 3.14159265
Int16 N = SINE_TABLE_SIZE;
/********************************************************************
 * Generisanje sinusnog signala upotrebom tabele pretrazivanja
 *   n - broj odbiraka
 *   a - amplituda (u opsegu 0 - 1.0)
 *   f - frekvencija (normalizovana)
 *   ph - fazni pomeraj
 *   buffer - niz u kome ce biti smesten izlazni signal
 *********************************************************************/

void gen_sinus_table(Int16 n, Int16 a, float f, float ph, Int16 buffer[])
{
 
		int i = 0;
		int delta = f * N;

		int k = (ph/(2*PI)* N);
		int mask = (N-1);

		for (i = 0; i < n; i++)
		{
			k = k & mask;
			if(k < N) {
				buffer[i] = a*p_sine_table[k];
			}
			else if((k >= N) && (k < N*2)) {
				buffer[i] = a*p_sine_table[N*2-k];
			}
			else if((k >= 2*N) && (k < N*3)) {
				buffer[i] = -a*p_sine_table[k-2*N];
			} else
				buffer[i] = -a*p_sine_table[4*N - k ];
			k += delta;
		}
		ph=k;
	}
