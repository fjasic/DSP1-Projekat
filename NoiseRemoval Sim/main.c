//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2017
// *
// * Zadatak: Uklanjanje periodicnog suma upotrebom notch filtra
// * Autor: Filip Jasic
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

/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];
/*Ovde smestam odbirke i radim sa njima */
Int16 oldValues[AUDIO_IO_SIZE];
Int16 newValues[AUDIO_IO_SIZE];
Int16 fftBuffer[FFT_SIZE];

Int32 componentPower[AUDIO_IO_SIZE];
Int32 maxVrednost;

void main( void )
{
	int i,maxKoeficijent;
	for(i = 0; i < AUDIO_IO_SIZE; i++){
	oldValues[i] = 0;
	}
	/*
	for(i = 0; i < 8; i++){
		componentPower[i] = 0;
	}

	*/
	/* Inicijalizaija razvojne ploce */
	EZDSP5535_init( );

    /* Inicijalizacija LCD kontrolera */
	initPrintNumber();

	printf("\n Uklanjanje sinusoidalnog suma iz signala \n");

    /* Podesavanje ulazne i izlazne datoteke za simulaciju AD/DA konvertora */
	aic3204_set_input_filename("../input1.pcm");
	aic3204_set_output_filename("../output1.pcm");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();

    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    while(1)
	{

    	aic3204_read_block(InputBufferL, InputBufferR);

		/* TODO*/
    	for(i = 0; i < AUDIO_IO_SIZE; i++) {
    			newValues[i] = InputBufferL[i];
    		}
    		/* Popunjavanje bafera za fft */
    		int k = AUDIO_IO_SIZE;
    		for(i = 0; i < AUDIO_IO_SIZE; i++, k++) {
    			fftBuffer[i] = oldValues[i];
    			fftBuffer[k] = newValues[i];
    		}
    		/* Prozoriranje */
    		for(i = 0; i < FFT_SIZE; i++) {
    			fftBuffer[i] = _smpy(fftBuffer[i], p_window[i]);
    		}
    		/* FFT */
    		rfft(fftBuffer, FFT_SIZE, SCALE);

    		/* Racunanje snaga */
    	for(i = 0; i < AUDIO_IO_SIZE; i++){
    componentPower[i] = (Int32)fftBuffer[2*i]*fftBuffer[2*i] + (Int32)fftBuffer[2*i + 1]*fftBuffer[2*i+ 1];
    			}

    	componentPower[0]=(Int32)fftBuffer[0]*fftBuffer[0] + (Int32)fftBuffer[0]*fftBuffer[0];
        maxVrednost=componentPower[0];
        for(i=1;i<AUDIO_IO_SIZE;i++){
        	if(componentPower[i]>maxVrednost){
        	    			maxVrednost=componentPower[i];
        	    			maxKoeficijent=i;
        	    		}
        }

		aic3204_write_block(newValues, InputBufferR);
	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


