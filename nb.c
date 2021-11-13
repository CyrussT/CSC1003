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
void loopArr(features arr[], int arr_size, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void calculateFeatures(features arr[], int result, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFloatFeatures(float num, int result, int index, int value_size, float values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFeatures(int num, int result, int index, int value_size, int values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void calculateFeature2(features arr[], cond_prob_range input[], int arr_size);
void calculateFeature9(features arr[], cond_prob_range input[], int arr_size);
void calculateProbability(cond_prob input[], int index_size, int indexNumber, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
float gaussian(float x, float mean, float variance);
void predict(features test[], int arr_size, predicted_prob prediction[], cond_prob season[], cond_prob_range age[], cond_prob disease[], cond_prob accident[], cond_prob surger[], cond_prob fever[], cond_prob alcohol[], cond_prob smoking[], cond_prob_range sitting[]);
int calculateError(predicted_prob prediction[], int arr_size);
features traininginput[TRAININGSIZE], testinput[DATACOUNT];

// Feature Variables
int normal_diagnosis, altered_diagnosis;
float seasonValues[4] = {-1, -0.33, 0.33, 1};
int twoValues[2] = {0, 1};
int threeValues[3] = {-1, 0, 1};
float alcoholValues[5] = {0.2, 0.4, 0.6, 0.8, 1};

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
    
    //Initialise a 3D array, index 0 is outcome, index 1 is feature number, index 2 is feature index
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

    //Calculating Probability for normal values, by looping each individual line
    loopArr(traininginput, TRAININGSIZE, probabilitySum);
    //Calculating mean and variance for feature 2 and 9
    cond_prob_range age[] = {0, 0, 0, 0};
    cond_prob_range sitting[] = {0, 0, 0, 0};
    calculateFeature2(traininginput, age, TRAININGSIZE);
    calculateFeature9(traininginput, sitting, TRAININGSIZE);
    // Populating probability 
    cond_prob season[4]; //Season
    calculateProbability(season, 4, 0, probabilitySum);
    cond_prob disease[2]; // Childish Disease
    calculateProbability(disease, 2, 2, probabilitySum);
    cond_prob accident[2]; // Accident or serious trauma
    calculateProbability(accident, 2, 3, probabilitySum);
    cond_prob surgery[2]; // Surgical Intervention
    calculateProbability(surgery, 2, 4, probabilitySum);
    cond_prob fever[3]; // High fevers
    calculateProbability(fever, 3, 5, probabilitySum);
    cond_prob alcohol[5]; // Freq. of Alcohol Consumption
    calculateProbability(alcohol, 5, 6, probabilitySum);
    cond_prob smoking[3]; // Smoking Habit
    calculateProbability(smoking, 3, 7, probabilitySum);
    // Printing of all the probabilities: 
    printf("=====================\n");
    printf("Overall Probability\n");
    printf("=====================\n");
    printf("Season 0: %f, %f, Season 1: %f, %f, Season 2: %f, %f, Season 3: %f, %f\n", season[0].normal_prob, season[0].altered_prob, season[1].normal_prob, season[1].altered_prob, season[2].normal_prob, season[2].altered_prob, season[3].normal_prob, season[3].altered_prob);
    printf("Normal age mean: %f, variance: %f, Altered age mean: %f, variance: %f\n", age->normal_mean, age->normal_variance, age->altered_mean, age->altered_variance);    
    printf("Disease 0: %f, %f, Disease 1: %f, %f\n", disease[0].normal_prob, disease[0].altered_prob, disease[1].normal_prob, disease[1].altered_prob);
    printf("Accident 0: %f, %f, Accident 1: %f, %f\n", accident[0].normal_prob, accident[0].altered_prob, accident[1].normal_prob, accident[1].altered_prob);
    printf("Surgery 0: %f, %f, Surgery 1: %f, %f\n", surgery[0].normal_prob, surgery[0].altered_prob, surgery[1].normal_prob, surgery[1].altered_prob);
    printf("Fever 0: %f, %f, Fever 1: %f, %f, Fever 2: %f, %f\n", fever[0].normal_prob, fever[0].altered_prob, fever[1].normal_prob, fever[1].altered_prob, fever[2].normal_prob, fever[2].altered_prob);
    printf("Alcohol 0: %f, %f, Alcohol 1: %f, %f, Alcohol 2: %f, %f, Alcohol 3: %f, %f, Alcohol 4: %f, %f\n", alcohol[0].normal_prob, alcohol[0].altered_prob, alcohol[1].normal_prob, alcohol[1].altered_prob, alcohol[2].normal_prob, alcohol[2].altered_prob, alcohol[3].normal_prob, alcohol[3].altered_prob, alcohol[4].normal_prob, alcohol[4].altered_prob);
    printf("Smoking 0: %f, %f, Smoking 1: %f, %f, Smoking 2: %f, %f\n", smoking[0].normal_prob, smoking[0].altered_prob, smoking[1].normal_prob, smoking[1].altered_prob, smoking[2].normal_prob, smoking[2].altered_prob);
    printf("Hours sitting normal mean: %f, variance: %f, Hours sitting altered mean: %f, variance: %f\n", sitting->normal_mean, sitting->normal_variance, sitting->altered_mean, sitting->altered_variance);
   
   //Testing Phase
   predicted_prob prediction[DATACOUNT];
   predict(testinput, DATACOUNT, prediction, season, age, disease, accident, surgery, fever, alcohol, smoking, sitting);
   int errors = calculateError(prediction, DATACOUNT);
   float accuracy = (float)1 - ((float) errors / (float) DATACOUNT);
   printf("Total Errors: %i out of %i at %f accuracy!\n", errors, DATACOUNT, accuracy);
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
        arr[i].season = roundf(arr[i].season * 100) / 100;
        printf("%f, %f, %d, %d, %d, %d, %f, %d, %f, %d\n", arr[i].season, arr[i].age, arr[i].disease, arr[i].accident, arr[i].surgery, arr[i].fever, arr[i].alcohol, arr[i].smoker, arr[i].sittinghours, arr[i].semendiagnosis);
        i++;
    }
}

void loopArr(features arr[], int arr_size, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    // looping through each individual line to determine normal or altered and calls calculateFeatures with the diagnosis as parameter
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
    // Input: A single line
    // Iterates through the features of the line, increment the count of that feature in the array probabilitySum
    // new season at index 0
    //float seasonValues[4] = {-1, -0.33, 0.33, 1};
    sumFloatFeatures(arr->season, result, 0, 4, seasonValues, probabilitySum);
    //Childish Disease at Index 2
    //int twoValues[2] = {0, 1};
    sumFeatures(arr->disease, result, 2, 2, twoValues, probabilitySum);
    //Accident or Serious Trauma at Index 3
    sumFeatures(arr->accident, result, 3, 2, twoValues, probabilitySum);
    //Surgical Intervention at Index 4
    sumFeatures(arr->surgery, result, 4, 2, twoValues, probabilitySum);
    //High fevers at Index 5
    //int threeValues[3] = {-1, 0, 1};
    sumFeatures(arr->fever, result, 5, 3, threeValues, probabilitySum);
    //Frequency of Alchohol at Index 6
    //float alcoholValues[5] = {0.2, 0.4, 0.6, 0.8, 1};
    sumFloatFeatures(arr->alcohol, result, 6, 5, alcoholValues, probabilitySum);
    //Smoking Habit at Index 7
    sumFeatures(arr->smoker, result, 7, 3, threeValues, probabilitySum);
}

void sumFloatFeatures(float num, int result, int index, int value_size, float values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    // Increments the array probabilitySum by matching the num against the possible array indexes for floats
    for (int i = 0; i < value_size; i++)
    {
        if (num == values[i])
        {
            probabilitySum[result][index][i]++;
            break;
        }
    }
}

void sumFeatures(int num, int result, int index, int value_size, int values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    // Increments the array probabilitySum by matching the num against the possible array indexes for floats
    for (int i = 0; i < value_size; i++)
    {
        if(num == values[i])
        {
            probabilitySum[result][index][i]++;
            break;
        }
    }
}

void calculateFeature2(features arr[], cond_prob_range input[], int arr_size)
{
    // Used for calculating feature 2, age
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

    //printf("Age of analysis\nNormal Mean: %f, Altered Mean: %f\n", input->normal_mean, input->altered_mean);

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

    //printf("Normal Variance: %f, Altered Variance: %f\n", input->normal_variance, input->altered_variance);
}

void calculateFeature9(features arr[], cond_prob_range input[], int arr_size)
{
    // used for calculating feature 9, hours sitting down.
    int i; 
    float normal_sum = 0, altered_sum = 0;

    //Calculating Sum of Normal and Altered
    for (i = 0; i < arr_size; i++)
    {
        if(arr[i].semendiagnosis == 0)
        {
            normal_sum+= arr[i].sittinghours;
        }
        else if(arr[i].semendiagnosis == 1)
        {
            altered_sum+= arr[i].sittinghours;
        }
    }
    //Calculating Mean of Normal and Altered
    input->normal_mean = normal_sum / normal_diagnosis;
    input->altered_mean = altered_sum / altered_diagnosis;

    //printf("Sitting Hours analysis\nNormal Mean: %f, Altered Mean: %f\n", input->normal_mean, input->altered_mean);

    //Calculating Sigma Portion of Normal and Altered
    float sigma_normal = 0, sigma_altered = 0;
    for (i = 0; i < arr_size; i++)
    {
        if(arr[i].semendiagnosis == 0)
        {
            sigma_normal += pow((arr[i].sittinghours - input->normal_mean), 2);
        }
        else if(arr[i].semendiagnosis == 1)
        {
            sigma_altered += pow((arr[i].sittinghours - input->altered_mean), 2);
        }
    }


    //Calculating Variance of Normal and Altered
    input->normal_variance = sigma_normal / (normal_diagnosis - 1);
    input->altered_variance = sigma_altered / (altered_diagnosis - 1);

    //printf("Normal Variance: %f, Altered Variance: %f\n", input->normal_variance, input->altered_variance);
}

void calculateProbability(cond_prob input[], int index_size, int indexNumber, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE])
{
    /*
    Calculates the Conditional Probability
    Input: struct cond_prob and number of distinct values representing the features, index number of feature, probabilitySum array
    Output: Populating the normal and altered probability for each cond_prob struct.
    */
   for (int i = 0; i < index_size; i++)
   {
       input[i].normal_prob = (float) probabilitySum[0][indexNumber][i] / (float) normal_diagnosis;
       input[i].altered_prob = (float) probabilitySum[1][indexNumber][i] / (float) altered_diagnosis;
       //printf("Feature %i at Index %i: Normal Probability: %f, Altered Probability: %f\n", indexNumber, i, input[i].normal_prob, input[i].altered_prob);
   }
}

float gaussian(float x, float mean, float variance)
{
    return (1 / (sqrt(2 * PI))) *exp(-0.5 * pow((x - mean),2) / variance);
}

void predict(features test[], int arr_size, predicted_prob prediction[], cond_prob season[], cond_prob_range age[], cond_prob disease[], cond_prob accident[], cond_prob surgery[], cond_prob fever[], cond_prob alcohol[], cond_prob smoking[], cond_prob_range sitting[])
{
    // loops through each line and predict the feature
    for (int i = 0; i < arr_size; i++)
    {
        prediction[i].actual_diagnosis = test[i].semendiagnosis; // store the actual diagnosis in prediction struct
        prediction[i].normal_prob = 1;
        prediction[i].altered_prob = 1;

        //Test Season
        for (int j = 0; j < 4; j++)
        {
            if (test[i].season == seasonValues[j])
            {
                prediction[i].normal_prob *= season[j].normal_prob;
                prediction[i].altered_prob *= season[j].altered_prob;
                break;
            }
        }

        //Test Age, needs to call the gaussian function
        prediction[i].normal_prob *= gaussian(test[i].age, age->normal_mean, age->normal_variance);
        prediction[i].altered_prob *= gaussian(test[i].age, age->altered_mean, age->altered_variance);

        //Test Childish disease
        for (int j = 0; j < 2; j++)
        {
            if (test[i].disease == twoValues[j])
            {
                prediction[i].normal_prob *= disease[j].normal_prob;
                prediction[i].altered_prob *= disease[j].altered_prob;
                break;
            }
        }

        //Test Accident
        for (int j = 0; j < 2; j++)
        {
            if (test[i].accident == twoValues[j])
            {
                prediction[i].normal_prob *= accident[j].normal_prob;
                prediction[i].altered_prob *= accident[j].altered_prob;
                break;
            }
        }

        //Test Surgery
        for (int j = 0; j < 2; j++)
        {
            if (test[i].surgery == twoValues[j])
            {
                prediction[i].normal_prob *= surgery[j].normal_prob;
                prediction[i].altered_prob *= surgery[j].altered_prob;
                break;
            }
        }

        //Test Fever
        for (int j = 0; j < 3; j++)
        {
            if (test[i].fever == twoValues[j])
            {
                prediction[i].normal_prob *= fever[j].normal_prob;
                prediction[i].altered_prob *= fever[j].altered_prob;
                break;
            }
        }        

        //Test Alcohol
        for (int j = 0; j < 5; j++)
        {
            if (test[i].alcohol == twoValues[j])
            {
                prediction[i].normal_prob *= alcohol[j].normal_prob;
                prediction[i].altered_prob *= alcohol[j].altered_prob;
                break;
            }
        }

        //Test Smoking
        for (int j = 0; j < 3; j++)
        {
            if (test[i].smoker == twoValues[j])
            {
                prediction[i].normal_prob *= smoking[j].normal_prob;
                prediction[i].altered_prob *= smoking[j].altered_prob;
                break;
            }
        }

        //Test Hours sitting down per day
        prediction[i].normal_prob *= gaussian(test[i].sittinghours, age->normal_mean, age->normal_variance);
        prediction[i].altered_prob *= gaussian(test[i].sittinghours, age->altered_mean, age->altered_variance);

        // Predict if Normal or Altered
        if (prediction[i].normal_prob >= prediction[i].altered_prob)
        {
            prediction[i].predicted_diagnosis = 0;
        }
        else
        {
            prediction[i].predicted_diagnosis = 1;
        }
        printf("For index: %i, normal probability: %g, altered probability: %g, predicted diagnosis: %d, actual diagnosis: %d \n", i, prediction[i].normal_prob, prediction[i].altered_prob, prediction[i].predicted_diagnosis, prediction[i].actual_diagnosis);
    }
}

int calculateError(predicted_prob prediction[], int arr_size)
{
    int wrong = 0;
    for (int i = 0; i < arr_size; i++)
    {
        if(prediction[i].predicted_diagnosis != prediction[i].actual_diagnosis)
        {
            wrong++;
        }
    }
    return wrong;
}