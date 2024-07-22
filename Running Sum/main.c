#include <stdio.h>
#include <stdlib.h>
#include <math.h> //added for pow()

# define SIG_LENGTH 320

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];
double Output_signl[SIG_LENGTH];

//prototype
void calculate_running_sum(double *sig_src_arr, double *sig_dest_arr, int sig_length);

int main()
{
    FILE *input_sig_fptr, *output_sig_fptr;

    calculate_running_sum((double*) &InputSignal_f32_1kHz_15kHz[0], (double*) &Output_signl[0], (int) SIG_LENGTH);

    // creates these files manually
    input_sig_fptr = fopen("input_signal.dat", "w");
    output_sig_fptr = fopen("output_signal.dat", "w");

    /***********************************************************************************/

    // loop to write down 320 rows of input signal, and written vertically
    for(int i = 0; i < SIG_LENGTH; i++) {
        fprintf(input_sig_fptr, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
    }

    // to close the input_signal.dat file
    fclose(input_sig_fptr);

    /***********************************************************************************/

    // loop to write down 349 rows of impulse response, and written vertically
    for(int i = 0; i < (SIG_LENGTH); i++) {
        fprintf(output_sig_fptr, "\n%f", Output_signl[i]);
    }

    // to close the impulse_response.dat file
    fclose(output_sig_fptr);

    return 0;
}

/***************************************************************************************
@param: sig_src_arr is the signal source array
@param: sig_dest_arr is the signal destination array
@param: sig_length is the signal length

@about: calculate the running sum of the signal source array with the signal destination array
***************************************************************************************/

void calculate_running_sum(double *sig_src_arr, double *sig_dest_arr, int sig_length)
{
    for(int i = 0; i < sig_length; i++) {
        sig_dest_arr[i] = sig_dest_arr[i - 1] + sig_src_arr[i];
    }
}
