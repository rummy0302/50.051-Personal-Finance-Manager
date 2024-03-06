#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Expenses.h"

/* Function prototypes */
cJSON *parseJSONfile(const char *filename);
Expenses *processData(cJSON *json, int *numExpenses);

// int main(int argc, char **argv)
// {
//     int i;
//     int numExpenses;

//     /* Parse the JSON file */
//     cJSON *json = parseExpensesJSONfile("Expenses.json");

//     /* Process the JSON data into Expenses array */
//     Expenses *expenses = processExpensesData(json, &numExpenses);

//     /* Print the expenses */
//     for (i = 0; i < numExpenses; i++)
//     {
//         printf("Account ID: %d\n", expenses[i].account_id);
//         printf("Date: %s\n", expenses[i].date);
//         printf("Description: %s\n", expenses[i].description);
//         printf("Currency: %s\n", expenses[i].currency);
//         printf("Amount Spent: %.2f\n", expenses[i].amount_spent);
//         printf("\n");
//     }

//     /* Free allocated memory */
//     free(expenses);
//     cJSON_Delete(json);

//     return 0;
// }

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

    /* Free allocated memory */
    free(jsonContent);

    return json;
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
