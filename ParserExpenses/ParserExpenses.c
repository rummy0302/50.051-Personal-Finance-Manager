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
    int lineNumber;
    cJSON *dateObject;
    cJSON *currencyItem;
    const char *currency;

    /* Check if JSON is an array */
    if (!cJSON_IsArray(json))
    {
        printf("Error: JSON is not an array.\n");
        return 0;
    }

    /* Get the number of elements in the array */
    numExpenses = cJSON_GetArraySize(json);
    lineNumber = 1; /* Starting [ */
    /* Each element in the json spans 7 lines (including {}) */

    /* Iterate over each element to validate its format */
    for (i = 0; i < numExpenses; i++)
    {
        
        cJSON *expenseObject = cJSON_GetArrayItem(json, i);
        lineNumber++; /* Starting { */

        /* Check if all required fields are present and are in the correct format */

        if (!cJSON_HasObjectItem(expenseObject, "account_id")){
                printf("\n");
                printf("Error in Expenses.json : Missing required account_id field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of account_id */
                if (!cJSON_IsNumber(cJSON_GetObjectItem(expenseObject, "account_id")) ||
                    cJSON_GetObjectItem(expenseObject, "account_id")->valuedouble < 10 ||
                    cJSON_GetObjectItem(expenseObject, "account_id")->valuedouble >= 100)
                {
                    printf("\n");
                    printf("Error in Expenses.json : Invalid account_id format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the account_id is of type int and is within the range of 1-90.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;


            if (!cJSON_HasObjectItem(expenseObject, "date")){
                printf("\n");
                printf("Error in Expenses.json : Missing required date field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            dateObject = cJSON_GetObjectItem(expenseObject, "date");
            if (cJSON_IsString(dateObject)) {
                /* Check format of date */
                if (!validateDateFormat(dateObject->valuestring)) {
                    printf("\n");
                    printf("Error in Expenses.json : Invalid date format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
                    printf("\n");
                    return 0;
                }
            } else if (cJSON_IsNumber(dateObject)) {
                /* If date is a number, raise an error immediately */
                printf("\n");
                printf("Error in Expenses.json : Invalid date format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
                printf("\n");
                return 0;
            } else {
                printf("\n");
                printf("Error in Expenses.json : Invalid date format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
                printf("\n");
                return 0;
            }
            lineNumber++;


            if (!cJSON_HasObjectItem(expenseObject, "description")){
                printf("\n");
                printf("Error in Expenses.json : Missing required description field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of description */
                if (!cJSON_IsString(cJSON_GetObjectItem(expenseObject, "description")))
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid description format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the description is a string.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;


            if (!cJSON_HasObjectItem(expenseObject, "currency")){
                printf("\n");
                printf("Error in Accounts.json : Missing required currency field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of currency */
                currencyItem = cJSON_GetObjectItem(expenseObject, "currency");
                if (!cJSON_IsString(currencyItem)) {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid currency format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the currency is a 3-letter string. \n");
                    printf("\n");
                    return 0;
                }

                currency = currencyItem->valuestring;
                if (strlen(currency) != 3) {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid currency format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the currency is a 3-letter string. \n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;


            if (!cJSON_HasObjectItem(expenseObject, "amount_spent")){
                printf("\n");
                printf("Error in Accounts.json : Missing required amount_spent field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of amount_spent */
                if (!cJSON_IsNumber(cJSON_GetObjectItem(expenseObject, "amount_spent")))
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid amount_spent format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the amount_spent is a float or an integer value.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;

        /* Check if there are extra fields */
        if (cJSON_GetArraySize(expenseObject) > 5)
        {
            
            lineNumber += (cJSON_GetArraySize(expenseObject) - 5);
            printf("\n");
            printf("Error in Accounts.json : Extra fields at entry number %d and line number %d.\n", i+1, lineNumber);
            printf("\n");
            return 0;
        }


        lineNumber++; /* Ending } */
        
    }
    lineNumber++; /* Ending ] */

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


