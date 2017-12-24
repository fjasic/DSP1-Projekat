//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2017
// *
// * Zadatak: Generator periodicnog suma
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
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "gen_sinus.h"
#include "math.h"
#include "Dsplib.h"
#include "Dsplib_c.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L
#define FL1 1520
#define AMPLITUDE_SCALE 1
#define PI 3.14159265
void key_pressed(char, float *);

/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(bufferL,4)
Int16 bufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(bufferR,4)
Int16 bufferR[AUDIO_IO_SIZE];

//odbirci za izlazak
#pragma DATA_ALIGN(bufferL,4)
Int16 outputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(bufferR,4)
Int16 outputBufferR[AUDIO_IO_SIZE];


//odbirci za generisani signal
#pragma DATA_ALIGN(bufferL,4)
Int16 outputBufferLgenerated[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(bufferR,4)
Int16 outputBufferRgenerated[AUDIO_IO_SIZE];

char Keys[5] = {'1', '2', '3', '4',' '};

void main( void )
{

	int i;
	int phaseL=0;//fazni pomeraj --levi
	int phaseR=0;//fazni pomeraj --desni
	int j;
	//int button = 0;
	float fl=0;
	char symbol = 0 , symbol_offset = 0 , _key_pressed = 0 , offset = 0;
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
		/* Popunjavanje DMA bafera sa nulama (Ping i Pong) kako bi na izlazu bila emitovana tisina dok nije pritisnut taster SW2 */
				for(j=0; j < AUDIO_IO_SIZE; j++) {
					outputBufferL[j] = 0;
					outputBufferR[j] = 0;

				}

				key = EZDSP5535_SAR_getKey();
						switch(key)
						{
							case SW1:
						      if (_key_pressed == 0) {
						        clearLCD();
						        phaseR = 0;
						        phaseL = 0;
						        _key_pressed = 1;
						        if (++offset > 16)
						          offset = 1;

						        symbol_offset = Keys[offset - 1];
						        printChar(symbol_offset);
						      }
						      break;

							case SW2:
							  if (_key_pressed == 0) {
								  	_key_pressed = 1;
							        symbol = symbol_offset;
							  }
							  break;

							case SW12:
							  break;

							case NoKey:
							  _key_pressed = 0;
							  symbol = Keys[0];
							  break;
						}
						key_pressed(symbol, &fl);
					/*
		int pressed;
		int pressedBefore = NoKey;
/*
		/* Ukoliko nije pritisnut SW2, a pritisnut je SW1 menjanje karaktera */
		/* dokle god pritiskam SW1 menja se karakter */
		/*		while((pressed = EZDSP5535_SAR_getKey()) != SW2) {
					if(pressed == SW1 && pressed != pressedBefore) {
						clearLCD();
						setWritePointerToFirstChar();
						printChar(buttons[button]);
						button++;
						button = button%16;
					}
					pressedBefore = pressed;
				}
				*/
				/* SW2 sluzi za slanje aktivnog karaktera(odnosno broja) */

					/*	while((pressed = EZDSP5535_SAR_getKey()) == SW2) {
							switch(buttons[button-1]) {
							case '1':
								f1 = FP + 1520;
								break;
							case '2':
								f1 = FP + 1980;
								break;
							case '3':
								f1 = FP + 2490;
								break;
							case '4':
								f1 = FP + 3110;
								break;
							}
						}*/


		aic3204_read_block(bufferL, bufferR);

		gen_sinus_table(AUDIO_IO_SIZE, AMPLITUDE_SCALE, 1520, phaseL, outputBufferLgenerated);
		gen_sinus_table(AUDIO_IO_SIZE, AMPLITUDE_SCALE, 1520, phaseR, outputBufferRgenerated);

		/* TODO : Generisati sum zadate frekvencije i sabrati sa ulznim signalom. */
		for(i=0;i<AUDIO_IO_SIZE;i++){
			outputBufferL[i]=outputBufferLgenerated[i]+bufferL[i];
			outputBufferR[i]=outputBufferRgenerated[i]+bufferR[i];

			/*printf("----ispis levog kanala: ");
					printf("%d",outputBufferL[i]);
							printf("\n");
					printf("---ispis desnog kanala: ");
					printf("%d",outputBufferR[i]);
						printf("\n");
						*/

		}


		aic3204_write_block(outputBufferL, outputBufferR);

	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}

void key_pressed(char pressed, float *fl)
{
  switch (pressed)
  {
  case '1':
    *fl = (FL1 + 0.0) / 8000.0;
    break;
  case '2':
    *fl = (FL1 + 460.0) / 8000.0;
    break;
  case '3':
    *fl = (FL1 + 510.0) / 8000.0;
    break;
  case '4':
    *fl = (FL1 + 620.0) / 8000.0;
    break;

  default:
    *fl = 0.0;
  }
}
