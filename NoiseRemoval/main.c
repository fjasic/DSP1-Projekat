//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2017
// *
// * Zadatak: Uklanjanje periodicnog suma upotrebom notch filtra
// * Autor:
// *                                                                          
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "print_number.h"
#include "math.h"
#include "Dsplib.h"
#include "window.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

/* Velicina prozora za racunanje FFT-a */
/* DONE Dodeliti velicinu bloka kod racunanja FFT*/
#define FFT_SIZE 256

/* Trajanje (8000 odbiraka u 1s, 1ms = 8 odbiraka, 256ms = 2048 odbirka, ucitava se po 256 odbiraka, pa je potrebno 8 blokova ucitati*/
#define DELTA 8

/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];
/*Ovde smestam odbirke i radim sa njima */
Int16 oldValues[AUDIO_IO_SIZE];
Int16 newValues[AUDIO_IO_SIZE];
Int16 fftBuffer[FFT_SIZE];

/*Za racunanje snage*/
//Int32 componentPower[8];
Int32 magnitude_response[AUDIO_IO_SIZE/2];

void main( void )
{   
	int i;
	/*for(i = 0; i < AUDIO_IO_SIZE; i++){
	oldValues[i] = 0;
	}
	for(i = 0; i < 8; i++){
		componentPower[i] = 0;
	}

	*/
	/* Inicijalizaija razvojne ploce */
	EZDSP5535_init( );

    /* Inicijalizacija LCD kontrolera */
	initPrintNumber();

	printf("\n Uklanjanje sinusoidalnog suma iz signala \n");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    while(kraj)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		
		/* TODO*/

    	for(i=0;i<AUDIO_IO_SIZE;i++){
    		oldValues[i]=InputBufferL[i];
    		newValues[i]=InputBufferL[i];
    	}

    	/* Popunjavanje bafera za fft */
    			int k = AUDIO_IO_SIZE;
    			for(i = 0; i< AUDIO_IO_SIZE; i++, k++) {
    				fftBuffer[i] = oldValues[i];
    				fftBuffer[k] = newValues[i];

    			}

    			//prozoriranje
    			for(i = 0; i < FFT_SIZE; i++) {
    				newValues[i] = _smpy(fftBuffer[i], p_window[i]);	//mnozenje
    			}
    			//fft
    			rfft(newValues, FFT_SIZE, SCALE);

    			magnitude_response[0] = ((Int32)newValues[0])*newValues[0];
		aic3204_write_block(InputBufferL, InputBufferR);
	}
    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}
