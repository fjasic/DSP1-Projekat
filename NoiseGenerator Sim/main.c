//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2017
// *
// * Zadatak: Generator periodicnog suma
// * Autor:Filip Jasic
// * Index:RA46-2014
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "gen_sinus.h"
#include "math.h"


/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L
/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(bufferL,4)
Int16 bufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(bufferR,4)
Int16 bufferR[AUDIO_IO_SIZE];

//ovde smestam generisani signal
#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];

//rezultat sabiranje ulaznog i generisanog signala
//#pragma DATA_ALIGN(OutputBufferSumLeft,4)
Int16 OutputBufferSumLeft[AUDIO_IO_SIZE];
//#pragma DATA_ALIGN(OutputBufferSumRight,4)
Int16 OutputBufferSumRight[AUDIO_IO_SIZE];

void main( void )
{
	/*VARIJABLE */
	float fl=0,fr=0;
	float phaseL=0,phaseR=0;
	int i;

    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Dodavanje suma u signal \n");

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

		aic3204_read_block(bufferL, bufferR);

		/* TODO : Generisati sum zadate frekvencije i sabrati sa ulznim signalom. */
		gen_sinus_table(AUDIO_IO_SIZE,16383,fl,phaseL,OutputBufferL);
		gen_sinus_table(AUDIO_IO_SIZE,16383,fr,phaseR,OutputBufferR);


		for (i = 0; i < AUDIO_IO_SIZE; i++)
				{
			OutputBufferSumLeft[i] = bufferL[i] + OutputBufferL[i];
			OutputBufferSumRight[i] = bufferR[i] + OutputBufferR[i];


				}



		aic3204_write_block(OutputBufferSumLeft, OutputBufferSumRight);

	}

	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


