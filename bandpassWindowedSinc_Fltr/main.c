#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 320
#define KERNEL_LENGTH 29

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];

double OutputSignal[SIG_LENGTH - KERNEL_LENGTH];
double OutputFilter[KERNEL_LENGTH];

double state_low_cutoff_buff[KERNEL_LENGTH];
double state_up_cutoff_buff[KERNEL_LENGTH];

/***
10kHz cutoff --> (10 / 24) * 0.5 = 0.2

@note based on sampling frequency
lower cutoff --> (0.1 / 24) * 0.5 = 0.002
upper cutoff --> (5.28 / 24) * 0.5 = 0.11
***/



//prototype
void bandpassWindowedSincFltr(double *lower_cutoff_state_buff, double *upper_cutoff_state_buff, double *fltr_kernel_dest_arr, double lower_cutoff, double upper_cutoff, int filter_length, double *sig_src_arr, double *sig_dest_arr, int sig_length);

int main()
{
    FILE *fptr, *fptr2, *fptr3;

    bandpassWindowedSincFltr((double *) &state_low_cutoff_buff[0], (double *) &state_up_cutoff_buff[0], (double *) &OutputFilter[0], (double) 0.002, (double) 0.11, (int) KERNEL_LENGTH, (double *) &InputSignal_f32_1kHz_15kHz[0], (double *) &OutputSignal[0], (int) SIG_LENGTH);

    fptr = fopen("input_signal.dat", "w");
    fptr2 = fopen("output_signal.dat", "w");
    fptr3 = fopen("output_kernel.dat", "w");

    /*******************************************************/

    for(int i = 0; i < SIG_LENGTH; i++)
    {
        fprintf(fptr, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
        if(i < KERNEL_LENGTH)
            fprintf(fptr3, "\n%f", OutputFilter[i]);
        if(i > KERNEL_LENGTH)
            fprintf(fptr2, "\n%f", OutputSignal[i]);
    }

    /*******************************************************/

    fclose(fptr);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}

/****************************************
@param lower_cutoff_state_buff is pointer to the lower cutoff state buffer
@param upper_cutoff_state_buff is pointer to the higher cutoff state buffer
@param fltr_kernel_dest_arr is pointer for filter kernel destination
@param lower_cutoff is the number for lower cutoff
@param upper_cutoff is the number of upper cutoff
@param filter_length is the length of the kernel
@param sig_src_arr is the signal source array
@param sig_length is the lenght of the signal

@description calculate the bandpass windowed Sync Filter
****************************************/

void bandpassWindowedSincFltr(double *lower_cutoff_state_buff, double *upper_cutoff_state_buff, double *fltr_kernel_dest_arr, double lower_cutoff, double upper_cutoff, int filter_length, double *sig_src_arr, double *sig_dest_arr, int sig_length)
{
    //calculate the first low-pass filter kernel
    for(int i = 0; i < filter_length; i++)
    {
        if((i - filter_length / 2) == 0)
            lower_cutoff_state_buff[i] = 2 * M_PI * lower_cutoff;
        if((i - filter_length / 2) != 0)
        {
            lower_cutoff_state_buff[i] = sin(2 * M_PI * lower_cutoff * (i - filter_length / 2)) / (i - filter_length / 2);
            lower_cutoff_state_buff[i] = lower_cutoff_state_buff[i] * (0.42 - 0.5 * cos(2 * M_PI * i / filter_length) + 0.08 * cos(4 * M_PI * i / filter_length));
        }
    }

    //calculate the second low-pass filter kernel
    for(int i = 0; i < filter_length; i++)
    {
        if((i - filter_length / 2) == 0)
            upper_cutoff_state_buff[i] = 2 * M_PI * upper_cutoff;
        if((i - filter_length / 2) != 0)
        {
            upper_cutoff_state_buff[i] = sin(2 * M_PI * upper_cutoff * (i - filter_length / 2)) / (i - filter_length / 2);
            upper_cutoff_state_buff[i] = upper_cutoff_state_buff[i] * (0.42 - 0.5 * cos(2 * M_PI * i / filter_length) + 0.08 * cos(4 * M_PI * i / filter_length));
        }
    }

    /********************************************************************************************/

    //calculate low-pass filter to a high-pass filter using Spectral Inversion
    for(int i = 0; i < filter_length; i++)
        upper_cutoff_state_buff[i] = -(upper_cutoff_state_buff[i]);
    upper_cutoff_state_buff[filter_length / 2] = upper_cutoff_state_buff[filter_length / 2] + 1;

    //add low-pass filter kernel to high-pass filter kernel to form a band-reject filter kernel
    for(int i = 0; i < filter_length; i++)
        fltr_kernel_dest_arr[i] = lower_cutoff_state_buff[i] + upper_cutoff_state_buff[i];

    //change band-reject filter into bandpass filter using spectral inversion
    for(int i = 0; i < filter_length; i++)
        fltr_kernel_dest_arr[i] = -(fltr_kernel_dest_arr[i]);
    fltr_kernel_dest_arr[filter_length / 2] = fltr_kernel_dest_arr[filter_length / 2] + 1;

    //convolve the input signal and the filter kernel
    for(int j = filter_length; j < sig_length; j++)
    {
        sig_dest_arr[j] = 0;
        for(int i = 0; i < filter_length; i++)
            sig_dest_arr[j] = sig_dest_arr[j] + sig_src_arr[j - i] * fltr_kernel_dest_arr[i];
    }
}
