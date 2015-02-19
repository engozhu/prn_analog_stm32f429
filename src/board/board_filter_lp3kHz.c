/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Bessel
Filter order: 2
Sampling Frequency: 10 KHz
Cut Frequency: 3.000000 KHz
Coefficents Quantization: float

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = -0.169451 + j -0.260758
z = -0.169451 + j 0.260758
***************************************************************/
#include "board_filter_lp3kHz.h"

#define NCoef 2
float board_filter_lp3kHz_iir(float NewSample) {
    float ACoef[NCoef+1] = {
        0.35890242532894062000,
        0.71780485065788124000,
        0.35890242532894062000
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        0.33890147866868925000,
        0.09670822264707343300
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}
