#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 320

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];
double Output_REX[SIG_LENGTH/2];
double Output_IMX[SIG_LENGTH/2];
double Output_MAG[SIG_LENGTH/2];
double Output_IGFT[SIG_LENGTH];

//prototype
void calc_sig_dft(double *sig_src_arr, double *sig_dest_rex, double *sig_dest_imx_arr, int sig_length);
void calc_idft(double *idft_out_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int idft_length);
void get_dft_output_mag(double *sig_dest_mag_arr);

int main()
{
    FILE *fptr, *fptr2, *fptr3, *fptr4;
    calc_sig_dft((double *) &InputSignal_f32_1kHz_15kHz[0], (double *) &Output_REX[0], (double *) &Output_IMX[0], (int) SIG_LENGTH);
    calc_idft((double *) &Output_IGFT[0], (double *) &Output_REX[0], (double *) &Output_IMX[0], (int) SIG_LENGTH);
    /*************************************************************/

    fptr = fopen("input_signal.dat", "w");
    fptr4 = fopen("output_idft.dat", "w");

    for(int i = 0; i < SIG_LENGTH; i++) {
        fprintf(fptr, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
        fprintf(fptr4, "\n%f", Output_IGFT[i]);
    }

    /*************************************************************/

    fptr2 = fopen("output_rex.dat", "w");
    fptr3 = fopen("output_imx.dat", "w");

    for(int i = 0; i < SIG_LENGTH; i++) {
        fprintf(fptr2, "\n%f", Output_REX[i]);
        fprintf(fptr3, "\n%f", Output_IMX[i]);
    }

    /**************************************************************/

    fclose(fptr);
    fclose(fptr2);
    fclose(fptr3);
    fclose(fptr4);

    return 0;
}

/***************************************************************
@param: sig_src_arr is signal source array
@param: sig_dest_rex is signal destination real part array
@param: sig_dest_imx is signal imaginary part array
@param: the size of the signal

@description: calculate signal discrete fourier transform algorithm
***************************************************************/

void calc_sig_dft(double *sig_src_arr, double *sig_dest_rex, double *sig_dest_imx_arr, int sig_length)
{
    int i, j, k;
    double pi = 3.14159265359;

    // we are dividing by 2 since the time domain is passed through the dft,
    // it will produce the frequency domain signal for both real and imaginary
    for(j = 0; j < sig_length / 2; j++)
    {
        sig_dest_rex[j] = 0;
        sig_dest_imx_arr[j] = 0;
    }

    for(k = 0; k < sig_length / 2; k++)
    {
        for(i = 0; i < sig_length; i++)
        {
            sig_dest_rex[k] = sig_dest_rex[k] + sig_src_arr[i] * cos(2 * pi * k * i/sig_length);
            sig_dest_imx_arr[k] = sig_dest_imx_arr[k] - sig_src_arr[i] * sin(2 * pi * k * i/sig_length);
        }
    }
}

void get_dft_output_mag(double *sig_dest_mag_arr)
{
    int k;
    for(k = 0; k < SIG_LENGTH/2; k++)
    {
        sig_dest_mag_arr[k] = sqrt(pow((Output_REX[k]), 2) + pow(Output_IMX[k], 2));
    }
}

/***************************************************************
@param: idft_out_arr is inverse discrete fourier transform signal source array
@param: sig_src_rex_arr is signal destination real part array
@param: sig_src_imx_arr is signal imaginary part array
@param: idft_length is the size of the idft

@description: calculate signal inverse discrete fourier transform algorithm
***************************************************************/

void calc_idft(double *idft_out_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int idft_length)
{
    int i, k;
    double PI = 3.14159265359;

    for(k = 0; k < idft_length/2; k++)
    {
        sig_src_rex_arr[k] = sig_src_rex_arr[k] / (idft_length / 2);
        sig_src_imx_arr[k] = -sig_src_imx_arr[k] / (idft_length / 2);
    }

    sig_src_rex_arr[0] = sig_src_rex_arr[0] / (2);
    sig_src_imx_arr[0] = -sig_src_imx_arr[0] / (2);

    for(i = 0; i < idft_length; i++)
    {
        idft_out_arr[i] = 0;
    }
    for(k = 0; k < idft_length/2; k++)
    {
        for(i = 0; i < idft_length; i++)
        {
            idft_out_arr[i] = idft_out_arr[i] + sig_src_rex_arr[k] * cos(2 * PI * k * i / idft_length);
            idft_out_arr[i] = idft_out_arr[i] + sig_src_imx_arr[k] * sin(2 * PI * k * i / idft_length);
        }
    }
}
