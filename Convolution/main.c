#include <stdio.h>
#include <stdlib.h>
#include <math.h> //added for pow()

# define SIG_LENGTH 320
# define IMP_RSP_LENGTH 29

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];
extern double Impulse_response[IMP_RSP_LENGTH];
double Output_signl[SIG_LENGTH + IMP_RSP_LENGTH];

//prototype
void convolution(double *sig_src_arr, double *sig_dest, double *imp_response_arr, int sig_src_length, int imp_response_length);

int main()
{
    FILE *input_sig_fptr, *imp_rsp_fptr, *output_sig_fptr;

    convolution((double*) &InputSignal_f32_1kHz_15kHz[0], (double*) &Output_signl[0], (double*) &Impulse_response[0], (int) SIG_LENGTH, (int) IMP_RSP_LENGTH);

    // creates these files manually
    input_sig_fptr = fopen("input_signal.dat", "w");
    imp_rsp_fptr = fopen("impulse_response.dat", "w");
    output_sig_fptr = fopen("output_signal.dat", "w");

    /***********************************************************************************/

    // loop to write down 320 rows of input signal, and written vertically
    for(int i = 0; i < SIG_LENGTH; i++) {
        fprintf(input_sig_fptr, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
    }

    // to close the input_signal.dat file
    fclose(input_sig_fptr);

    /***********************************************************************************/

    // loop to write down 29 rows of impulse response, and written vertically
    for(int i = 0; i < IMP_RSP_LENGTH; i++) {
        fprintf(imp_rsp_fptr, "\n%f", Impulse_response[i]);
    }

    // to close the impulse_response.dat file
    fclose(imp_rsp_fptr);

    /***********************************************************************************/

    // loop to write down 349 rows of impulse response, and written vertically
    for(int i = 0; i < (SIG_LENGTH + IMP_RSP_LENGTH); i++) {
        fprintf(output_sig_fptr, "\n%f", Output_signl[i]);
    }

    // to close the impulse_response.dat file
    fclose(output_sig_fptr);

    return 0;
}

/***********************************
@param: sig_src_arr is signal source array
@param: sig_dest is signal destination array
@param: imp_response_arr is impulse response array
@param: sig_src_length is signal source length
@param: imp_response_length is impulse response length
***********************************/

void convolution(double *sig_src_arr, double *sig_dest, double *imp_response_arr, int sig_src_length, int imp_response_length) {
    int i, j;

    for(i = 0; i < sig_src_length + imp_response_length; i++) {
        sig_dest[i] = 0;
    }

    for(i = 0; i < sig_src_length; i++)
    {
        for(j = 0; j < imp_response_length; j++)
        {
            // convolution sound equation
            sig_dest[i+j] = sig_dest[i+j] + sig_src_arr[i] * imp_response_arr[j];
        }
    }
}
