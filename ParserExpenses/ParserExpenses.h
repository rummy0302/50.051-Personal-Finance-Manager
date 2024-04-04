#ifndef PARSER_EXPENSES_H
#define PARSER_EXPENSES_H

/* Include necessary libraries */
#include "cJSON.h"

/* Define Expenses structure */
typedef struct
{
    int account_id;
    char date[11]; /* Date in the format "YYYY-MM-DD" plus null terminator */
    char description[50];
    char currency[4];
    double amount_spent;
    char category[20];
} Expenses;

/* Function prototypes */
cJSON *parseExpensesJSONfile(const char *filename);
Expenses *processExpensesData(cJSON *json, int *numExpenses);
int validateExpensesJSON(cJSON *json);
int validateDateFormat(const char *date);

#endif /* PARSER_EXPENSES_H */
