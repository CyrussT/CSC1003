#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "nbstructs.h"

#define trainingsize 80 /* 80% of data used for training accuracy */
#define datacount 20 /* 20% of data used for testing accuracy */
#define featuresize 9
#define PI 3.142

double mean(double arr[], int n);
double variance(double arr[], int n);

int main()
{
    return 0;
}

double mean(double arr[], int n) //needs to run and check output
{
    double sum = 0; // sum of all elements
    for (int i=0; i<n; i++)
    {
        sum += arr[i];
    }
    return sum/n; // return mean
}

double variance(double arr[], int n) // needs to run and check output
{
    double arr_mean = mean(arr, n);
    double sum = 0;
    for (int i=0; i<n; i++)
    {
        sum += pow(arr[i] - arr_mean, 2);
    }
    return sum/n;
}

double gaussian(double x, double mean, double variance) //needs to run and check output
{
    return (1/(sqrt(2*PI*variance)))*exp(-0.5 * pow(x-mean, 2)/(2*variance));
}