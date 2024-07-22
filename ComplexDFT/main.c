#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 501

extern double _501pts_20Hz_sig_imx[SIG_LENGTH];
extern double _501pts_20Hz_sig_rex[SIG_LENGTH];

double Output_Rex[SIG_LENGTH];
double Output_Imx[SIG_LENGTH];

//prototype
void complex_dft(double *sig_src_time_domain_rex, double *sig_src_time_domain_imx, double *sig_dest_freq_domain_rex, double *sig_dest_freq_domain_imx, int sig_length);

int main()
{
    FILE *input_rex, *input_imx, *output_rex, *output_imx;

    complex_dft((double *) &_501pts_20Hz_sig_rex[0], (double *) &_501pts_20Hz_sig_imx[0], (double *) &Output_Rex[0], (double *) &Output_Imx[0], (int) SIG_LENGTH);

    input_rex = fopen("input_rex.dat", "w");
    input_imx = fopen("input_imx.dat", "w");
    output_rex = fopen("output_rex.dat", "w");
    output_imx = fopen("output_imx.dat", "w");

    for(int i = 0; i < SIG_LENGTH; i++)
    {
        fprintf(input_rex, "\n%f", _501pts_20Hz_sig_rex[i]);
        fprintf(input_imx, "\n%f", _501pts_20Hz_sig_imx[i]);
        fprintf(output_rex, "\n%f", Output_Rex[i]);
        fprintf(output_imx, "\n%f", Output_Imx[i]);
    }

    return 0;
}

/*****************************************************
@param: sig_src_time_domain_rex is the real signal source time domain array
@param: sig_src_time_domain_imx is the imaginary signal source time domain array
@param: sig_dest_freq_domain_rex is the real signal destination frequency domain array
@param: sig_dest_freq_domain_imx is the imaginary signal destination frequency domain array
@param: sig_length is the length of signal

@description: calculating the complex discrete fourier transform algorithm
*****************************************************/

void complex_dft(double *sig_src_time_domain_rex, double *sig_src_time_domain_imx, double *sig_dest_freq_domain_rex, double *sig_dest_freq_domain_imx, int sig_length)
{
    double real, imag, norm;

    norm = (double) 1 / (double) sig_length;

    for(int k = 0; k < sig_length-1;k++){
        for(int i = 0; i < sig_length-1; i++) {

            real =  cos(2 * PI * k * i / sig_length);
            imag = -sin(2 * PI * k * i / sig_length);

            sig_dest_freq_domain_rex[k] += norm*(sig_src_time_domain_rex[i]*real - sig_src_time_domain_imx[i]*imag);
            sig_dest_freq_domain_imx[k] += norm*(sig_src_time_domain_rex[i]*imag + sig_src_time_domain_imx[i]*real);
        }
    }
}
