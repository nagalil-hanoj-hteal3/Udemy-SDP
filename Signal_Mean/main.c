#include <stdio.h>
#include <stdlib.h>

# define SIG_LENGTH 320

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];
double calculate_signal_mean(double* sig_src_arr, int sig_legnth);

double MEAN;

int main()
{
    // include an '&' to get the address of the signal array
    MEAN = calculate_signal_mean(&InputSignal_f32_1kHz_15kHz[0], SIG_LENGTH);
    printf("\nMean of our waveforms equals: %f\n\n", MEAN);
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
