#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "nbstructs.h"

#define FEATURESIZE 9
#define MAXINDEXSIZE 5
#define NUMOFOUTCOMES 2
#define PI 3.14159

// Global variables for TRAININGSIZE
int TRAININGSIZE = 0;
int TESTINGSIZE = 0;

//Function Prototypes
void scanArray(features arr[]);
void loopArr(features arr[], int arr_size, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void calculateFeatures(features arr[], int result, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFloatFeatures(float num, int result, int index, int value_size, float values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void sumFeatures(int num, int result, int index, int value_size, int values[], int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
void calculateFeature2(features arr[], cond_prob_range input[], int arr_size);
void calculateFeature9(features arr[], cond_prob_range input[], int arr_size);
void calculateProbability(cond_prob input[], int index_size, int indexNumber, int probabilitySum[NUMOFOUTCOMES][FEATURESIZE][MAXINDEXSIZE]);
double gaussian(float x, float mean, float variance);
void predict(features test[], int arr_size, predicted_prob prediction[], cond_prob season[], cond_prob_range age[], cond_prob disease[], cond_prob accident[], cond_prob surger[], cond_prob fever[], cond_prob alcohol[], cond_prob smoking[], cond_prob_range sitting[]);
void calculateError(predicted_prob prediction[], int arr_size, confusion_matrix errors[]);
void plotConfusion(confusion_matrix errors[]);
void naivesBayes(int TRAININGSIZE, int TESTINGSIZE, float *trainError, float *testError);
void run(void);
void plotGraph(float errors[5][2]);

// Feature Variables to be used to loop later
int normal_diagnosis, altered_diagnosis;
float prior_normal, prior_altered;
float seasonValues[4] = {-1, -0.33, 0.33, 1};
int twoValues[2] = {0, 1};
int threeValues[3] = {-1, 0, 1};
float alcoholValues[5] = {0.2, 0.4, 0.6, 0.8, 1};

int main()
{
    //Start clock timer
    double timer = 0.0;
    clock_t start = clock();
    // Runs the program
    run();
    //Stop timer and print time taken
    clock_t end = clock();
    timer += (double) (end - start) / CLOCKS_PER_SEC;
    printf("Total time taken: %g seconds", timer);
    return 0;
}

void scanArray(features arr[])
{
    // Open the file and access it using the file pointer
    FILE *fileptr;
    fileptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");

    // Check if the file pointer is null
    if (fileptr == NULL)
    {
        printf("File could not be opened. \n");
        exit(1);
    }

    // Scans each line of the file, and place it into the features arr[] array.
    int i = 0;
    while (EOF != fscanf(fileptr, "%f, %f, %d, %d, %d, %d, %f, %d, %f, %d", &arr[i].season, &arr[i].age, &arr[i].disease, &arr[i].accident, &arr[i].surgery, &arr[i].fever, &arr[i].alcohol, &arr[i].smoker, &arr[i].sittinghours, &arr[i].semendiagnosis))
    {
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
    // Iterates through each feature in arr[], increment the count of that feature in the array probabilitySum
    // Season at index 0
    sumFloatFeatures(arr->season, result, 0, 4, seasonValues, probabilitySum);
    // Childish Disease at Index 2
    sumFeatures(arr->disease, result, 2, 2, twoValues, probabilitySum);
    // Accident or Serious Trauma at Index 3
    sumFeatures(arr->accident, result, 3, 2, twoValues, probabilitySum);
    // Surgical Intervention at Index 4
    sumFeatures(arr->surgery, result, 4, 2, twoValues, probabilitySum);
    // High fevers at Index 5
    sumFeatures(arr->fever, result, 5, 3, threeValues, probabilitySum);
    // Frequency of Alchohol at Index 6
    sumFloatFeatures(arr->alcohol, result, 6, 5, alcoholValues, probabilitySum);
    // Smoking Habit at Index 7
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
    // Increments the array probabilitySum by matching the num against the possible array indexes
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

    // Calculating Sum of Normal and Altered
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
    // Calculating Mean of Normal and Altered
    input->normal_mean = normal_sum / normal_diagnosis;
    input->altered_mean = altered_sum / altered_diagnosis;

    // Calculating Sigma Portion of Normal and Altered
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

    // Calculating Variance of Normal and Altered
    input->normal_variance = sigma_normal / (normal_diagnosis - 1);
    input->altered_variance = sigma_altered / (altered_diagnosis - 1);
}

void calculateFeature9(features arr[], cond_prob_range input[], int arr_size)
{
    // Used for calculating feature 9, hours sitting down.
    int i; 
    float normal_sum = 0, altered_sum = 0;

    // Calculating Sum of Normal and Altered
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
    // Calculating Mean of Normal and Altered
    input->normal_mean = normal_sum / normal_diagnosis;
    input->altered_mean = altered_sum / altered_diagnosis;

    // Calculating Sigma Portion of Normal and Altered
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

    // Calculating Variance of Normal and Altered
    input->normal_variance = sigma_normal / (normal_diagnosis - 1);
    input->altered_variance = sigma_altered / (altered_diagnosis - 1);
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
   }
}

double gaussian(float x, float mean, float variance)
{
    // returns the probability value given a gaussian standard distribution.
    // input: float x, float mean, float variance
    // output: a float probability
    return (1 / (sqrt(2 * PI))) * exp(-0.5 * (pow((x - mean),2) / variance));
}

void predict(features test[], int arr_size, predicted_prob prediction[], cond_prob season[], cond_prob_range age[], cond_prob disease[], cond_prob accident[], cond_prob surgery[], cond_prob fever[], cond_prob alcohol[], cond_prob smoking[], cond_prob_range sitting[])
{
    // loops through each line and predict the feature
    for (int i = 0; i < arr_size; i++)
    {
        prediction[i].actual_diagnosis = test[i].semendiagnosis; // store the actual diagnosis in prediction struct
        prediction[i].normal_prob = 1;
        prediction[i].altered_prob = 1;

        // Test Season
        for (int j = 0; j < 4; j++)
        {
            if (test[i].season == seasonValues[j])
            {
                prediction[i].normal_prob *= season[j].normal_prob;
                prediction[i].altered_prob *= season[j].altered_prob;
                break;
            }
        }

        // Calling gaussian function to determine probability for age
        prediction[i].normal_prob *= gaussian(test[i].age, age->normal_mean, age->normal_variance);
        prediction[i].altered_prob *= gaussian(test[i].age, age->altered_mean, age->altered_variance);

        // Test Childish disease
        for (int j = 0; j < 2; j++)
        {
            if (test[i].disease == twoValues[j])
            {
                prediction[i].normal_prob *= disease[j].normal_prob;
                prediction[i].altered_prob *= disease[j].altered_prob;
                break;
            }
        }

        // Test Accident
        for (int j = 0; j < 2; j++)
        {
            if (test[i].accident == twoValues[j])
            {
                prediction[i].normal_prob *= accident[j].normal_prob;
                prediction[i].altered_prob *= accident[j].altered_prob;
                break;
            }
        }

        // Test Surgery
        for (int j = 0; j < 2; j++)
        {
            if (test[i].surgery == twoValues[j])
            {
                prediction[i].normal_prob *= surgery[j].normal_prob;
                prediction[i].altered_prob *= surgery[j].altered_prob;
                break;
            }
        }

        // Test Fever
        for (int j = 0; j < 3; j++)
        {
            if (test[i].fever == threeValues[j])
            {
                prediction[i].normal_prob *= fever[j].normal_prob;
                prediction[i].altered_prob *= fever[j].altered_prob;
                break;
            }
        }        

        // Test Alcohol
        for (int j = 0; j < 5; j++)
        {
            if (test[i].alcohol == alcoholValues[j])
            {
                prediction[i].normal_prob *= alcohol[j].normal_prob;
                prediction[i].altered_prob *= alcohol[j].altered_prob;
                break;
            }
        }

        // Test Smoking
        for (int j = 0; j < 3; j++)
        {
            if (test[i].smoker == threeValues[j])
            {
                prediction[i].normal_prob *= smoking[j].normal_prob;
                prediction[i].altered_prob *= smoking[j].altered_prob;
                break;
            }
        }

        // Calling gaussian function to determine probability for hours sitting down
        prediction[i].normal_prob *= gaussian(test[i].sittinghours, sitting->normal_mean, sitting->normal_variance);
        prediction[i].altered_prob *= gaussian(test[i].sittinghours, sitting->altered_mean, sitting->altered_variance);
        
        // Multiply Prior Probability
        prediction[i].normal_prob *= prior_normal;
        prediction[i].altered_prob *= prior_altered;

        // Predict if Normal or Altered
        if (prediction[i].normal_prob >= prediction[i].altered_prob)
        {
            prediction[i].predicted_diagnosis = 0;
        }
        else
        {
            prediction[i].predicted_diagnosis = 1;
        }
     }
}

void calculateError(predicted_prob prediction[], int arr_size, confusion_matrix errors[])
{
    // This functions compares predicted diagnosis against actual diagnosis to determine error probability and 
    // confusion matrix and store it into the errors[] struct.
    for (int i = 0; i < arr_size; i++)
    {
        switch (prediction[i].actual_diagnosis)
        {
            // Normal Diagnosis
            case 0:
                if (prediction[i].predicted_diagnosis == prediction[i].actual_diagnosis)
                { 
                    errors->true_negative++;
                }
                else
                {
                    errors->false_negative++;
                }
                break;

            // Altered Diagnosis
            case 1:
                if (prediction[i].predicted_diagnosis == prediction[i].actual_diagnosis)
                {
                    errors->true_positive++;
                }
                else
                {
                    errors->false_positive++;
                }
                break;

            break;
        }
    }
    errors->total_errors = (errors->false_positive + errors->false_negative) / (float) arr_size;
}

void plotConfusion(confusion_matrix errors[])
{
    // Used to plot the Confusion Matrix
    printf("\t\t\tPositive (predicted)\t|\tNegative (predicted)\n");
    printf("Positive (actual)\tTrue Positive = %i\t|\tFalse Negative = %i\n", errors->true_positive, errors->false_negative);
    printf("Negative (actual)\tFalse Positive = %i\t|\tTrue Negative = %i\n\n", errors->false_positive, errors->true_negative);
}

void naivesBayes(int TRAININGSIZE, int TESTINGSIZE, float *trainError, float *testError)
{
    printf("================================================================================\n");
    printf("Naives Bayes for Training: %d and Testing: %d\n", TRAININGSIZE, TESTINGSIZE);
    // Create struct to store the training and test data
    features traininginput[TRAININGSIZE], testinput[TESTINGSIZE];
    // create a temporary table in heap
    features *input = malloc(sizeof(features) * (TRAININGSIZE + TESTINGSIZE));
    // store entire data into the input
    printf("=====================\n");
    printf("Reading in Data\n");
    printf("=====================\n");
    scanArray(input);
    // split input into 2 arrays, traininginput and testinput, based on the size passed to the function
    memcpy(traininginput, input, TRAININGSIZE * sizeof(features));
    memcpy(testinput,&input[TRAININGSIZE], TESTINGSIZE * sizeof(features));
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
    prior_normal = (float) normal_diagnosis / (float) TRAININGSIZE;
    prior_altered = (float) altered_diagnosis / (float) TRAININGSIZE;
    printf("Prior Probability\nNormal: %f, Altered: %f \n", prior_normal, prior_altered); 

    //Initialise a 3D array, index 0 is outcome, index 1 is feature number, index 2 is feature's index
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

    //Predict the training data and store into train_prediction
    predicted_prob train_prediction[TRAININGSIZE];
    predict(traininginput, TRAININGSIZE, train_prediction, season, age, disease, accident, surgery, fever, alcohol, smoking, sitting);

    // Caclulating error for training set
    confusion_matrix train_errors[4] = {0, 0, 0, 0};
    calculateError(train_prediction, TRAININGSIZE, train_errors);
    printf("=====================\n");
    printf("Confusion Matrix\n");
    printf("=====================\n");
    printf("Training Error: %f\n", train_errors->total_errors);
    //Plot confusion matrix
    printf("Confusion Matrix for Training Size of %i\n", TRAININGSIZE);
    plotConfusion(train_errors);

    //Predict the testing data and store into test_prediction
    predicted_prob test_prediction[TESTINGSIZE];
    predict(testinput, TESTINGSIZE, test_prediction, season, age, disease, accident, surgery, fever, alcohol, smoking, sitting);

    // Calculating error for test set
    confusion_matrix test_errors[4] = {0, 0, 0, 0};
    calculateError(test_prediction, TESTINGSIZE, test_errors);
    printf("=====================\n");
    printf("Confusion Matrix\n");
    printf("=====================\n");
    printf("Testing Error: %f\n", test_errors->total_errors);
    //Plot confusion matrix
    printf("Confusion Matrix for Test Size of %i\n", TESTINGSIZE);
    plotConfusion(test_errors);

    // Store the training and testing errors back into the error 2D array
    *trainError = train_errors->total_errors;
    *testError = test_errors->total_errors;
}

void run(void)
{
    // Store all the errors inside this float 2d array, rows for each run and column 0 for training and column 1 for testing
    float errors[5][2];
    // Run the 80:20 split first
    naivesBayes(80, 20, &errors[3][0], &errors[3][1]);
    // Repeat the runs from 50:50 to 90:10, excluding 80:20 and storing the results into the errors 2d array
    for (int i = 50, loop = 0; i < 100; i += 10)
    {
        if (i != 80)
        {
            naivesBayes(i, 100 - i, &errors[loop][0], &errors[loop][1]);
        }
        loop++;
    }
    // Call plotGraph function to plot the probability error graph
    plotGraph(errors);
}

void plotGraph(float errors[5][2])
{
    // This function plots the probability error graph: 

    // Creates 2 files (training.txt and testing.txt) if it does not exist, and assign FILE pointer fp and gp to them respectively.
    FILE *fp = fopen("training.txt", "w");
    FILE *gp = fopen("testing.txt", "w");
    // Creates a pipeline to pipe commands to GNUplot
    FILE *hp = popen("gnuplot -persistent", "w");

    //populate training.txt and testing.txt, where x is the size and y is the error
    for (int i = 0, size = 50; i < 5; i ++, size += 10)
    {
        fprintf(fp, "%i %f\n", size, errors[i][0]);
        fprintf(gp, "%i %f\n", 100-size, errors[i][1]);
    }
    // Sets the title and plot the graph
    fprintf(hp, "set title 'Probability of Error'\n");
    fprintf(hp, "plot 'training.txt' with lines, 'testing.txt' with lines\n");
    // Close the file pointers
    fclose(fp);
    fclose(gp);
    fclose(hp);
}