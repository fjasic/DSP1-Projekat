//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2015
// *
// * Zadatak: Prijemnik DTMF signala
// * Autor:   Filp Jasic
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
#include "iir.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Velicina prozora za racunanje FFT-a */
#define FFT_SIZE 256


/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(outputBufferL,4)
Int16 outputBufferL[AUDIO_IO_SIZE];

Int16 oldValues[AUDIO_IO_SIZE];
Int16 newValues[AUDIO_IO_SIZE];
Int16 fftBuffer[FFT_SIZE];
Int16 importantComponents[4] = {85,20, 27, 32};

Int32 componentPower[4];

Int16 coefficients_1520[]={-7277,3385,9884,-7277,9884};
Int16 coefficients_1980[]={-301,5662,9597,-301,9597};
Int16 coefficients_2490[]={7430,3409,9897,7430, 9897};
Int16 coefficients_3110[]={17238,6136,11258,17238,11258};
Int16 z_x[]={0,0};
Int16 z_y[]={0,0};
Int16 rezultat;

void main( void )
{
	int i,chosenFreq;
    Int16* x;
    x=z_x;
	/*Int16* y;
	y=&z_y;
	Int16 *p;
	p=&coefficients_1980;
*/
	for(i = 0; i < 4; i++)
		componentPower[i] = 0;

	/* Inicijalizaija razvojne ploce */
	EZDSP5535_init( );

    /* Inicijalizacija LCD kontrolera */
	initPrintNumber();

	printf("\n DTMF Prijemnik\n");
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
		/* Preuzimanje odbiraka */
		for(i = 0; i < AUDIO_IO_SIZE; i++) {
			newValues[i] = InputBufferL[i];

		}
		/* Popunjavanje bafera za fft */
		for(i = 0; i < AUDIO_IO_SIZE; i++) {
			fftBuffer[i] = newValues[i];
		}
		/* Prozoriranje */
		for(i = 0; i < FFT_SIZE; i++) {
			fftBuffer[i] = _smpy(fftBuffer[i], p_window[i]);


		}
		/* FFT */
		rfft(fftBuffer, FFT_SIZE, SCALE);
		for(i=0;i<FFT_SIZE;i++){
		}

		/* Racunanje snaga */
		for(i = 0; i < 4; i++)
			componentPower[i] = (Int32)fftBuffer[2*importantComponents[i]]*fftBuffer[2*importantComponents[i]] + (Int32)fftBuffer[2*importantComponents[i] + 1]*fftBuffer[2*importantComponents[i] + 1];
		/* Racunanje najjace i druge najjace snage radi provere sa pragom */
		Int32 biggest = componentPower[0];
		Int16 biggestIndex = 0;

		for(i = 1; i < 4; i++) {
			if(componentPower[i] > biggest) {
				biggest = componentPower[i];
				biggestIndex = i;
				printf("%maksmalni koeficijent ld",biggest);
				printf("koef %d",biggestIndex);
			}
		}

		/* Provera frekvencije */

			if(biggestIndex == 0) {
				chosenFreq=1;
				Int16 *p=coefficients_1520;
				printf("izabrani karakter je: %d...",chosenFreq);
				printf("pronadjena frekvencija je:1520Hz");
				printf("\n");
				//for(i = 0; i < AUDIO_IO_SIZE; i++) {
								//rezultat=second_order_IIR(InputBufferL[i],p,x,y);
								//printf("%ld",rezultat);
								//printf("\n");
								//outputBufferL[i] = rezultat[i];
							//}


				aic3204_write_block(fftBuffer, fftBuffer);
				/* Prekid veze sa AIC3204 kodekom */
			    aic3204_disable();

			    printf( "\n***Kraj programa***\n" );
				SW_BREAKPOINT;
			}
			if(biggestIndex == 1) {
				chosenFreq=2;
				Int16 *p=coefficients_1980;
				printf("izabrani karakter je: %d...",chosenFreq);
				printf("pronadjena frekvencija je:1980Hz");
				printf("\n");
				//for(i = 0; i < AUDIO_IO_SIZE; i++) {
								//rezultat=second_order_IIR(InputBufferL[i],p,x,y);
								//printf("%ld",rezultat);
								//printf("\n");
								//outputBufferL[i] = rezultat[i];
							//}


				aic3204_write_block(fftBuffer, fftBuffer);
				/* Prekid veze sa AIC3204 kodekom */
			    aic3204_disable();

			    printf( "\n***Kraj programa***\n" );
				SW_BREAKPOINT;
			}
			if(biggestIndex == 2) {
				chosenFreq=3;
				Int16 *p=coefficients_2490;
				printf("izabrani karakter je: %d...",chosenFreq);
				printf("pronadjena frekvencija je:2490Hz");
				printf("\n");
				//for(i = 0; i < AUDIO_IO_SIZE; i++) {
								//rezultat=second_order_IIR(InputBufferL[i],p,x,y);
								//printf("%ld",rezultat);
								//printf("\n");
								//outputBufferL[i] = rezultat[i];
							//}

				aic3204_write_block(fftBuffer, fftBuffer);
				/* Prekid veze sa AIC3204 kodekom */
			    aic3204_disable();

			    printf( "\n***Kraj programa***\n" );
				SW_BREAKPOINT;
			}
			if(biggestIndex == 3) {
				chosenFreq=4;
				Int16 *p=coefficients_3110;
				printf("izabrani karakter je: %d...",chosenFreq);
				printf("pronadjena frekvencija je:3110Hz");
				printf("\n");
				//for(i = 0; i < AUDIO_IO_SIZE; i++) {
								//rezultat=second_order_IIR(InputBufferL[i],p,x,y);
								//printf("%ld",rezultat);
								//printf("\n");
								//outputBufferL[i] = rezultat[i];
							//}


				aic3204_write_block(fftBuffer, fftBuffer);
				/* Prekid veze sa AIC3204 kodekom */
			    aic3204_disable();

			    printf( "\n***Kraj programa***\n" );
				SW_BREAKPOINT;
			}else{
				printf("nije pronadjena odgovarajuca frekvencija...");
				//for(i = 0; i < AUDIO_IO_SIZE; i++) {
								//rezultat=second_order_IIR(InputBufferL[i],p,x,y);
								//printf("%ld",rezultat);
								//printf("\n");
								//outputBufferL[i] = rezultat[i];
							//}

				aic3204_write_block(fftBuffer, fftBuffer);
				/* Prekid veze sa AIC3204 kodekom */
			    aic3204_disable();

			    printf( "\n***Kraj programa***\n" );
				SW_BREAKPOINT;
			}
	}
}
