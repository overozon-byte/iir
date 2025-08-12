#include <stdio.h>
#include <stdlib.h>
#define NCoef 2


double y_g;
int j;

//Fs = 44100
double iir(double NewSample, int size) {
    double ACoef[NCoef+1] = {
        //Fc = 3000 Hz
        0.03478271538033044600,
        0.06956543076066089300,
        0.03478271538033044600

    };

    double BCoef[NCoef+1] = {
        //Fc = 3000 Hz
        1.00000000000000000000,
        -1.40750534391514300000,
        0.54664949368209625000
    };

    double y = 0;
    double x = 0;

    if (j == 0) {
        x = NewSample;
        y = ACoef[0] * x;
        y_g = y;
        for (int i = 1; i < NCoef +1; i++) {
            y = ACoef[i] * x - BCoef[i] * y_g;
            y_g = y;
        }
        j++;
    }
    else {
        x = NewSample;
        y = ACoef[0] * x + BCoef[0] * y_g;
        y_g = y;
        for (int i = 1; i < NCoef + 1; i++) {
            y = ACoef[i] * x - BCoef[i] * y_g;
            y_g = y;
        }
    }
    return y;
}


int main(void)
{
    FILE* fp_r; // for read
    FILE* fp_w; // for write
    int size = 0;
    char need_bytes[44];


    if ((fp_r = fopen("E:\\digital_fitler_lp\\audio\\mono.wav", "r+b")) == NULL) {
        printf("Cannot open file for read.\n");
        return (1);
    }

    if ((fp_w = fopen("E:\\digital_fitler_lp\\audio\\mono_vs.wav", "w+b")) == NULL) {
        printf("Cannot open file for write.\n");
        return (1);
    }

    fseek(fp_r, 0L, SEEK_END); //To the end of file
    size = ftell(fp_r); //Take size 
    fseek(fp_r, 0L, SEEK_SET); //To the begin of file

    //Read 44 bytes from mono(.wav)
    fread(need_bytes, sizeof(char), 44, fp_r);
    fwrite(need_bytes, sizeof(char), 44, fp_w);
    printf("%s", need_bytes);
    
    //Input buffer for dynamic memory
    short* inputbuffer = (short*)malloc(size * sizeof(short));
    if (inputbuffer == NULL) {
        printf("Memory allocation for inputbuffer failed!\n");
        return 1;
    }
    //Output buffer for dynamic memory
    short* outputbuffer = (short*)malloc(size * sizeof(short));
    if (outputbuffer == NULL) {
        printf("Memory allocation for outputbuffer failed!\n");
        return 1;
    }
    //Read (size of mono) - 44 bytes in dynamic memory 
    fread(inputbuffer, sizeof(short), size, fp_r);

    double* mi = (double*)malloc(size * sizeof(double));
    if (mi == NULL) {
        printf("Memory allocation for mi failed!\n");
        return 1;
    }
    
    //From short to double
    for (int i = 0; i < (size-1); i++) {
        mi[i] = (double)inputbuffer[i];
    }

    double outputbuffer_o = 0;
    for (int i = 0; i < (size-1); i++) {
        outputbuffer[i] = (short)(iir(mi[i],size));
        outputbuffer_o = outputbuffer[i];
        if (outputbuffer[i] >= 32767) { // per on sample 16 bit
            outputbuffer[i] = 32767;
        }
        else {
            if (outputbuffer[i] <= (-32768)) {
                outputbuffer[i] = (-32768);
            }
            else {
                outputbuffer[i] = outputbuffer_o;
            }
        }
    }
   
    //Wirte in second file mono_vs(.wav)
    fwrite(outputbuffer, sizeof(char), size, fp_w);

    free(inputbuffer);
    free(outputbuffer);
    free(mi);
   
    fclose(fp_r);
    fclose(fp_w);


    return (0);
}
