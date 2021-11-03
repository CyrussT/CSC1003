#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "nbstructs.h"

#define trainingsize 80 /* 80% of data used for training accuracy */
#define datacount 20    /* 20% of data used for testing accuracy */
#define featuresize 9
#define PI 3.142

void scanArray(features arraySet[], int arrSize);
double mean(double arr[], int n);
double variance(double arr[], int n);

int main()
{
    features traininginput[trainingsize - 1];
    features testinput[datacount - 1];
    scanArray(traininginput, trainingsize - 1);
    scanArray(testinput, datacount - 1);
    return 0;
}
void scanArray(features arr_feature[], int arrSize)
{
    int i, ii, count;
    FILE *file_ptr;
    file_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");
    float season;           // Season of analysis: -1 for Winter, -0.33 for Spring, 0.33 for Summer, 1 for Fall
    double age;             // Age of analysis: Age 18 to 36: 0 to 1
    unsigned char disease;  // Childish Disease: 0 for yes, 1 for no
    unsigned char accident; // Accident or serious trauma: 0 for yes, 1 for no
    unsigned char surgery;  // Surgery intervention: 0 for yes, 1 for no
    int fever;              // High fever in last year: -1 for less than three months ago, 0 for more than three months ago, 1 for no.
    float alcohol;          // Frequency of alcohol consumption: 0.2 for several times a day, 0.4 for everyday, 0.6 for several times a week, 0.8 for once a week, 1 for hardly ever or ever.
    int smoker;             // Smoking Habit: -1 for never, 0 for occassional, 1 for daily
    double sittinghours;    // Hours spent sitting down per day: 0 to 1 (0 for 0 hours, 1 for 24 hours?)
    char semendiagnosis;    // Semen Diagnosis: 0 for Normal, 1 for Altered
    if (arrSize == trainingsize - 1)
    {
        if (file_ptr == NULL)
        {
            printf("File could not be opened \n");
            exit(1);
        }
        for (i = 0; i <= trainingsize - 1; i++)
        {
            /* scanning the wrong numbers, could be the ascii*/
            fscanf(file_ptr, "%f, %f, %f ,%f, %f, %f, %f, %f, %f, %c", &season, &age, &disease, &accident, &surgery, &fever, &alcohol, &smoker, &sittinghours, &semendiagnosis);
            arr_feature[i].season = season;
            arr_feature[i].age = age;
            arr_feature[i].disease = disease;
            arr_feature[i].accident = accident;
            arr_feature[i].surgery = surgery;
            arr_feature[i].fever = fever;
            arr_feature[i].alcohol = alcohol;
            arr_feature[i].smoker = smoker;
            arr_feature[i].sittinghours = sittinghours;
            arr_feature[i].semendiagnosis = semendiagnosis;
            printf("\n%f, %d, %c ,%c, %c, %d, %f, %d, %lf, %c", arr_feature[i].season, arr_feature[i].age, arr_feature[i].disease, arr_feature[i].accident, arr_feature[i].surgery, arr_feature[i].fever, arr_feature[i].alcohol, arr_feature[i].smoker, arr_feature[i].sittinghours, arr_feature[i].semendiagnosis);
        }
        fclose(file_ptr);
    }
    else
    {
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