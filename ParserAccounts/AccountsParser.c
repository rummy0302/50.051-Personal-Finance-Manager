#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AccountsParser.h"

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

int isValidname(const char name[])
{

    int i;
    /* Check if name is empty */

    for (i = 0; name[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)name[i]))
        {

            return 1;
        }
    }

    return 0;
}

int isValidDefaultCurrency(const char *currency)
{
    /* Check if the currency contains space or is not 3 digit */

    int i;

    for (i = 0; currency[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)currency[i]))
        {

            return 1;
        }
    }

    return 0;
}

int isValidBalance(const float balance)
{
    /* Check if the balance is empty or it is a string */

    if (balance == 0.00)
        return 0;

    return 1;
}

int isValidAccountKey(const char *current_pos, const char **next_pos, char *expected_key)
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

void parse_accountsjson(const char *json, Account accounts[], int *num_accounts, int *error)
{
    const char *ptr = json;
    int lineNumber = 1;
    const char *current_pos;
    const char *next_pos = NULL;
    char *expected_key;

    char name[50];
    char default_currency[4];
    float balance = 0.0;
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

    ptr++; /* Skip [ */

    /* Parse array elements */
    while (*ptr != '\0' && *ptr != ']' && *num_accounts < MAX_ACCOUNTS)
    {
        /* Parse account object */
        Account *account = &accounts[*num_accounts];

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
        if (!isValidAccountKey(current_pos, &next_pos, expected_key))
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

        /* Skip whitespace after ':' */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check format of account id: */
        if (!isValidAccountsID(atoi(ptr)))
        {
            *error = 1;
            printf("\n");
            printf("Error in Accounts.json : Invalid account_id format at line number: %d.\n", lineNumber);
            printf("Please ensure that the account_id is of type int and is within the range of 1-100.\n");
            printf("\n");
            return;
        }

        /* Parse numeric value */
        account->account_id = atoi(ptr);

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
        expected_key = "\"user_id\"";
        if (!isValidAccountKey(current_pos, &next_pos, expected_key))
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

        /* Skip whitespace after ':' */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Check format of user id : */
        if (!isValidUserID(atoi(ptr)))
        {
            *error = 1;
            printf("\n");
            printf("Error in Accounts.json : Invalid user_id format at line number: %d.\n", lineNumber);
            printf("Please ensure that the user_id is of type int and is within the range of 1-50.\n");
            printf("\n");
            return;
        }

        /* Parse numeric value */
        account->user_id = atoi(ptr);

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
        expected_key = "\"name\"";
        if (!isValidAccountKey(current_pos, &next_pos, expected_key))
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

        /* Skip whitespace after ':' */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        result = sscanf(ptr, "\"%[^\"]\"", name);

        if (result == 1)
        {

            if (!isValidname(name))

            {
                *error = 1;
                printf("\n");
                printf("Error in Accounts.json : Invalid name format at line number: %d.\n", lineNumber);
                printf("Please ensure that the name is a not a empty string.\n");
                printf("\n");
                return;
            }

            strcpy(account->name, name);
        }
        else
        {
            *error = 1;
            printf("\n");
            printf("Error in Accounts.json : Invalid name format at line number: %d.\n", lineNumber);
            printf("Please ensure that the name is a string.\n");
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
        expected_key = "\"default_currency\"";
        if (!isValidAccountKey(current_pos, &next_pos, expected_key))
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

        /* Skip whitespace after ':' */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse string value */
        result = sscanf(ptr, "\"%[^\"]\"", default_currency);

        if (result == 1)
        {
            if (!isValidDefaultCurrency(default_currency))
            {
                *error = 1;
                printf("\n");
                printf("Error in Accounts.json : Invalid currency format at line number: %d.\n", lineNumber);
                printf("Please ensure that the currency is not an empty string. \n");
                printf("\n");
                return;
            }
        }
        else
        {
            *error = 1;
            printf("\n");
            printf("Error in Accounts.json : Invalid currency format at line number: %d.\n", lineNumber);
            printf("Please ensure that the currency is a string. \n");
            printf("\n");
            return;
        }

        strcpy(account->default_currency, default_currency);

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
        expected_key = "\"balance\"";
        if (!isValidAccountKey(current_pos, &next_pos, expected_key))
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

        /* Skip whitespace after ':' */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')
        {
            ptr++;
        }

        /* Parse numeric value */
        sscanf(ptr, "%f", &balance);

        if (!isValidBalance(balance))
        {
            *error = 1;
            printf("\n");
            printf("Error in Accounts.json : Invalid balance format at line number: %d.\n", lineNumber);
            printf("Please ensure that the balance is a float.\n");
            printf("\n");
            return;
        }

        account->balance = balance;

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

        lineNumber++;

        /* Increment the number of account parsed */
        (*num_accounts)++;
    }
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
    char *jsonContent = (char *)malloc(fileSize + 1);
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        fclose(accounts_file);
        return 1;
    }

    // Read the JSON content into the allocated memory
    size_t bytesRead = fread(jsonContent, 1, fileSize, accounts_file);

    // Null-terminate the JSON content
    jsonContent[fileSize] = '\0';

    // Close the file
    fclose(accounts_file);

    // Array to store parsed expenses
    Account accounts[MAX_ACCOUNTS];
    int num_accounts = 0;

    // Parse the JSON content
    memset(accounts, 0, sizeof(accounts));
    int error = 0;

    parse_accountsjson(jsonContent, accounts, &num_accounts, &error);

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

    free(jsonContent);

    return 0;
} */
