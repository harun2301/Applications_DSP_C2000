/**	
 * Compile command on linux terminal
 * gcc [name].c -lm -o [name].out
 * 
 * Run command on linux terminal
 * ./[nombre].out
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/**
 * 	Signals parameters
 **/
#define	N	1000			// no. of samples
#define f7	7 				// cycles of 1st tone
#define f15	15 				// cycles of 2nd tone
#define f25 25 				// cycles of 3rd tone
#define A   1				// amplitude
#define PI 	3.1415926		// pi constant


/**
 * 	Fixed point formats
 **/
#define Q12 12
#define Q10 10
#define Q8  8
#define Q4 	4


/**
 * 	Function signatures
 **/
int float_to_Qi(int Qi, float num);
int* sine_to_Qi(int *signal_qi, double *signal_float, int qi);
void create_signal_data_file_qi(int *signal, const char *filename);
void create_signal_data_file_float(double *signal, const char *filename);
double* iir_osc(double *osc,double fDig,double amplitude,long samples);
double* add_tones(double* sum, double *tone1, double *tone2, double *tone3);

/*
* ====================================================================================================================
* M A I N 			M A I N 			M A I N 			M A I N 			M A I N 			M A I N 			
* ====================================================================================================================
*/
int main(void){
	
	double *tone1, *tone2, *tone3;			// arrays for oscillator values
	double *three_tones;
	int *three_tones_qi;

	tone1 = calloc(N,sizeof(double));		// memory reserved for the 1st tone values array
	tone2 = calloc(N,sizeof(double));		// memory reserved for the 2nd tone values array
	tone3 = calloc(N,sizeof(double));		// memory reserved for the 3rd tone values array
	three_tones = calloc(N,sizeof(double));	// memory reserved for the 3 tones combined values array	
	
	// creates tone with digital ocillator
	tone1 = iir_osc(tone1, (float)f7/N, A, N);
	create_signal_data_file_float(tone1,"data_files/7_cycles.dat");
	
	// creates tone with digital ocillator
	tone2 = iir_osc(tone2, (float)f15/N, A, N);
	create_signal_data_file_float(tone2,"data_files/15_cycles.dat");
	
	// creates tone with digital ocillator
	tone3 = iir_osc(tone3, (float)f25/N, A, N);
	create_signal_data_file_float(tone3,"data_files/25_cycles.dat");

	
	// adds 3 tones into one signal
	three_tones = add_tones(three_tones, tone1, tone2, tone3);
	// converts signal to qi
	three_tones_qi = sine_to_Qi(three_tones_qi, three_tones, Q12);
	create_signal_data_file_qi(three_tones_qi,"data_files/3_tones_Q12.dat");
	
	
	system("gnuplot -p 'graf.gp'");

	return 0;
}


/*
* ====================================================================================================================
* F U N C T I O N 	D E F I N I T I O N S 			F U N C T I O N 	D E F I N I T I O N S			
* ====================================================================================================================
*/

/**
 *	@brief	Converts a signal of float values to fixed point values in the Qi given
 *	
 * 	@param 	Qi Given fixed point format
 * 	@param 	num Floating point number to be converted
 * 	
 * 	@retval number converted to Qi fixed point
 **/
int* sine_to_Qi(int *signal_qi, double *signal_float, int qi){
	int i;
	for(i=0; i<N; i++)		
			signal_qi[i] = float_to_Qi(qi, signal_float[i]);

	return signal_qi;
}

/**
 *	@brief	Converts a float number to fixed point in the Qi given
 *	
 * 	@param 	Qi Given fixed point format
 * 	@param 	num Floating point number to be converted
 * 	
 * 	@retval number converted to Qi fixed point
 **/
int float_to_Qi(int Qi, float num){
    int punto_fijo = 0;
    punto_fijo = (int)(num*pow(2,Qi));
    return punto_fijo;
}


/**
 *  @brief	Creates a sine signal from IIR quasi-stable system
 * 	
 * 	@param 	*osc Array with floating point values	
 *  @param  fDig Digital frequency (fDig = cycles/samples)
 *	@param 	amplitude Amplitude of output signal
 *	@param  samples Array size, number of samples
 * 	
 * 	@retval	generated signal
 **/
double * iir_osc(double *osc,double fDig,double amplitude,long samples){

    double b0, a1;
    double oscifloat, osc1float, osc2float;
    long i;

    b0 = sin(2*PI*fDig);

    a1 = -2*cos(2*PI*fDig);

	// initial conditions
    osc1float = -a1*b0;
    osc2float = b0;

    osc[0] = A*osc2float;

    osc[1] = A*osc1float;

	// starts from y[2]
    for (i = 2; i < N; i++){
        oscifloat = -a1*osc1float-osc2float;
        osc[i] = A*oscifloat;
        osc2float = osc1float;
        osc1float = oscifloat;
    }
    
	return osc;
}


/**
 *  @brief	Adds 3 tones into one mixed signal
 * 	
 * 	@param 	*sum Array with floating point values of resulting signal	
 * 	@param 	*tone1 Array with floating point values of 1st tone
 * 	@param 	*tone2 Array with floating point values of 2nd tone
 * 	@param 	*tone3 Array with floating point values of 3rd tone
 * 	
 * 	@retval	generated signal
 **/
double* add_tones(double* sum, double *tone1, double *tone2, double *tone3){
	int i;

	for(i=0; i<N; i++)
		sum[i] = tone1[i] + tone2[i] + tone3[i];

	return sum;
}


/**
 *	@brief	Creates a .dat file with a signal points in float format
 * 	@param 	*signal input signal, of type double, to be saved
 *	@param 	*filename name + extension of the file to be created
 * 	@retval None
 **/
void create_signal_data_file_float(double *signal, const char *filename){
	int i;
	FILE *write;
	write = fopen(filename,"w");
    if (write == NULL) {
        perror("Error opening file");
    }else{
		for(i=0; i<N; i++)
			fprintf(write, "%2.6f\n", signal[i]);

		fclose(write);
	}

}


/**
 *	@brief	Creates a .dat file with a signal points in integer (qi) format
 * 	@param 	*signal input signal, of type integer, to be saved
 *	@param 	*filename name + extension of the file to be created
 * 	@retval None
 **/
void create_signal_data_file_qi(int *signal, const char *filename){
	int i;
	FILE *write;
	write = fopen(filename,"w");
    if (write == NULL) {
        perror("Error opening file");
    }else{
		for(i=0; i<N; i++)
			fprintf(write, "%d\n", signal[i]);

		fclose(write);
	}

}
