#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "ExpenseParser.h"

#define MAX_EXPENSES 400 /* Maximum number of expenses to store */

void parse_expensesjson(const char *json, Expenses expenses[], int *num_expenses, int *error)
{
    const char *ptr = json;
    int lineNumber = 1;
    const char *current_pos;
    const char *next_pos = NULL;
    char *expected_key;
    float amount_spent = 0;
    int result;

    /* Skip leading whitespace */
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
    {
        ptr++;
    }

    /* Check if JSON starts with [ */
    if (*ptr != '[')
    {
        *error = 1;
        printf("Error: Invalid JSON format. File does not contain starting [ at line number: %d.\n", lineNumber);
        return;
    }

    ptr++; /* / Skip [ */

    /* Parse array elements */
    while (*ptr != '\0' && *ptr != ']' && *num_expenses < MAX_EXPENSES)
    {
        /* Parse expense object */
        Expenses *expense = &expenses[*num_expenses];

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r' || *ptr == ',')
        {
            ptr++;
        }

        /* Check if each JSON object starts with { */
        if (*ptr != '{')
        {
            *error = 1;
            printf("Error: Invalid JSON. JSON object does not start with starting { at line number: %d. \n", lineNumber);
            return;
        }

        ptr++; /* Skip { */
        lineNumber++;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        lineNumber++;

        current_pos = ptr;
        expected_key = "\"account_id\"";
        if (!isValidExpenseKey(current_pos, &next_pos, expected_key))
        {
            *error = 1;
            printf("Error in %s key format in line number: %d\n", expected_key, lineNumber);
            return;
        }
        ptr = next_pos;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

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

        /* Check format of account id: */
        if (!isValidAccountID(atoi(ptr)))
        {
            *error = 1;
            printf("\n");
            printf("Error in Expenses.json : Invalid account_id format at line number: %d.\n", lineNumber);
            printf("Please ensure that the account_id is of type int and is within the range of 1-100.\n");
            printf("\n");
            return;
        }

        /* Parse numeric value */
        expense->account_id = atoi(ptr);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Check if the next character is a comma, indicating the start of the next key-value pair */
        if (*ptr == ',')
        {
            ptr++;
        }

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }
        lineNumber++;

        current_pos = ptr;
        expected_key = "\"date\"";
        if (!isValidExpenseKey(current_pos, &next_pos, expected_key))
        {
            *error = 1;
            printf("Error in %s key format in line number: %d\n", expected_key, lineNumber);
            return;
        }
        ptr = next_pos;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check if the next character is ':' */
        if (*ptr != ':')
        {
            *error = 1;
            printf("Error: Invalid JSON format. File does not contain : between key and value at line number: %d.\n", lineNumber);
            return;
        }

        /* Move ptr to the next character after ':' */
        ptr++;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        sscanf(ptr, "\"%[^\"]\"", expense->date);
        if (!isValidDate(expense->date))
        {
            *error = 1;
            printf("\n");
            printf("Error in Expenses.json : Invalid date format in line number: %d.\n", lineNumber);
            printf("Please ensure that the date is a string and follows the format of YYYY-MM-DD. \n");
            printf("\n");
            return;
        }

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Check if the next character is a comma, indicating the start of the next key-value pair */
        if (*ptr == ',')
        {
            ptr++;
        }

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }
        lineNumber++;

        current_pos = ptr;
        expected_key = "\"description\"";
        if (!isValidExpenseKey(current_pos, &next_pos, expected_key))
        {
            *error = 1;
            printf("Error in %s key format in line number: %d\n", expected_key, lineNumber);
            return;
        }
        ptr = next_pos;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check if the next character is ':' */
        if (*ptr != ':')
        {
            *error = 1;
            printf("Error: Invalid JSON format. File does not contain : between key and value at line number: %d.\n", lineNumber);
            return;
        }

        /* Move ptr to the next character after ':' */
        ptr++;

        /* Skip whitespace*/
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        result = sscanf(ptr, "\"%[^\"]\"", expense->description);

        if (result == 1)
        {
            if (!isValidDescription(expense->description))
            {
                *error = 1;
                printf("\n");
                printf("Error in Expenses.json : Invalid description format in line number: %d.\n", lineNumber);
                printf("Please ensure that the description is a not a empty string.\n");
                printf("\n");
                return;
            }
        }

        else
        {
            *error = 1;
            printf("\n");
            printf("Error in Expenses.json : Invalid description format in line number: %d.\n", lineNumber);
            printf("Please ensure that the description is a string.\n");
            printf("\n");
            return;
        }

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Check if the next character is a comma, indicating the start of the next key-value pair */
        if (*ptr == ',')
        {
            ptr++;
        }

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }
        lineNumber++;

        current_pos = ptr;
        expected_key = "\"currency\"";
        if (!isValidExpenseKey(current_pos, &next_pos, expected_key))
        {
            *error = 1;
            printf("Error in %s key format in line number: %d\n", expected_key, lineNumber);
            return;
        }
        ptr = next_pos;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check if the next character is ':' */
        if (*ptr != ':')
        {
            *error = 1;
            printf("Error: Invalid JSON format. File does not contain : between key and value at line number: %d.\n", lineNumber);
            return;
        }

        /* Move ptr to the next character after ':' */
        ptr++;

        /* Skip whitespace*/
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value*/
        result = sscanf(ptr, "\"%[^\"]\"", expense->currency);

        if (result == 1)
        {
            if (!isValidCurrency(expenses->currency))
            {
                *error = 1;
                printf("\n");
                printf("Error in Expenses.json : Invalid currency format in line number: %d.\n", lineNumber);
                printf("Please ensure that the currency is not an empty string. \n");
                printf("\n");
                return;
            }
        }

        else
        {
            *error = 1;
            printf("\n");
            printf("Error in Expenses.json : Invalid currency format in line number: %d.\n", lineNumber);
            printf("Please ensure that the currency is a 3-letter string. \n");
            printf("\n");
            return;
        }

        /* Move ptr to next token*/
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Check if the next character is a comma, indicating the start of the next key-value pair */
        if (*ptr == ',')
        {
            ptr++;
        }

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }
        lineNumber++;

        current_pos = ptr;
        expected_key = "\"amount_spent\"";
        if (!isValidExpenseKey(current_pos, &next_pos, expected_key))
        {
            *error = 1;
            printf("Error in %s key format in line number: %d\n", expected_key, lineNumber);
            return;
        }
        ptr = next_pos;

        /* Skip whitespace */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check if the next character is ':' */
        if (*ptr != ':')
        {
            *error = 1;
            printf("Error: Invalid JSON format. File does not contain : between key and value at line number: %d.\n", lineNumber);
            return;
        }

        /* Move ptr to the next character after ':' */
        ptr++;

        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        sscanf(ptr, "%f", &amount_spent);

        if (!isValidAmountSpent(amount_spent))
        {
            printf("\n");
            printf("Error in Expenses.json : Invalid amount_spent format in line number: %d.\n", lineNumber + 1);
            printf("Please ensure that the amount_spent is a float or an integer value.\n");
            printf("\n");
        }

        expense->amount_spent = amount_spent;

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
    int i;
    /* Check if description is an empty string */

    for (i = 0; description[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)description[i]))
        {

            return 1;
        }
    }

    return 0;
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

int isValidExpenseKey(const char *current_pos, const char **next_pos, char *expected_key)
{
    size_t key_len;
    /* Skip leading whitespace */
    while (*current_pos == ' ' || *current_pos == '\t' || *current_pos == '\n' || *current_pos == '\r')
    {
        current_pos++;
    }

    /* Check if the JSON starts with the expected key */
    key_len = strlen(expected_key); /* To account for the last character */

    if (strncmp(current_pos, expected_key, key_len) != 0)
    {
        return 0;
    }

    /* Move ptr to the end of the key */
    current_pos += key_len;

    *next_pos = current_pos;
    return 1;
}

/*
int main()
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
    char *jsonContent = (char *)malloc(fileSize + 1);
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        fclose(expenses_file);
        return 1;
    }

    // Read the JSON content into the allocated memory
    size_t bytesRead = fread(jsonContent, 1, fileSize, expenses_file);

    // Null-terminate the JSON content
    jsonContent[fileSize] = '\0';

    // Close the file
    fclose(expenses_file);

    // Array to store parsed expenses
    Expenses expenses[MAX_EXPENSES];
    int num_expenses = 0;

    // Parse the JSON content
    memset(expenses, 0, sizeof(expenses));
    int error = 0;

    parse_expensesjson(jsonContent, expenses, &num_expenses, &error);

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

    return 0;
} */
