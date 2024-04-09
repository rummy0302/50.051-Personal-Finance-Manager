#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "AccountsParser.h"

void parse_accountsjson(const char *json, Account accounts[], int *num_accounts)
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

    ptr++; /* Skip [ */

    /* Parse array elements */
    while (*ptr != '\0' && *ptr != ']' && *num_accounts < MAX_ACCOUNTS)
    {
        /* Parse account object */
        Account *account = &accounts[*num_accounts];

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
        account->account_id = atoi(ptr);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse user_id */
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

        /* Parse numeric value */
        account->user_id = atoi(ptr);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse name */
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

        /* Parse string value */
        sscanf(ptr, "\"%[^\"]\"", account->name);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse default_currency */
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

        /* Parse string value */
        sscanf(ptr, "\"%[^\"]\"", account->default_currency);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Parse amount_spent */
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

        /* Parse numeric value */
        sscanf(ptr, "%f", &account->balance);

        /* Move ptr to next token */
        while (*ptr != ',' && *ptr != ']')
        {
            ptr++;
        }

        /* Increment the number of account parsed */
        (*num_accounts)++;
    }
}

int isValidAccountsID(const int account_id)
{
    /* Check if the accountID within the range and if it is an integer */
    if (account_id < 1 || account_id > 100)
        return 0;

    return 1;
}

int isValidUserID(const int user_id)
{
    /* Check if the userID within the range and if it is an integer */
    if (user_id < 1 || user_id > 50)
        return 0;

    return 1;
}

int isValidname(const char *name)
{
    /* Check if description is not empty or it is an integer */
    if (name[0] == '\0')
        return 0;
    return 1;
}

int isValidDefaultCurrency(const char *currency)
{
    /* Check if the currency is empty or is not 3 digit */

    if (strlen(currency) != 3)
        return 0;

    return 1;
}

int isValidBalance(const float balance)
{
    /* Check if the balance is empty or it is a string */

    if (balance == 0.00)
        return 0;

    return 1;
}

int validateAccounts(Account accounts[], int num_accounts)

{
    int lineNumber;
    int i;
    int isAllValid = 1;
    lineNumber = 1;
    

    
    for (i = 0; i < num_accounts; i++)
    {
        int valid = 1;
        lineNumber++;

        if (!isValidAccountsID(accounts[i].account_id))
        {

            printf("\n");
            printf("Error in Accounts.json : Invalid account_id format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the account_id is of type int and is within the range of 1-100.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidUserID(accounts[i].user_id))
        {
            printf("\n");
            printf("Error in Accounts.json : Invalid user_id format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the user_id is of type int and is within the range of 1-50.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }

        lineNumber++;

        if (!isValidname(accounts[i].name))
        {
            printf("\n");
            printf("Error in Accounts.json : Invalid name format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the name is a string.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidDefaultCurrency(accounts[i].default_currency))
        {
            printf("\n");
            printf("Error in Accounts.json : Invalid currency format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the currency is a 3-letter string. \n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (!isValidBalance(accounts[i].balance))
        {
            printf("\n");
            printf("Error in Accounts.json : Invalid balance format at entry number: %d and line number: %d.\n", i + 1, lineNumber + 1);
            printf("Please ensure that the balance is a float.\n");
            printf("\n");
            valid = 0;
            isAllValid = 0;
        }
        lineNumber++;

        if (valid)
        {
            /* printf("Account %d is valid.\n", i + 1); */
        }
        else
        {
            printf("Account %d has validation errors.\n", i + 1);
        }
    }

    return isAllValid;
}

/*
int main()
{
    const char *filename = "Accounts.json";
    FILE *accounts_file = fopen(filename, "r");
    if (accounts_file == NULL)
    {
        printf("Error: Unable to open %s\n", filename);
        return 1;
    }

    // Get the file size
    fseek(accounts_file, 0L, SEEK_END);
    long fileSize = ftell(accounts_file);
    rewind(accounts_file);

    // Allocate memory for JSON content
    char *jsonContent = (char *)malloc(fileSize + 1); // +1 for null terminator
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        fclose(accounts_file);
        return 1;
    }

    // Read the JSON content into the allocated memory
    size_t bytesRead = fread(jsonContent, 1, fileSize, accounts_file);
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
    fclose(accounts_file);

    // Array to store parsed expenses
    Account accounts[MAX_ACCOUNTS];
    int num_accounts = 0;

    // Parse the JSON content
    memset(accounts, 0, sizeof(accounts));

    parse_accountsjson(jsonContent, accounts, &num_accounts);

    // Access the parsed expenses
    for (int i = 0; i < num_accounts; i++)
    {
        printf("Account ID: %d\n", accounts[i].account_id);
        printf("User ID: %d\n", accounts[i].user_id);
        printf("Name: %s\n", accounts[i].name);
        printf("Default_Currency: %s\n", accounts[i].default_currency);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("\n");
    }

    // Free the allocated memory
    free(jsonContent);

    validateAccounts(accounts, num_accounts);

    return 0;
} */
