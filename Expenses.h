/* Include necessary libraries */
#include "cJSON.h"

/* Define Expenses structure */
typedef struct {
    int account_id;
    char date[11]; /* Date in the format "YYYY-MM-DD" plus null terminator */
    char description[50];
    char currency[4];
    double amount_spent;
} Expenses;

/* Function prototypes */
cJSON * parseJSONfile(const char * filename);
Expenses * processData(cJSON * json, int * numAccounts);
