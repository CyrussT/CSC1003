#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "nbstructs.h"

#define TRAININGSIZE 80 /* 80% of data used for training accuracy */
#define DATACOUNT 20    /* 20% of data used for testing accuracy */
#define FEATURESIZE 9
#define PI 3.142

void scanArray(features arr[]);
double mean(double arr[], int n);
double variance(double arr[], int n);
features traininginput[TRAININGSIZE], testinput[DATACOUNT];

int main()
{
    // create a temporary table in heap
    features *input = malloc(sizeof(features) * TRAININGSIZE + DATACOUNT);
    // store entire data into the input
    scanArray(input);
    // split first 80% of data into traininginput and remaining 20% into testinput
    memcpy(traininginput, input, TRAININGSIZE * sizeof(features));
    memcpy(testinput,&input[80], DATACOUNT * sizeof(features));
    // free temporary table from heap 
    free(input);
    return 0;
}

void scanArray(features arr[])
{
    FILE *fileptr;
    fileptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");

    if (fileptr == NULL)
    {
        printf("File could not be opened. \n");
        exit(1);
    }

    int i = 0; // used to loop through the struct, features
    while (EOF != fscanf(fileptr, "%f, %f, %d, %d, %d, %d, %f, %d, %f, %d", &arr[i].season, &arr[i].age, &arr[i].disease, &arr[i].accident, &arr[i].surgery, &arr[i].fever, &arr[i].alcohol, &arr[i].smoker, &arr[i].sittinghours, &arr[i].semendiagnosis))
    {
        printf("%f, %f, %d, %d, %d, %d, %f, %d, %f, %d\n", arr[i].season, arr[i].age, arr[i].disease, arr[i].accident, arr[i].surgery, arr[i].fever, arr[i].alcohol, arr[i].smoker, arr[i].sittinghours, arr[i].semendiagnosis);
        i++;
    }
}

double mean(double arr[], int n) //needs to run and check output
{
    double sum = 0; // sum of all elements
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    return sum / n; // return mean
}

double variance(double arr[], int n) // needs to run and check output
{
    double arr_mean = mean(arr, n);
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += pow(arr[i] - arr_mean, 2);
    }
    return sum / n;
}

double gaussian(double x, double mean, double variance) //needs to run and check output
{
    return (1 / (sqrt(2 * PI * variance))) * exp(-0.5 * pow(x - mean, 2) / (2 * variance));
}