#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 640

//extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];
extern double _640_points_ecg_[SIG_LENGTH];

double Output_REX[SIG_LENGTH/2];
double Output_IMX[SIG_LENGTH/2];
double Output_MAG[SIG_LENGTH/2];
double Output_IDFT[SIG_LENGTH];

// Prototypes
void calc_sig_dft(double *sig_src_arr, double *sig_dest_rex, double *sig_dest_imx_arr, int sig_length);
void calc_idft(double *idft_out_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int idft_length);
void get_dft_output_mag(double *sig_dest_mag_arr);

int main()
{
    FILE *fptr, *fptr2, *fptr3, *fptr4, *fptr5;

    calc_sig_dft((double *) &_640_points_ecg_[0], (double *) &Output_REX[0], (double *) &Output_IMX[0], (int) SIG_LENGTH);
    get_dft_output_mag((double *) &Output_MAG[0]);
    calc_idft((double *) &Output_IDFT[0], (double *) &Output_REX[0], (double *) &Output_IMX[0], (int) SIG_LENGTH);

    // Open files for writing
    fptr = fopen("input_signal.dat", "w");
    fptr2 = fopen("output_rex.dat", "w");
    fptr3 = fopen("output_imx.dat", "w");
    fptr4 = fopen("output_idft.dat", "w");
    fptr5 = fopen("output_mag.dat", "w");

    // Write data to files
    for(int i = 0; i < SIG_LENGTH; i++) {
        fprintf(fptr, "\n%f", _640_points_ecg_[i]);
        fprintf(fptr4, "\n%f", Output_IDFT[i]);
    }

    for(int i = 0; i < SIG_LENGTH/2; i++) {
        fprintf(fptr2, "\n%f", Output_REX[i]);
        fprintf(fptr3, "\n%f", Output_IMX[i]);
        fprintf(fptr5, "\n%f", Output_MAG[i]);
    }

    // Close files
    fclose(fptr);
    fclose(fptr2);
    fclose(fptr3);
    fclose(fptr4);
    fclose(fptr5);

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

    for(j = 0; j < sig_length / 2; j++)
    {
        sig_dest_rex[j] = 0;
        sig_dest_imx_arr[j] = 0;
    }

    for(k = 0; k < sig_length / 2; k++)
    {
        for(i = 0; i < sig_length; i++)
        {
            sig_dest_rex[k] += sig_src_arr[i] * cos(2 * pi * k * i / sig_length);
            sig_dest_imx_arr[k] -= sig_src_arr[i] * sin(2 * pi * k * i / sig_length);
        }
    }
}

/***************************************************************
@param: sig_dest_mag_arr is signal destination magnitude array

@description: calculate and display the magnitude of the discrete fourier transform
***************************************************************/
void get_dft_output_mag(double *sig_dest_mag_arr)
{
    int k;
    for(k = 0; k < SIG_LENGTH/2; k++)
    {
        sig_dest_mag_arr[k] = sqrt(pow(Output_REX[k], 2) + pow(Output_IMX[k], 2));
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

    for(k = 0; k < idft_length / 2; k++)
    {
        sig_src_rex_arr[k] = sig_src_rex_arr[k] / (idft_length / 2);
        sig_src_imx_arr[k] = -sig_src_imx_arr[k] / (idft_length / 2);
    }

    sig_src_rex_arr[0] = sig_src_rex_arr[0] / 2;
    sig_src_imx_arr[0] = -sig_src_imx_arr[0] / 2;

    for(i = 0; i < idft_length; i++)
    {
        idft_out_arr[i] = 0;
    }

    for(k = 0; k < idft_length / 2; k++)
    {
        for(i = 0; i < idft_length; i++)
        {
            idft_out_arr[i] += sig_src_rex_arr[k] * cos(2 * PI * k * i / idft_length);
            idft_out_arr[i] += sig_src_imx_arr[k] * sin(2 * PI * k * i / idft_length);
        }
    }
}
