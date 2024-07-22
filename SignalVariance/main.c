#include <stdio.h>
#include <stdlib.h>
#include <math.h> //added for pow()

# define SIG_LENGTH 320

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];

// prototypes of the functions used
double calculate_signal_mean(double* sig_src_arr, int sig_legnth);
double calculate_signal_variance(double* sig_src_arr, double sig_mean, int sig_legnth);

double MEAN;
double VARIANCE;

int main()
{
    // include an '&' to get the address of the signal array
    MEAN = calculate_signal_mean(&InputSignal_f32_1kHz_15kHz[0], SIG_LENGTH);
    printf("\nMean of our waveforms equals: %f\n\n", MEAN);

    VARIANCE = calculate_signal_variance(&InputSignal_f32_1kHz_15kHz[0],MEAN, SIG_LENGTH);
    printf("\nVariance of our waveforms equals: %f\n\n", VARIANCE);
    return 0;
}

/*
@param: sig_src_arr to locate the source of the array
@param: sig_length to obtain the length of the signal

@about: calculate the signal mean from the signal source array
to obtain the mean and return the result.
*/
double calculate_signal_mean(double* sig_src_arr, int sig_legnth)
{
    double _mean = 0.0;

    // loop to calculate the mean
    for(int i = 0; i < sig_legnth; i++)
    {
        // this is the total
        _mean = _mean + sig_src_arr[i];
    }

    _mean = _mean/(double)sig_legnth;

    return _mean;
}

/*
@param: sig_src_arr to locate the source of the array
@param: sig_mean to obtain the mean of the signal
@param: sig_length to obtain the length of the signal

@about: calculate the signal variance from the signal source array
with the mean and return the result.
*/
double calculate_signal_variance(double* sig_src_arr, double sig_mean, int sig_legnth)
{
    double _variance = 0.0;
    for(int i =0; i< sig_legnth; i++)
    {
        _variance = _variance + pow((sig_src_arr[i] - sig_mean), 2);
    }
    _variance = _variance / (sig_legnth - 1);
    return _variance;
}
