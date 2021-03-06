// Stores the features input
typedef struct features 
{
    float season; // Season of analysis: -1 for Winter, -0.33 for Spring, 0.33 for Summer, 1 for Fall
    float age; // Age of analysis: Age 18 to 36: 0 to 1
    unsigned char disease; // Childish Disease: 0 for yes, 1 for no
    unsigned char accident; // Accident or serious trauma: 0 for yes, 1 for no
    unsigned char surgery; // Surgery intervention: 0 for yes, 1 for no
    char fever; // High fever in last year: -1 for less than three months ago, 0 for more than three months ago, 1 for no.
    float alcohol; // Frequency of alcohol consumption: 0.2 for several times a day, 0.4 for everyday, 0.6 for several times a week, 0.8 for once a week, 1 for hardly ever or ever.
    char smoker; // Smoking Habit: -1 for never, 0 for occassional, 1 for daily
    float sittinghours; // Hours spent sitting down per day: 0 to 1 (0 for 0 hours, 1 for 24 hours?) 
    char semendiagnosis; // Semen Diagnosis: 0 for Normal, 1 for Altered
} features;

// Stores the conditional probability
typedef struct cond_prob
{
    float normal_prob;
    float altered_prob;
} cond_prob;

// Stores the conditional probability for feature 2 and 9, age and hours spent sitting down
typedef struct cond_prob_range
{
    float normal_mean; // mean for normal diagnosis
    float normal_variance; // variance for normal diagnosis
    float altered_mean; // mean for altered diagnosis
    float altered_variance; // variance for normal diagnosis
} cond_prob_range;

// Used to store normal and altered probability of each line. As well as the prediction and actual diagnosis
typedef struct predicted_prob
{
    double normal_prob; // Posterior Normal Probability
    double altered_prob; // Posterior Altered Probability
    char predicted_diagnosis; // Storing of predicted diagnosis result
    char actual_diagnosis; // Storing of actual diagnosis result
} predicted_prob;

// Stores the confusion matrix values and error probability 
typedef struct confusion_matrix
{
    int true_positive; 
    int true_negative;
    int false_positive;
    int false_negative;
    float total_errors;
} confusion_matrix;