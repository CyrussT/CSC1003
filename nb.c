#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "nbstructs.h"

#define TRAININGSIZE 80 /* 80% of data used for training accuracy */
#define DATACOUNT 20    /* 20% of data used for testing accuracy */
#define FEATURESIZE 9
#define PI 3.14159

void scanArray(features arr[]);
double mean(double arr[], int n);
double variance(double arr[], int n);
void calculateProbability(cond_prob input[], int index_size);
features traininginput[TRAININGSIZE], testinput[DATACOUNT];

// Feature Variables
int normal_diagnosis, altered_diagnosis;

int main()
{
    // create a temporary table in heap
    features *input = malloc(sizeof(features) * (TRAININGSIZE + DATACOUNT));
    // store entire data into the input
    scanArray(input);
    // split first 80% of data into traininginput and remaining 20% into testinput
    memcpy(traininginput, input, TRAININGSIZE * sizeof(features));
    memcpy(testinput,&input[80], DATACOUNT * sizeof(features));
    // free temporary table from heap 
    free(input);

    // Calculating Prior Probability
    for (int i = 0; i < TRAININGSIZE; i++)
    {
        if (traininginput[i].semendiagnosis == 1)
        {
            altered_diagnosis++;
        }
        else if (traininginput[i].semendiagnosis == 0)
        {
            normal_diagnosis++;
        }
        else
        {
            printf("Error - unable to calculate Prior Probability\n");
            exit(1);
        }
    }

    printf("Normal: %i, Altered: %i \n", normal_diagnosis, altered_diagnosis); // Checked output and figure tallys.

    // Calculating Conditional Probability for Seasons of Analysis
    cond_prob season_prob[4] = {0, 0, 0}; 
    // Winter: -1, Spring: -0.33, Summer: 0.33, Fall: 1
    // Index : 0, 1, 2, 3

    for (int i = 0; i < TRAININGSIZE; i++)
    {
        float season = traininginput[i].season;
        int semendiagnosis = traininginput[i].semendiagnosis;

        switch (semendiagnosis)
        {
            case (0):
                if (season == (float)-1)
                {
                    season_prob[0].normal_count++;
                }
                else if (season == (float)-0.33)
                {
                    season_prob[1].normal_count++;
                }
                else if (season == (float)0.33)
                {
                    season_prob[2].normal_count++;
                }
                else if (season == (float)1)
                {
                    season_prob[3].normal_count++;
                }
                else
                {
                    printf("Error - unable to calculate Conditional Probability\n");
                    exit(1);
                }
                break;
            case (1):
                if (season == (float)-1)
                {
                    season_prob[0].altered_count++;
                }
                else if (season == (float)-0.33)
                {
                    season_prob[1].altered_count++;
                }
                else if (season == (float)0.33)
                {
                    season_prob[2].altered_count++;
                }
                else if (season == (float)1)
                {
                    season_prob[3].altered_count++;
                }
                else
                {
                    printf("Error - unable to calculate Conditional Probability\n");
                    exit(1);
                }
                break;
        }
    }

    // Calculating Conditional Probability for Season of Analysis
    calculateProbability(season_prob, 4);
    return 0;
}

void calculateProbability(cond_prob input[], int index_size)
{
    /*
    Calculates the Conditional Probability
    Input: struct cond_prob and number of distinct values representing the features
    Output: Populating the normal and altered probability for each cond_prob struct.
    */
   
    for (int i = 0; i < index_size; i++)
    {
        input[i].normal_prob = (float)input[i].normal_count / (float)normal_diagnosis;
        input[i].altered_prob = (float)input[i].altered_count / (float)altered_diagnosis;
        printf("Index %i, Normal count %i, Normal Probablity: %f, Altered Count %i, Altered Probability: %f\n", i, input[i].normal_count, input[i].normal_prob, input[i].altered_count, input[i].altered_prob);
    }
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
        arr[i].season = roundf(arr[i].season * 100) / 100;
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