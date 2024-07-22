#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIG_LENGTH 320

extern double _320_pts_ecg_IMX[SIG_LENGTH];
extern double _320_pts_ecg_REX[SIG_LENGTH];

double Output_Mag[SIG_LENGTH];
double Output_Phase[SIG_LENGTH];

//prototype
void rect_to_polar_conversion(double *sig_src_rex, double *sig_src_imx, double *sig_out_mag, double *sig_out_phase, int sig_length);

int main()
{
    FILE *input_rex, *input_imx, *output_mag, *output_phase;

    rect_to_polar_conversion((double *) &_320_pts_ecg_REX[0], (double *) &_320_pts_ecg_IMX[0], (double *) &Output_Mag[0], (double *) &Output_Phase[0], (int) SIG_LENGTH);

    input_rex = fopen("input_rex.dat", "w");
    input_imx = fopen("input_imx.dat", "w");
    output_mag = fopen("output_mag.dat", "w");
    output_phase = fopen("output_phase.dat", "w");

    for(int i = 0; i < SIG_LENGTH; i++)
    {
        fprintf(input_rex, "\n%f", _320_pts_ecg_REX[i]);
        fprintf(input_imx, "\n%f", _320_pts_ecg_IMX[i]);
        fprintf(output_mag, "\n%f", Output_Mag[i]);
        fprintf(output_phase, "\n%f", Output_Phase[i]);
    }

    return 0;
}

/*****************************************************
@param: sig_src_rex is the real signal array
@param: sig_src_imx is the imaginary signal array
@param: sig_out_mag is the magnitude array
@param: sig_out_phase is the output phase array
@param: sig_length is the length of signal

@description: converting rectangular notation to polar notation
*****************************************************/

void rect_to_polar_conversion(double *sig_src_rex, double *sig_src_imx, double *sig_out_mag, double *sig_out_phase, int sig_length)
{
    double PI = 3.14159265358979f;
    int k;

    for(k = 0; k < sig_length; k++)
    {
        sig_out_mag[k] = sqrt(powf(sig_src_rex[k], 2) + powf(sig_src_imx[k], 2));

        //when the real input part is 0
        if(sig_src_rex[k] == 0)
        {
            sig_src_rex[k] = pow(10, -20);
            sig_out_phase[k] = atan(sig_src_imx[k]/sig_src_rex[k]);

        }

        //when the real/imaginary input part is less than 0
        if((sig_src_rex[k] < 0) && (sig_src_imx[k] < 0))
        {
            sig_out_phase[k] = sig_out_phase[k] - PI;
        }

        //when the real input part is less than 0 and imaginary input part is greater than or equal to 0
        if((sig_src_rex[k] < 0) && (sig_src_imx[k] >= 0))
        {
            sig_out_phase[k] = sig_out_phase[k] + PI;
        }
    }
}
