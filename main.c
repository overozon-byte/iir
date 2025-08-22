#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NCoef 2

//__________________________________________________________________________________________________________________________________________________________________________________________________________________________//
//int main(void)
//{
// From double to int
//	// 1000 Hz
//	double a0 = 0.00461263667292077970;
//	double a1 = 0.00922527334584155940;
//	double a2 = 0.00461263667292077970;
//
//	double b0 = 1.00000000000000000000;
//	double b1 = -1.79909640948466820000;
//	double b2 = 0.81751240338475795000;
//
//	// 5000 Hz
//	/*double a0 = 0.08315985711886320100;
//	double a1 = 0.16631971423772640000;
//	double a2 = 0.08315985711886320100;
//
//	double b0 = 1.00000000000000000000;
//	double b1 = -1.03517120973894180000;
//	double b2 = 0.36781068945875101000;*/
//
//	// 3000 Hz
//	/*double a0 = 0.03478271538033044600;
//	double a1 = 0.06956543076066089300;
//	double a2 = 0.03478271538033044600;
//
//	double b0 = 1.00000000000000000000;
//	double b1 = -1.40750534391514300000;
//	double b2 = 0.54664949368209625000;*/
//
//	int a0_int = a0 * pow(2, 24);
//	int a1_int = a1 * pow(2, 24);
//	int a2_int = a2 * pow(2, 24);
//	int b0_int = b0 * pow(2, 24);
//	int b1_int = b1 * pow(2, 24);
//	int b2_int = b2 * pow(2, 24);
//
//	printf("a0_int = ");
//	printf("%d", a0_int);
//	printf("\n");
//
//	printf("a1_int = ");
//	printf("%d", a1_int);
//	printf("\n");
//
//	printf("a2_int = ");
//	printf("%d", a2_int);
//	printf("\n");
//
//	printf("b0_int = ");
//	printf("%d", b0_int);
//	printf("\n");
//
//	printf("b1_int = ");
//	printf("%d", b1_int);
//	printf("\n");
//
//	printf("b2_int = ");
//	printf("%d", b2_int);
//	printf("\n");
//
//	int p = 9;
//	printf("p = ");
//	printf("%d", p);
//	printf("\n");
//	p = p << 2;
//	printf("p = ");
//	printf("%d", p);
//	printf("\n");
//
//}
//__________________________________________________________________________________________________________________________________________________________________________________________________________________________//
// int
short signed y_g;

//Fs = 44100
short iir(short NewSample) {
    long long signed ACoef[NCoef+1] = {
        //Fc = 3000 Hz
        583557, // 0.03478271538033044600 * 2^24
        1167114, // 0.06956543076066089300 * 2^24
        583557// 0.03478271538033044600 * 2^24

    };

    long long signed BCoef[NCoef+1] = {
        //Fc = 3000 Hz
        16777216,  // 1.00000000000000000000 * 2^24
        (-23614021), // -1.40750534391514300000 * 2^24
        9171256  // 0.54664949368209625000 * 2^24
    };

    long long signed y = 0;
    short signed x = 0;
    short signed y_out = 0;

    x = NewSample;
    //y_g = y;
    for (int i = 0; i < NCoef + 1; i++) {
        y = ACoef[i] * x - BCoef[i] * y_g;
        if (i == 0) {
            y = y >> 25;
        }
        if (i == 1) {
            y = y >> 23;
        }
        if (i == NCoef) {
            y = y >> 24;
        }
       // y = y >> 24;
        y_g = (short)y;
    }

    y_out = y_g;
    return y_out;
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

    if ((fp_w = fopen("E:\\digital_fitler_lp\\audio\\mono_vs_int.wav", "w+b")) == NULL) {
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

    size = (size-44) / 2;
    //Read (size of mono) - 44 bytes in dynamic memory 
    fread(inputbuffer, sizeof(short), size, fp_r);

    //int j = 0;
    //while (feof(fp_r)) {
    //    if (!feof(fp_r)) {

    //    }
    //    else {
    //        outputbuffer[j] = iir(inputbuffer[j]);
    //    }
    //    j++;
    //}

    for (int i = 0; i < (size); i++) {
           outputbuffer[i] = iir(inputbuffer[i]);
    }
   
    //Wirte in second file mono_vs(.wav)
    fwrite(outputbuffer, sizeof(short), size, fp_w);

    free(inputbuffer);
    free(outputbuffer);
   
    fclose(fp_r);
    fclose(fp_w);


    return (0);
}
//__________________________________________________________________________________________________________________________________________________________________________________________________________________________//
//// double
//double y_g;
//int j;
//
////Fs = 44100
//double iir(double NewSample, int size) {
//    double ACoef[NCoef + 1] = {
//        //Fc = 3000 Hz
//        0.03478271538033044600,
//        0.06956543076066089300,
//        0.03478271538033044600
//
//    };
//
//    double BCoef[NCoef + 1] = {
//        //Fc = 3000 Hz
//        1.00000000000000000000,
//        -1.40750534391514300000,
//        0.54664949368209625000
//    };
//
//    double y = 0;
//    double x = 0;
//
//    if (j == 0) {
//        x = NewSample;
//        y = ACoef[0] * x;
//        y_g = y;
//        for (int i = 1; i < NCoef + 1; i++) {
//            y = ACoef[i] * x - BCoef[i] * y_g;
//            y_g = y;
//        }
//        j++;
//    }
//    else {
//        x = NewSample;
//        y = ACoef[0] * x + BCoef[0] * y_g;
//        y_g = y;
//        for (int i = 1; i < NCoef + 1; i++) {
//            y = ACoef[i] * x - BCoef[i] * y_g;
//            y_g = y;
//        }
//    }
//    return y;
//}
//
//
//int main(void)
//{
//    FILE* fp_r; // for read
//    FILE* fp_w; // for write
//    int size = 0;
//    char need_bytes[44];
//
//
//    if ((fp_r = fopen("E:\\digital_fitler_lp\\audio\\mono.wav", "r+b")) == NULL) {
//        printf("Cannot open file for read.\n");
//        return (1);
//    }
//
//    if ((fp_w = fopen("E:\\digital_fitler_lp\\audio\\mono_vs.wav", "w+b")) == NULL) {
//        printf("Cannot open file for write.\n");
//        return (1);
//    }
//
//    fseek(fp_r, 0L, SEEK_END); //To the end of file
//    size = ftell(fp_r); //Take size 
//    fseek(fp_r, 0L, SEEK_SET); //To the begin of file
//
//    //Read 44 bytes from mono(.wav)
//    fread(need_bytes, sizeof(char), 44, fp_r);
//    fwrite(need_bytes, sizeof(char), 44, fp_w);
//    printf("%s", need_bytes);
//
//    //Input buffer for dynamic memory
//    short* inputbuffer = (short*)malloc(size * sizeof(short));
//    if (inputbuffer == NULL) {
//        printf("Memory allocation for inputbuffer failed!\n");
//        return 1;
//    }
//    //Output buffer for dynamic memory
//    short* outputbuffer = (short*)malloc(size * sizeof(short));
//    if (outputbuffer == NULL) {
//        printf("Memory allocation for outputbuffer failed!\n");
//        return 1;
//    }
//    //Read (size of mono) - 44 bytes in dynamic memory 
//    fread(inputbuffer, sizeof(short), size, fp_r);
//
//    double* mi = (double*)malloc(size * sizeof(double));
//    if (mi == NULL) {
//        printf("Memory allocation for mi failed!\n");
//        return 1;
//    }
//
//    //From short to double
//    for (int i = 0; i < (size - 1); i++) {
//        mi[i] = (double)inputbuffer[i];
//    }
//
//    double outputbuffer_o = 0;
//    for (int i = 0; i < (size - 1); i++) {
//        outputbuffer[i] = (short)(iir(mi[i], size));
//        outputbuffer_o = outputbuffer[i];
//        if (outputbuffer[i] >= 32767) { // per on sample 16 bit
//            outputbuffer[i] = 32767;
//        }
//        else {
//            if (outputbuffer[i] <= (-32768)) {
//                outputbuffer[i] = (-32768);
//            }
//            else {
//                outputbuffer[i] = outputbuffer_o;
//            }
//        }
//    }
//
//    //Wirte in second file mono_vs(.wav)
//    fwrite(outputbuffer, sizeof(char), size, fp_w);
//
//    free(inputbuffer);
//    free(outputbuffer);
//    free(mi);
//
//    fclose(fp_r);
//    fclose(fp_w);
//
//
//    return (0);
//}
