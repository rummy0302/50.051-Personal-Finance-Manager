#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ExpenseParser.h"

#define MAX_EXPENSES 100 /* Maximum number of expenses to store */

void parse_expensesjson(const char *json, Expenses expenses[], int *num_expenses)
{
    const char *ptr = json;

    /* Skip leading whitespace */
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
    {
        ptr++;
    }

    /* Check if JSON starts with [ */
    if (*ptr != '[')
    {
        printf("Invalid JSON\n");
        return;
    }

    ptr++; /* / Skip [ */

    /* Parse array elements */
    while (*ptr != '\0' && *ptr != ']' && *num_expenses < MAX_EXPENSES)
    {
        /* Parse expense object */
        Expenses *expense = &expenses[*num_expenses];

        /* Parse account_id */
        while (*ptr != ':')
        {
            ptr++;
        }
        ptr++; /* Skip : */

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse numeric value */
        expense->account_id = atoi(ptr);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse date */
        while (*ptr != ':')
        {
            ptr++;
        }
        ptr++; /* Skip :*/

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        sscanf(ptr, "\"%[^\"]\"", expense->date);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse description*/
        while (*ptr != ':')
        {
            ptr++;
        }
        ptr++; /* Skip : */

        /* Skip whitespace*/
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        sscanf(ptr, "\"%[^\"]\"", expense->description);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse currency*/
        while (*ptr != ':')
        {
            ptr++;
        }
        ptr++; /* Skip :*/

        /* Skip whitespace*/
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value*/
        sscanf(ptr, "\"%[^\"]\"", expense->currency);

        /* Move ptr to next token*/
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        
        while (*ptr != ':')
        {
            ptr++;
        }
        ptr++; 

        
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        
        sscanf(ptr, "%f", &expense->amount_spent);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Increment the number of expenses parsed */
        (*num_expenses)++;
    }
}

int isValidAccountID(const int account_id)
{
    
    if (account_id < 1 || account_id > 100)
        return 0;

    return 1;
}

int isValidDate(const char *date)
{
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
        return 0;

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31)
        return 0;

    return 1;
}

int isValidDescription(const char *description)
{
    
    if (description[0] == '\0')
        return 0;
    return 1;
}

int isValidCurrency(const char *currency)
{
    

    if (strlen(currency) != 3)
        return 0;

    return 1;
}

int isValidAmountSpent(const float amount_spend)
{
    

    if (amount_spend == 0.00)
        return 0;

    return 1;
}

int validateExpenses(Expenses expenses[], int num_expenses)

{
    int lineNumber;
    int i;
    int isAllValid = 1;
    lineNumber = 1;
    
    
    for (i = 0; i < num_expenses; i++)
    {
        int valid = 1;
        lineNumber++;

        if (!isValidAccountID(expenses[i].account_id))
        {

            printf("\n");
            printf("Error in Expenses.json : Invalid account_id format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the account_id is of type int and is within the range of 1-100.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidDate(expenses[i].date))
        {
            printf("\n");
            printf("Error in Expenses.json : Invalid date format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }

        lineNumber++;

        if (!isValidDescription(expenses[i].description))
        {
            printf("\n");
            printf("Error in Expenses.json : Invalid description format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the description is a string.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidCurrency(expenses[i].currency))
        {
            printf("\n");
            printf("Error in Expenses.json : Invalid currency format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the currency is a 3-letter string. \n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidAmountSpent(expenses[i].amount_spent))
        {
            printf("\n");
            printf("Error in Expenses.json : Invalid amount_spent format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the amount_spent is a float or an integer value.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (valid)
        {
            /* printf("Expense %d is valid.\n", i + 1);*/
        }
        else
        {
            printf("Expense %d has validation errors.\n", i + 1);
        }
    }
    return isAllValid;
}

/* int main()
{
    const char *filename = "Expenses.json";
    FILE *expenses_file = fopen(filename, "r");
    if (expenses_file == NULL)
    {
        printf("Error: Unable to open %s\n", filename);
        return 1;
    }

    // Get the file size
    fseek(expenses_file, 0L, SEEK_END);
    long fileSize = ftell(expenses_file);
    rewind(expenses_file);

    // Allocate memory for JSON content
    char *jsonContent = (char *)malloc(fileSize + 1); // +1 for null terminator
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        fclose(expenses_file);
        return 1;
    }

    // Read the JSON content into the allocated memory
    size_t bytesRead = fread(jsonContent, 1, fileSize, expenses_file);
    // if (bytesRead != fileSize)
    // {
    //     printf("Error: Failed to read JSON content\n");
    //     free(jsonContent);
    //     fclose(expenses_file);
    //     return 1;
    // }

    // Null-terminate the JSON content
    jsonContent[fileSize] = '\0';

    // Close the file
    fclose(expenses_file);

    // Array to store parsed expenses
    Expenses expenses[MAX_EXPENSES];
    int num_expenses = 0;

    // Parse the JSON content
    memset(expenses, 0, sizeof(expenses));

    parse_expensesjson(jsonContent, expenses, &num_expenses);

    // Access the parsed expenses
    for (int i = 0; i < num_expenses; i++)
    {
        printf("Expense %d:\n", i + 1);
        printf("Account ID: %d\n", expenses[i].account_id);
        printf("Date: %s\n", expenses[i].date);
        printf("Description: %s\n", expenses[i].description);
        printf("Currency: %s\n", expenses[i].currency);
        printf("Amount Spent: %.2f\n", expenses[i].amount_spent);
        printf("\n");
    }

    // Free the allocated memory
    free(jsonContent);

    validateExpenses(expenses, num_expenses);

    return 0;
} */
