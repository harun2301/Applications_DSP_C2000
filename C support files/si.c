/*
*	Procesamiento Digital de Senales
*			Proyecto 2
*
*	Autores:	Rafael Araujo Lague, Oscar Harun Baños Mancilla
*	Fecha:		Junio 04, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926

// **************************************** F U N C T I O N   F I R M S ****************************************

double * oscilador(double *osc,double fDig,double A,long N);
double * read_voice(FILE *read, double *voice, long N);
double * add_voice_tones(double *tone1, double *tone2, double *voice, double *voice2tones, long N);



// **************************************** M A I N   F U N C T I O N ****************************************

int main(void){
	
	long N = 40000;				// signal length
	long k,j = 0;				// counters
	float fs = 8000.0;			// sampling frequency
	float f1 = 127.0;			// 1st tone analog frequency 127 Hz
	float f2 = 352.0;			// 2st tone analog frequency 352 Hz
	float f_d = 0.0;			// digital frequency
	float A = 0.2;				// oscillator amplitude

	
	FILE *write;				// pointer for writing files
	FILE *read;					// pointer for reading files
	
	double *tone1, *tone2;					// arrays for oscillator values
	double *voice;							// array for voice sampled values
	double *voice2tones;					// array for voice + 127 Hz + 352 Hz
	tone1 = calloc(N,sizeof(double));		// memory reserved for the 1st tone values array
	tone2 = calloc(N,sizeof(double));		// memory reserved for the 2nd tone values array
	voice = calloc(N,sizeof(double));		// memory reserved for the 2nd tone values array
	voice2tones = calloc(N,sizeof(double));	// memory reserved for the 2nd tone values array
	
		
		
	// -------------------- Creation of 1st tone = 127 Hz --------------------
	f_d = f1/fs; 								// digital frequency F_d = F_an / F_samp
	tone1 = oscilador(tone1, f_d, A, N);		// creates tone with digital ocillator
	
	write = fopen("127.dat","w");				// Saving tone values to .dat file
	for(j=0; j<N; j++){
		fprintf(write, "%2.6f\n", tone1[j]);	
	}
	fclose(write);
	
	
	// -------------------- Creation of 2nd tone = 352 Hz --------------------
	f_d = f2/fs; 								// digital frequency F_d = F_an / F_samp
	tone2 = oscilador(tone2, f_d, A, N);		// creates tone with digital ocillator
	
	write = fopen("352.dat","w");				// Saving tone values to .dat file
	for(j=0; j<N; j++){
		fprintf(write, "%2.6f\n", tone2[j]);
	}
	fclose(write);


	// -------------------- Reading of voice file --------------------
    read = fopen("audio_grabado_8000.dat", "r");
    if (read == NULL) {
        perror("Error File. Can't open.'");
        return 1;
    }
	voice = read_voice(read, voice, N);
	fclose(read);


	// -------------------- Adding tones to voice --------------------
	voice2tones = add_voice_tones(tone1, tone2, voice, voice2tones, N);
	write = fopen("voz_y_tonos.dat","w");				// Saving tone values to .dat file
	for(j=0; j<N; j++){
		fprintf(write, "%2.7f\n", voice2tones[j]);
	}
	fclose(write);

	return 0;
}




// **************************************** F U N C T I O N   D E F I N I T I O N S ****************************************

/*
 * =========================================================
 * Digital oscillator for 2nd order IIR quasi stable systems 
 * =========================================================
 *      double *osc : Array with floating point values
		
        double f_dig: Digital frequency
                    f_dig = f_analog/f_sampling

        double A : Amplitude of output signal

        long N : Array size. Number of samples
*/
double * oscilador(double *osc,double fDig,double A,long N){

    double b0, a1;
    double oscifloat, osc1float, osc2float;
    long i;

    b0 = sin(2*PI*fDig);

    a1 = -2*cos(2*PI*fDig);

	// condiciones iniciales
    osc1float = -a1*b0;
    osc2float = b0;

    osc[0] = A*osc2float;

    osc[1] = A*osc1float;

	// comienza a partir de y[2]
    for (i = 2; i < N; i++){
        oscifloat = -a1*osc1float-osc2float;
        osc[i] = A*oscifloat;
        osc2float = osc1float;
        osc1float = oscifloat;
    }
    
	return osc;
}

/*
 * =========================================================
 * Reading samples from a .dat file and saving to an array
 * =========================================================
 *      FILE *read : pointer for reading files
		
        double *voice: pointer to the array in which samples 
                    will be stored

        long N : Array size. Number of samples
*/
double * read_voice(FILE *read, double *voice, long N){
	long n=0;
	double sample;
	
	// LÃª os dados do arquivo e armazena no vetor seno_original
    while (fscanf(read, "%lf", &sample) == 1 && n<N) {
        voice[n++] = sample;
    }
	return voice;
}

/*
 * =========================================================
 * Adding two different frequency tones to a voice sample
 * =========================================================
 *  
 		double *tone1: pointer to the array of 1st tone
 		
 		double *tone2: pointer to the array of 2nd tone
 		
 		double *voice: pointer to the array containing the
 					voice samples
        
		double *voice2tones: pointer to the array in which
                    the new signal will be stored

        long N : Array size. Number of samples
*/
double * add_voice_tones(double *tone1, double *tone2, double *voice, double *voice2tones, long N){
	long n;
	
	for(n=0; n<N; n++){
		voice2tones[n] = voice[n] + tone1[n] + tone2[n];
	}	
	
	return voice2tones;
}
