/*
 
 This program simulates the subject's responses
 in a n-AFC contrast detection task following a Weibull
 psychometric function.

 Berenice López-Casal
 March, 2016
 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define STEP 0.05 // Any arbitrarily small number will do
#define INITIAL 0 // First value of stimulus intensity

#define NALT 2 // Number of alternatives
#define SD 0.3
#define MEAN 0.5
#define PL 0.05
#define PINUM 0.75
#define D 0.01
#define PG 0.5

#define N 100 // Each stimulus is presented N times
#define LENGTH_X 21 // Any arbitrarily large number will do
#define M 20 // M stimuli are presented throughout the whole experiment

void seq (double x[*], int j, int length_x);
void weibull(double x[*], double prob[*], int i, int length_x);
void runif (int j, int i, int m, double ** answ);
void generate_responses (int j, int i, int m, double ** answ, double prob2[m], double ** response);
void calc_proportions(int j, int i, int m, double ** response, double mean[*]);

int main(void) {
    
                /* Define variables */
    int j;
    int i;
    
    double  mean[M];
    double  x[LENGTH_X]; // array for the x axis (i.e., stimulus contrast) for the Weibull function
            x[0] = INITIAL;
    double  prob[LENGTH_X]; // array for the y axis (i.e., probability(correct)) for the Weibull function
    double  prob2[M];
    double  x2[M]; // x axis for the observer's answers
            x2[0] = INITIAL;
    
    double ** answ; // answ[N][m] will hold N*m random numbers from 0 to 1
        answ = (double **)malloc(sizeof(double *)*N);
        for(i=0; i < N; i++) {
            answ[i] = (double *)malloc(sizeof(double)*M);
    }
    double ** response; // response[N][m] will hold 0s and 1s depending on the subject's responses
        response = (double **)malloc(sizeof(double *)*N);
        for(i=0; i < N; i++) {
            response[i] = (double *)malloc(sizeof(double)*M);
    }

                /* Calculate and print the psychometric function */
    
    seq(x, j, LENGTH_X);
    weibull(x, prob, i, LENGTH_X);
    
    printf("The following table shows the values\nfor the psychometric (Weibull) function governing\nthe subject's responses:\n\n");
    printf("\tCONTRAST\tP(CORRECT)\n");
    printf("---------------------------\n");
    for(i = 0; i < LENGTH_X; i++){
        printf("%d\t%f\t%f\n", i+1, x[i], prob[i]);
    }
    printf("---------------------------\n\n");
    
                /* Run the experiment and print the results */
    
    seq(x2, j, M);
    weibull(x2, prob2, i, M);
    runif(j, i, M, answ);
    generate_responses(j, i, M, answ, prob2, response);
    calc_proportions(j, i, M, response, mean);

    printf("The following table shows the subject's\nresponses in a %d-AFC detection task:\n\n", NALT);
    printf("\tCONTRAST\tP(CORRECT)\n");
    printf("---------------------------\n");
    for (i=0; i < M; i++) {
        printf("%d\t%f\t%f\n", i+1, x2[i], mean[i]);
    }
    printf("---------------------------\n\n");
    
                /* Free dynamically allocated chunks of memory */
    
    for(i = 0; i < N; i++)
        free(answ[i]);
    free(answ);
     
     for(i = 0; i < N; i++)
            free(response[i]);
     free(response);

    return 0;
    
}

void seq (double x[], int j, int length_x) {
    for (j = 1; j < length_x; j++)	{
        x[j] = x[j-1] + STEP;
    }
}

void weibull (double x[], double prob[], int i, int length_x) {
    
    double first;
    double second;
    double b;
    double um;
    double a;
    double d2;
    
    for(i = 0; i < length_x; i++){
        first = log(D / (1-PL-PG) );
        second = log( ( (1-PL-PG-D) / (1-PL-PG) ) );
        b = (1/SD) * log10(first/second);
        um = log( ( (1-PL-PG) / (1-PL-PINUM) ) );
        a = MEAN * pow(um, (-1/b) );
        d2 = -pow( (x[i]/a) , b);
        prob[i] = 1-PL-(1-PL-PG) * exp(d2);
    }
    
}

void runif (int j, int i, int m, double ** answ) {
    for (j = 0; j < N; j++) {
        for(i=0; i < m; i++) {
        srand((double) random());
        answ[j][i] = ((double) rand() / (RAND_MAX));
        }
    }
}

 void generate_responses(int j, int i, int m, double ** answ, double prob2[], double ** response) {
 for (j = 0; j < N; j++) {
        for(i=0; i < m; i++) {
            if (answ[j][i] < prob2[i])   {
                response[j][i] = 1;
            }
            else {
                response[j][i] = 0;
            }
        }
    }
 }

void calc_proportions(int j, int i, int m, double ** response, double mean[m]) {
    
    double sum[m];
    
    for(i = 0; i < m; i++) {
        for (j = 0; j < N; j++){
            sum[i] = response[j][i] +  sum[i];
                }
        mean[i] = sum[i] / N;
    }
}
