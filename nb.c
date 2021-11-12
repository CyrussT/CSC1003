#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "nbstructs.h"

#define TRAININGSIZE 80 /* 80% of data used for training accuracy */
#define DATACOUNT 20    /* 20% of data used for testing accuracy */
#define FEATURESIZE 9
#define MAXINDEXSIZE 5
#define NUMOFOUTCOMES 2
#define PI 3.14159

void scanArray(features arr[]);
double mean(double arr[], int n);
double variance(double arr[], int n);
void calculateProbability(cond_prob input[], int index_size);
double calculateMeanAndVariance(features arr[], cond_prob_range input[], int arr_size);
void calculateFeatures(features arr[], int result, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void loopArr(features arr[], int arr_size, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFloatFeatures(float num, int result, int index, int value_size, float values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFeatures(int num, int result, int index, int value_size, int values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void throwError(int code);
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

    printf("Prior Probability\nNormal: %i, Altered: %i \n", normal_diagnosis, altered_diagnosis); // Checked output and figure tallys.
    
    //Initialise a 2D array, index 0 is outcome, index 1 is feature number, index 2 is feature index
    int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE];
    for (int i = 0; i < NUMOFOUTCOMES; i ++)
    {
        for (int j = 0; j < FEATURESIZE; j++)
        {
            for (int k = 0; k < MAXINDEXSIZE; k++)
            {
                probabilitySum[i][j][k] = 0;
            }
        }
    }

    loopArr(traininginput, TRAININGSIZE, probabilitySum);
    //printf("%i, %i", probabilitySum[0][0][0], probabilitySum[0][0][1]);
    return 0;

    // // Calculating Conditional Probability for Seasons of Analysis
    // // Winter: -1, Spring: -0.33, Summer: 0.33, Fall: 1
    // // Index : 0, 1, 2, 3
    // cond_prob season_prob[4] = {0, 0, 0, 0}; 
    // for (int i = 0; i < TRAININGSIZE; i++)
    // {
    //     float season = traininginput[i].season;
    //     int semendiagnosis = traininginput[i].semendiagnosis;

    //     switch (semendiagnosis)
    //     {
    //         case (0):
    //             if (season == (float)-1)
    //             {
    //                 season_prob[0].normal_count++;
    //             }
    //             else if (season == (float)-0.33)
    //             {
    //                 season_prob[1].normal_count++;
    //             }
    //             else if (season == (float)0.33)
    //             {
    //                 season_prob[2].normal_count++;
    //             }
    //             else if (season == (float)1)
    //             {
    //                 season_prob[3].normal_count++;
    //             }
    //             else
    //             {
    //                 printf("Error - unable to calculate Conditional Probability\n");
    //                 exit(1);
    //             }
    //             break;
    //         case (1):
    //             if (season == (float)-1)
    //             {
    //                 season_prob[0].altered_count++;
    //             }
    //             else if (season == (float)-0.33)
    //             {
    //                 season_prob[1].altered_count++;
    //             }
    //             else if (season == (float)0.33)
    //             {
    //                 season_prob[2].altered_count++;
    //             }
    //             else if (season == (float)1)
    //             {
    //                 season_prob[3].altered_count++;
    //             }
    //             else
    //             {
    //                 printf("Error - unable to calculate Conditional Probability\n");
    //                 exit(1);
    //             }
    //             break;
    //     }
    // }

    // // Calculating Conditional Probability for Season of Analysis
    // printf("Season Conditional Probability\n");
    // calculateProbability(season_prob, 4);
    // //Calculating Conditional Probability for Age of Analysis
    // cond_prob_range age_prob[] = {0, 0, 0, 0};
    // calculateMeanAndVariance(traininginput, age_prob, TRAININGSIZE);
    // return 0;
}

void loopArr(features arr[], int arr_size, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    for (int i = 0; i < arr_size; i++)
    {
        if (arr[i].semendiagnosis == 0)
        {
            calculateFeatures(&arr[i], 0, probabilitySum);
        }
        else if (arr[i].semendiagnosis == 1)
        {
            calculateFeatures(&arr[i], 1, probabilitySum);
        }
    }
}

void calculateFeatures(features arr[], int result, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    // new season
    float seasonValues[5] = {-1, -0.33, 0.33, 1};
    sumFloatFeatures(arr->season, result, 0, 4, seasonValues, probabilitySum);
    //Childish Disease at Index 2
    int twoValues[4] = {0, 1};
    sumFeatures(arr->disease, result, 2, 2, twoValues, probabilitySum);
    //Accident or Serious Trauma at Index 3
    sumFeatures(arr->accident, result, 3, 2, twoValues, probabilitySum);
    //Surgical Intervention at Index 4
    sumFeatures(arr->surgery, result, 4, 2, twoValues, probabilitySum);
    //High fevers at Index 5
    int threeValues[5] = {-1, 0, 1};
    sumFeatures(arr->fever, result, 5, 3, threeValues, probabilitySum);
    //Frequency of Alchohol at Index 6
    float alcoholValues[5] = {0.2, 0.4, 0.6, 0.8, 1};
    sumFloatFeatures(arr->alcohol, result, 6, 5, alcoholValues, probabilitySum);
    //Smoking Habit at Index 7
    sumFeatures(arr->smoker, result, 7, 3, threeValues, probabilitySum);
}

void sumFloatFeatures(float num, int result, int index, int value_size, float values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    for (int i = 0; i < value_size; i++)
    {
        if (num == values[i])
        {
            probabilitySum[result][index][i];
        }
    }
}

void sumFeatures(int num, int result, int index, int value_size, int values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    for (int i = 0; i < value_size; i++)
    {
        if(num == values[i])
        {
            probabilitySum[result][index][i];
        }
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

double calculateMeanAndVariance(features arr[], cond_prob_range input[], int arr_size)
{
    int i; 
    float normal_sum = 0, altered_sum = 0;

    //Calculating Sum of Normal and Altered
    for (i = 0; i < arr_size; i++)
    {
        if(arr[i].semendiagnosis == 0)
        {
            normal_sum+= arr[i].age;
        }
        else if(arr[i].semendiagnosis == 1)
        {
            altered_sum+= arr[i].age;
        }
    }
    //Calculating Mean of Normal and Altered
    input->normal_mean = normal_sum / normal_diagnosis;
    input->altered_mean = altered_sum / altered_diagnosis;

    printf("Age of analysis\nNormal Mean: %f, Altered Mean: %f\n", input->normal_mean, input->altered_mean);

    //Calculating Sigma Portion of Normal and Altered
    float sigma_normal = 0, sigma_altered = 0;
    for (i = 0; i < arr_size; i++)
    {
        if(arr[i].semendiagnosis == 0)
        {
            sigma_normal += pow((arr[i].age - input->normal_mean), 2);
        }
        else if(arr[i].semendiagnosis == 1)
        {
            sigma_altered += pow((arr[i].age - input->altered_mean), 2);
        }
    }
    //Calculating Variance of Normal and Altered
    input->normal_variance = sigma_normal / (normal_diagnosis - 1);
    input->altered_variance = sigma_altered / (altered_diagnosis - 1);

    printf("Normal Variance: %f, Altered Variance: %f\n", input->normal_variance, input->altered_variance);
}

// void calculateSumOfFeatures(features arr[], int arr_size)
// {
//     int i, semendiagnosis;
//     // Childish Disease
//     cond_prob disease_prob[2] = {0, 0, 0, 0};
//     for (i = 0; i < arr_size; i++)
//     {
//         semendiagnosis = arr[i].semendiagnosis;

//         switch (semendiagnosis)
//         {
//             case (0):
//                 if (arr[i].disease == 0)
//                 {
//                     disease_prob[0].normal_count++;
//                 }
//                 else if (arr[i].disease == 1)
//                 {
//                     disease_prob[1].normal_count++;
//                 }
//                 else
//                 {
//                     printf("Error - unable to calculate Conditional Probability\n");
//                     exit(1);
//                 }
//                 break;
//             case (1):
//                 if (arr[i].disease == 0)
//                 {
//                     disease_prob[0].altered_count++;
//                 }
//                 else if (arr[i].disease == 1)
//                 {
//                     disease_prob[1].altered_count++;
//                 }
//                 else
//                 {
//                     printf("Error - unable to calculate Conditional Probability\n");
//                     exit(1);
//                 }
//                 break;
//         }
//     }
// }

double gaussian(double x, double mean, double variance) //needs to run and check output
{
    return (1 / (sqrt(2 * PI * variance))) * exp(-0.5 * pow(x - mean, 2) / (2 * variance));
}

void throwError(int code)
{
    printf("Error occured!");
    exit(code);
}