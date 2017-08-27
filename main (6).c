#include <stdio.h>
#include <stdlib.h>
#define SIN_2PI_16 0.38268343236508978
#define SIN_4PI_16 0.707106781186547460
#define SIN_6PI_16 0.923879532511286740
#define C_P_S_2PI_16 1.30656296487637660
#define C_M_S_2PI_16 0.54119610014619690
#define C_P_S_6PI_16 1.3065629648763766
#define C_M_S_6PI_16 -0.54119610014619690

/* INPUT: float input[16], float output[16] */
/* OUTPUT: none */
/* EFFECTS:  Places the 16 point fft of input in output in a strange */
/* order using 10 real multiplies and 79 real adds. */
/* Re{F[0]}= out0 */
/* Im{F[0]}= 0 */
/* Re{F[1]}= out8 */
/* Im{F[1]}= out12 */
/* Re{F[2]}= out4 */
/* Im{F[2]}= -out6 */
/* Re{F[3]}= out11 */
/* Im{F[3]}= -out15 */
/* Re{F[4]}= out2 */
/* Im{F[4]}= -out3 */
/* Re{F[5]}= out10 */
/* Im{F[5]}= out14 */
/* Re{F[6]}= out5 */
/* Im{F[6]}= -out7 */
/* Re{F[7]}= out9 */
/* Im{F[7]}= -out13 */
/* Re{F[8]}= out1 */
/* Im{F[8]}=0 */
/* F[9] through F[15] can be found by using the formula */
/* Re{F[n]}=Re{F[(16-n)mod16]} and Im{F[n]}= -Im{F[(16-n)mod16]} */

/* Note using temporary variables to store intermediate computations */
/* in the butterflies might speed things up.  When the current version */
/* needs to compute a=a+b, and b=a-b, I do a=a+b followed by b=a-b-b.  */
/* So practically everything is done in place, but the number of adds */
/* can be reduced by doinc c=a+b followed by b=a-b. */

/* The algorithm behind this program is to find F[2k] and F[4k+1] */
/* seperately.  To find F[2k] we take the 8 point Real FFT of x[n]+x[n+8] */
/* for n from 0 to 7.  To find F[4k+1] we take the 4 point Complex FFT of */
/* exp(-2*pi*j*n/16)*{x[n] - x[n+8] + j(x[n+12]-x[n+4])} for n from 0 to 3.*/

float *R16SRFFT(float *input) {
    float *output=(float*) calloc(16,sizeof(float));
    float *out=(float*) calloc(16,sizeof(float));
    float temp;
    int i;
    for(i=0;i<8;i++)
        out[i]=input[i]+input[i+8]; // output[0 through 7] is the data that we
                                    // take the 8 point real FFT of.
    for(i=8;i<12;i++)
        out[i]=input[i-8]-input[i]; // inputs 8,9,10,11 are
                                    // the Real part of the
                                    // 4 point Complex FFT inputs.
    for(i=12;i<16;i++)
        out[i]=input[i]-input[i-8]; //outputs 12,13,14,15 are
                                    // the Imaginary pars of
                                    // the 4 point Complex FFT inputs.

  /*First we do the "twiddle factor" multiplies for the 4 point CFFT */
  /*Note that we use the following handy trick for doing a complex */
  /*multiply:  (e+jf)=(a+jb)*(c+jd) */
  /*   e=(a-b)*d + a*(c-d)   and    f=(a-b)*d + b*(c+d)  */

  /* C_M_S_2PI/16=cos(2pi/16)-sin(2pi/16) when replaced by macroexpansion */
  /* C_P_S_2PI/16=cos(2pi/16)+sin(2pi/16) when replaced by macroexpansion */
  /* (SIN_2PI_16)=sin(2pi/16) when replaced by macroexpansion */
    temp=(out[13]-out[9])*(SIN_2PI_16);
    out[9]=out[9]*(C_P_S_2PI_16)+temp;
    out[13]=out[13]*(C_M_S_2PI_16)+temp;

    out[14]*=(SIN_4PI_16);
    out[10]*=(SIN_4PI_16);
    out[14]+=-out[10];
    out[10]=out[14]+2*out[10];

    temp=(out[15]-out[11])*(SIN_6PI_16);
    out[11]=out[11]*(C_P_S_6PI_16)+temp;
    out[15]=out[15]*(C_M_S_6PI_16)+temp;

  /* The following are the first set of two point butterfiles */
  /* for the 4 point CFFT */

    out[8]+=out[10];
    out[10]=out[8]-2*out[10];

    out[12]+=out[14];
    out[14]=out[12]-2*out[14];

    out[9]+=out[11];
    out[11]=out[9]-2*out[11];

    out[13]+=out[15];
    out[15]=out[13]-2*out[15];
printf("AUX=%f\n",out[1]);
  /*The followin are the final set of two point butterflies */
    output[1]=out[8]+out[9];
    output[7]=out[8]-out[9];

    output[9]=out[12]+out[13];
    output[15]=out[13]-out[12];

    output[5]=out[10]+out[15];      /* implicit multiplies by */
    output[13]=out[14]-out[11];     /* a twiddle factor of -j */
    output[3]=out[10]-out[15];      /* implicit multiplies by */
    output[11]=-out[14]-out[11];            /* a twiddle factor of -j */


    /* What follows is the 8-point FFT of points output[0-7] */
    /* This 8-point FFT is basically a Decimation in Frequency FFT */
    /* where we take advantage of the fact that the initial data is real*/

    /* First set of 2-point butterflies */
    out[0]+=out[4];//
    out[4]=out[0]-2*out[4];
    out[1]+=out[5];
    out[5]=out[1]-2*out[5];
    out[2]+=out[6];//
    out[6]=out[2]-2*out[6];
    out[3]+=out[7];
    out[7]=out[3]-2*out[7];
    /* Computations to find X[0], X[4], X[6] */

    output[0]=out[0]+out[2];
    output[4]=out[0]-out[2];
    out[1]+=out[3];
    output[12]=2*out[3]-out[1];

    output[0]+=out[1];  /* Real Part of X[0] */
    output[8]=output[0]-2*out[1]; /*Real Part of X[4] */
    /* out2 = Real Part of X[6] */
    /* out3 = Imag Part of X[6] */

    /* Computations to find X[5], X[7] */

    out[5]*=SIN_4PI_16;
    out[7]*=SIN_4PI_16;
    out[5]+=-out[7];
    out[7]=out[5]+2*out[7];

    output[14]=out[6]-out[7]; /* Imag Part of X[5] */
    output[2]=out[5]+out[4]; /* Real Part of X[7] */
    output[6]=out[4]-out[5]; /*Real Part of X[5] */
    output[10]=-out[7]-out[6]; /* Imag Part of X[7] */
    for(i=0;i<16;i++)
        printf("out[%d]=%f\n",i,out[i]);
    for(i=0;i<16;i++)
        output[i]/=4;
return output;
}

void main(void) {
    int i;
    float *data=(float*) calloc(16,sizeof(float));
    float *output=(float*) calloc(16,sizeof(float));
    printf("\ntype 16 point input vector\n");
    scanf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6],&data[7],&data[8],&data[9],&data[10],&data[11],&data[12],&data[13],&data[14],&data[15]);
    output=R16SRFFT(data);
    printf("\nReal Part\t\tImaginary Part\n");
    printf("%f\n",output[3]);
    for(i=0;i<=8;i++)
        if(i==0 || i==8)
            printf("%.9f\t\t%.9f\n",output[i],0);
        else
            printf("%.9f\t\t%.9f\n",output[i],output[i+8]);
    for(i=7;i>=1;i--)
        printf("%.9f\t\t%.9f\n",output[i],-output[i+8]);
}
