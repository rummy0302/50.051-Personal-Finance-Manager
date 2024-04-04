#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "ParserExpenses.h"

/* Function prototypes */
cJSON *parseJSONfile(const char *filename);
Expenses *processData(cJSON *json, int *numExpenses);


/* Function to parse JSON file and return cJSON object */
cJSON *parseExpensesJSONfile(const char *filename)
{
    long fileSize;
    char *jsonContent;
    cJSON *json;

    /* Open the JSON file */
    FILE *expenses_file = fopen(filename, "r");
    if (expenses_file == NULL)
    {
        printf("Error: Unable to open Expenses.json file.\n Please check the Expenses.json file. \n");
        return NULL;
    }

    /* Get the file size */
    fseek(expenses_file, 0L, SEEK_END);
    fileSize = ftell(expenses_file);

    /* Allocate memory for JSON content */
    jsonContent = (char *)malloc(fileSize + 1);
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        return NULL;
    }

    /* Read file content into jsonContent buffer */
    fseek(expenses_file, 0L, SEEK_SET);
    fread(jsonContent, 1, fileSize, expenses_file);
    fclose(expenses_file);
    jsonContent[fileSize] = '\0';

    /* Parse JSON content into cJSON object */
    json = cJSON_Parse(jsonContent);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        return NULL;
    }
    
    /* Validate JSON format */
    if (!validateExpensesJSON(json))
    {
        printf("Error: Invalid JSON format.\n");
        cJSON_Delete(json);
        free(jsonContent);
        return NULL;
    }


    /* Free allocated memory */
    free(jsonContent);

    return json;
}

/* Function to validate the format of Expenses JSON */
int validateExpensesJSON(cJSON *json)
{
    int i, numExpenses;
    /* double amountSpent;*/

    /* Check if JSON is an array */
    if (!cJSON_IsArray(json))
    {
        printf("Error: JSON is not an array.\n");
        return 0;
    }

    /* Get the number of elements in the array */
    numExpenses = cJSON_GetArraySize(json);

    /* Iterate over each element to validate its format */
    for (i = 0; i < numExpenses; i++)
    {
        cJSON *expenseObject = cJSON_GetArrayItem(json, i);

        /* Check if all required fields are present */
        if (!cJSON_HasObjectItem(expenseObject, "account_id") || !cJSON_HasObjectItem(expenseObject, "date") ||
            !cJSON_HasObjectItem(expenseObject, "description") || !cJSON_HasObjectItem(expenseObject, "currency") ||
            !cJSON_HasObjectItem(expenseObject, "amount_spent"))
        {
            printf("Error: Missing required field in expense at index %d.\n", i);
            return 0;
        }

        /* Check if there are extra fields */
        if (cJSON_GetArraySize(expenseObject) > 5)
        {
            printf("Error: Extra fields in expense at index %d.\n", i);
            return 0;
        }

        /* Check format of account_id */
        if (!cJSON_IsNumber(cJSON_GetObjectItem(expenseObject, "account_id")) ||
            cJSON_GetObjectItem(expenseObject, "account_id")->valuedouble < 10 ||
            cJSON_GetObjectItem(expenseObject, "account_id")->valuedouble >= 100)
        {
            printf("Error: Invalid account_id format in at entry number: %d.\n", i+1);
            printf("Please ensure that the account_id is of type int and is within the range of 1-90.\n");
            return 0;
        }

        /* Check format of date */
        if (!validateDateFormat(cJSON_GetObjectItem(expenseObject, "date")->valuestring))
        {
            printf("Error: Invalid date format at entry number: %d.\n", i+1);
            printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
            return 0;
        }

        /* Check format of description */
        if (!cJSON_IsString(cJSON_GetObjectItem(expenseObject, "description")))
        {
            printf("Error: Invalid description format in at entry number: %d.\n", i+1);
            printf("Please ensure that the description is a string.\n");
            return 0;
        }

        /* Check format of currency */
        if (!cJSON_IsString(cJSON_GetObjectItem(expenseObject, "currency")))
        {
            printf("Error: Invalid currency format in at entry number: %d.\n", i+1);
            printf("Please ensure that the currency is a string.\n");
            return 0;
        }

        /* Check format of amount_spent */
        if (!cJSON_IsNumber(cJSON_GetObjectItem(expenseObject, "amount_spent")))
        {
            printf("Error: Invalid amount_spent format in at entry number: %d.\n", i+1);
            printf("Please ensure that the currency is a float.\n");
            return 0;
        }
    }

    return 1; /* JSON format is valid */
}

/* Function to validate the date format */
int validateDateFormat(const char *date)
{
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
        return 0;

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
        return 0;

    return 1;
}

/* Function to process cJSON object into Expenses array */
Expenses *processExpensesData(cJSON *json, int *numExpenses)
{
    int i;
    Expenses *expensesArray;

    /* Get the number of expenses (array size) */
    *numExpenses = cJSON_GetArraySize(json);

    /* Allocate memory for Expenses array */
    expensesArray = (Expenses *)malloc((*numExpenses) * sizeof(Expenses));

    /* Iterate over JSON array and populate Expenses array */
    for (i = 0; i < *numExpenses; i++)
    {
        cJSON *expenseObject = cJSON_GetArrayItem(json, i);
        expensesArray[i].account_id = cJSON_GetObjectItem(expenseObject, "account_id")->valueint;
        strncpy(expensesArray[i].date, cJSON_GetObjectItem(expenseObject, "date")->valuestring, sizeof(expensesArray[i].date));
        strncpy(expensesArray[i].description, cJSON_GetObjectItem(expenseObject, "description")->valuestring, sizeof(expensesArray[i].description));
        strncpy(expensesArray[i].currency, cJSON_GetObjectItem(expenseObject, "currency")->valuestring, sizeof(expensesArray[i].currency));
        expensesArray[i].amount_spent = cJSON_GetObjectItem(expenseObject, "amount_spent")->valuedouble;
    }

    return expensesArray;
}


