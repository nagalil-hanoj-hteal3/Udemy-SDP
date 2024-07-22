#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 320
#define KERNEL_LENGTH 29
/************
@note Cutoff frequency is normalized and must be between 0 and 0.5
      represents the nyquist frequency. In this example, the signal
      was sampled at 48kHz, therefore the nyquist is 24kHz.

      i.e. 24kHz --> 0.5, 10kHz --> 0.2: (10/24) x 0.5

************/

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];

double OutputSignal[SIG_LENGTH - KERNEL_LENGTH];
double OutputFilter[KERNEL_LENGTH];

//prototype
void lp_windowed_sinc_ftr(double *sig_src_arr, double *sig_dest_arr, double *fltr_kernel_dest_arr, double cutoff_freq, int filter_length, int input_sig_length);

int main()
{
    FILE *fptr, *fptr2, *fptr3;

    lp_windowed_sinc_ftr((double *) &InputSignal_f32_1kHz_15kHz[0], (double *) &OutputSignal[0], (double *) &OutputFilter[0], (double) 0.2, (int) KERNEL_LENGTH, (int) SIG_LENGTH);

    /*************************************************************************/

    fptr = fopen("output_signal.dat", "w");
    fptr2 = fopen("input_signal.dat", "w");
    fptr3 = fopen("output_kernel.dat", "w");

    /*************************************************************************/

    for(int i = 0; i < SIG_LENGTH; i++)
    {
        fprintf(fptr2, "\n%f", InputSignal_f32_1kHz_15kHz[i]);

        if(i >= KERNEL_LENGTH)
            fprintf(fptr, "\n%f", OutputSignal[i]);

        if(i <= KERNEL_LENGTH)
            fprintf(fptr3, "\n%f", OutputFilter[i]);

    }

    /*************************************************************************/

    fclose(fptr);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}

/************************************************************
@param: sig_src_arr is array of source signal
@param: sig_dest_arr is result of where its stored
@param: fltr_kernel_dest_arr is result to store the filter kernel
@param: cutoff_freq is cutoff frequency that is normalized
@param: filter_length is filter length
@param: input_sig_length is signal length

@description: Calculate the Low-pass Windowed-Sinc Filter Algorithm
************************************************************/

void lp_windowed_sinc_ftr(double *sig_src_arr, double *sig_dest_arr, double *fltr_kernel_dest_arr, double cutoff_freq, int filter_length, int input_sig_length)
{
    // calculate the lp filter kernel
    for(int i = 0; i < filter_length; i++)
    {
        if((i - filter_length / 2) == 0)
        {
            fltr_kernel_dest_arr[i] = 2 * M_PI * cutoff_freq;
        }
        if((i - filter_length / 2) != 0)
        {
            fltr_kernel_dest_arr[i] = sin(2 * M_PI * cutoff_freq * (i - filter_length / 2)) / (i - filter_length / 2);
            fltr_kernel_dest_arr[i] = fltr_kernel_dest_arr[i] * (0.54 - 0.46 * cos(2 * M_PI * i / filter_length));
        }
    }

    //convolve the input signal and filter kernel
    for(int j = filter_length; j < input_sig_length; j++)
    {
        sig_dest_arr[j] = 0;
        for(int i = 0; i < filter_length; i++)
        {
            sig_dest_arr[j] = sig_dest_arr[j] + sig_src_arr[j - i] * fltr_kernel_dest_arr[i];
        }
    }


}
