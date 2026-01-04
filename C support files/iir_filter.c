/**	
 * Compile command on linux terminal
 * gcc [name].c -lm -o [name].out
 * 
 * Run command on linux terminal
 * ./[nombre].out
 **/

#include <stdio.h>
#include <stdlib.h>


/**
 * 	Signal parameters
 **/
#define	N		1000		// no. of samples

/**
 * 	Filter parameters
 **/
#define ORDER_2	2			// order of IIR filter


/**
 * Function signatures
 **/
float* read_signal(float *ap, const char *filename);
void create_signal_data_file_float(float *signal, const char *filename);
float filter_sample(float *x, float *y, float *b_coef, float *a_coef, float n_sample, int order);
float* iir_filter(float *filt_sig, float *input_sig, float *x, float *y, float *b, float *a, int order);


/*
* ====================================================================================================================
* M A I N 			M A I N 			M A I N 			M A I N 			M A I N 			M A I N 			
* ====================================================================================================================
*/
int main(void){

	// signals
	float *three_tones;
	float *filtered_signal;

	// coefficents
	float a[ORDER_2+1] = {1,-1.9513,0.9604};
	float b[ORDER_2+1] = {1,-1.9911,1};

	// buffers
	float x[ORDER_2+1] = {0};
	float y[ORDER_2+1] = {0};

	three_tones = calloc(N,sizeof(float));		// memory reserved for the signal to be filtered
	filtered_signal = calloc(N,sizeof(float));	// memory reserved for the filtered signal


	three_tones = read_signal(three_tones, "data_files/3_tones.dat");

	filtered_signal = iir_filter(filtered_signal, three_tones, x, y, b, a, ORDER_2);
	create_signal_data_file_float(filtered_signal, "data_files/filtered.dat");

	system("gnuplot -p 'iir.gp'");

	return 0;
}



/*
* ====================================================================================================================
* F U N C T I O N 	D E F I N I T I O N S 			F U N C T I O N 	D E F I N I T I O N S			
* ====================================================================================================================
*/


/**
 * 	@brief 
 * 	@param 	*ap pointer to array where signal is going to be saved
 * 	@param 	*filename contains signal data 
 * 	@retval	
 **/
float* read_signal(float *ap, const char *filename){
	FILE* file = fopen(filename, "r");
    int i;

    if (!file)
        perror("No se pudo abrir el archivo");  

    for (i=0; i<N; i++) 
        fscanf(file, "%f", &ap[i]);

    fclose(file);

    return ap;
}


/**
 *	@brief	Creates a .dat file with a signal points in float format
 * 	@param 	*signal input signal, of type double, to be saved
 *	@param 	*filename name + extension of the file to be created
 * 	@retval None
 **/
void create_signal_data_file_float(float *signal, const char *filename){
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
 * 	@brief	Filters one sample using N order IIR on it's Direct Form I (difference equations)
 * 	@param 	*x buffer for delays in input signal (must be initialized with 0s)
 * 	@param 	*y buffer for delays in output signal (must be initialized with 0s)
 * 	@param 	*b_coef array containing b coefficients of the filter
 * 	@param 	*a_coef array containing a coefficients of the filter
 * 	@param 	n_sample to be filtered
 * 	@param 	order of the filter
 **/
float filter_sample(float *x, float *y, float *b_coef, float *a_coef, float n_sample, int order){
	int i;

	// shifts previous values, delays in time
	//x[2] = x[1]
	//x[1] = x[0]

	//y[2] = y[1]
	//y[1] = y[0]
	for (i = order; i > 0; i--) {
		x[i] = x[i-1];
		y[i] = y[i-1];
	}

	x[0] = n_sample;
	
	// Difference equation of IIR filter
	y[0] = b_coef[0] * x[0];
	for (i = 1; i <= order; i++) {
		y[0] += b_coef[i] * x[i] - a_coef[i] * y[i];
	}
	
	// one filtered sample
	return y[0];
}


/**
 * 	@brief	Filters a signal using N order IIR
 * 	@param 	*filt_sig array to save th filtered signal
 * 	@param 	*input_sig array containing signal to be filtered
 * 	@param 	*x buffer for delays in input signal (must be initialized with 0s)
 * 	@param 	*y buffer for delays in output signal (must be initialized with 0s)
 *  @param 	*b array containing b coefficients of the filter
 * 	@param 	*a array containing a coefficients of the filter
 * 	@param 	order of the filter
 **/
float* iir_filter(float *filt_sig, float *input_sig, float *x, float *y, float *b, float *a, int order){
	int i;
	
	for (i=0; i<N; i++) {
		filt_sig[i] = filter_sample(x, y, b, a, input_sig[i], order);
	}
	
	return filt_sig;
}