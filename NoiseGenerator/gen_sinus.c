#include "tistdtypes.h"
#include "gen_sinus.h"
#include "sine_table.h"
#include "math.h"

#define PI 3.14159265

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
  /* TODO */
  
  	int i = 0;
	int delta = f * SINE_TABLE_SIZE*4;
    int k = (ph/(2*PI)*SINE_TABLE_SIZE*4);
    int mask = ((SINE_TABLE_SIZE*4)-1);

	for (i = 0; i < n; i++)
	{
		//prvi kvadrant
		if (k < SINE_TABLE_SIZE) {
			buffer[i] = a*p_sine_table[k];
		//drugi kvadrant
			//k > SINE_TABLE_SIZE && uslov ne treba jer ako nije ispunjen prvi if to se podrazumeva isto vazi i za dalje provere
		} else if ( k < SINE_TABLE_SIZE*2) {
			buffer[i] = a*p_sine_table[2*SINE_TABLE_SIZE -k];
		//treci kvadrant
		} else if ( k < SINE_TABLE_SIZE*3) {
			buffer[i] = - a*p_sine_table[k - 2*SINE_TABLE_SIZE ];
		//cetvrti kvadrant
		} else if ( k < SINE_TABLE_SIZE*4) {
			buffer[i] = - a*p_sine_table[4*SINE_TABLE_SIZE-k];
		}

	     k = k & mask;
	     buffer[i] = a*p_sine_table[k];
	     k+=delta;
	}
}
