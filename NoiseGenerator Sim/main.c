//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2017
// *
// * Zadatak: Generator periodicnog suma
// * Autor:Filip Jasic
// *
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
#include "Dsplib.h"
#include "Dsplib_c.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define AMPLITUDE_SCALE 1

/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(bufferL,4)
Int16 bufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(bufferR,4)
Int16 bufferR[AUDIO_IO_SIZE];

//odbirci za izlazak
Int16 outputBufferL[AUDIO_IO_SIZE];

//odbirci za generisani signal
Int16 outputBufferLgenerated[AUDIO_IO_SIZE];

char frekvencije[4] = {1520,1980,2490,3110};

void main( void )
{

	int i;
	int ph=0;//fazni pomeraj
	float f=0;
	char _key_pressed = 0 ,br=0;
	Uint16 key;

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
				key = EZDSP5535_SAR_getKey();
						switch(key)
						{
							case SW1:
						      if (_key_pressed == 0) {
						        clearLCD();
						        ph = 0;
						        br++;
						        br=br%4;
						        f=frekvencije[br];
						        if (br == 0) {
								clearLCD();
									printChar('1');
									printChar('5');
									printChar('2');
									printChar('0');
									printf("1520");
								}else if (br == 1) {
								clearLCD();
									printChar('1');
									printChar('9');
									printChar('8');
									printChar('0');
									printf("1980");
								}else if(br == 2) {
									clearLCD();
									printChar('2');
									printChar('4');
									printChar('9');
									printChar('0');
									printf("2490");
								}else if (br == 3) {
									clearLCD();
									printChar('3');
									printChar('1');
									printChar('1');
									printChar('0');
									printf("3110");
									}
								else {
									clearLCD();
										}
						        _key_pressed = 1;

						      }
						      break;

							case SW2:
							  if (_key_pressed == 0) {
								  	_key_pressed = 1;
							  }
							  break;

							case SW12:
								  if (_key_pressed == 0) {
								  _key_pressed = 1;
								  }
							  break;

							case NoKey:
							  _key_pressed = 0;
							  break;
						}
		aic3204_read_block(bufferL, bufferR);

		gen_sinus_table(AUDIO_IO_SIZE, AMPLITUDE_SCALE,(float) 3110/8000, ph, outputBufferLgenerated);

		/* TODO : Generisati sum zadate frekvencije i sabrati sa ulznim signalom. */
		for(i=0;i<AUDIO_IO_SIZE;i++){
			outputBufferL[i]=outputBufferLgenerated[i]+bufferL[i];
		}
		aic3204_write_block(outputBufferL, outputBufferL);
	}

	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}
