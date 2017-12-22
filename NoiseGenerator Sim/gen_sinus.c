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
 
  /* TODO: DONE */
	int i = 0;
	int delta = f * SINE_TABLE_SIZE;
	int k = (ph/(2*PI)*SINE_TABLE_SIZE);


	int mask = (SINE_TABLE_SIZE-1);

	for(i = 0; i < n; i++) {
		k = k & mask;
	//prvi kvadrant
		if(k <= SINE_TABLE_SIZE)
			buffer[i] = _smpy(a , p_sine_table[k]);
		if(k > SINE_TABLE_SIZE && k <= 2*SINE_TABLE_SIZE)
	//drugi kvadrant
			buffer[i] = _smpy(a , p_sine_table[2*SINE_TABLE_SIZE - k]);
		if(k > 2*SINE_TABLE_SIZE && k <= 3*SINE_TABLE_SIZE)
	//treci kvadrant	
		buffer[i] = _smpy(a , p_sine_table[k - 2*SINE_TABLE_SIZE]);
		if(k > 3*N && k <= 4*N )
	//cetvrti kvadrant
			buffer[i] = _smpy(a , p_sine_table[4*SINE_TABLE_SIZE - k]);
		k+=delta;
	}
}
