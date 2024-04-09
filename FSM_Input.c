#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_Input.h"

/*This file parses and processes data from two input JSON files representing accounts and expenses respectively.*/

void initInputFSM(InputFSM *inputFsm)
{
    inputFsm->currentState = INITIAL_STATE;
    inputFsm->accounts;
    inputFsm->expenses;
}

void processInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile)
{
    long fileSize;
    long fileSize2;
    FILE *expenses_file;
    char *jsonContent;
    char *jsonContent2;
    int num_expenses = 0;
    int num_accounts = 0;
    

    while ((inputFSM->currentState != COMPLETED) && (inputFSM->currentState != ERROR))
    {

        switch (inputFSM->currentState)
        {
        case INITIAL_STATE:
        {
            /* parse account json */
            FILE *accounts_file = fopen(accountFile, "r");
            if (accounts_file == NULL)
            {
                printf("Error: Unable to open %s\n", accountFile);
                inputFSM->currentState = ERROR;
            }

            /* Get the file size */
            fseek(accounts_file, 0L, SEEK_END);
            fileSize = ftell(accounts_file);
            rewind(accounts_file);

            /* Allocate memory for JSON content */
            jsonContent = (char *)malloc(fileSize + 1); /* +1 for null terminator */
            if (jsonContent == NULL)
            {
                printf("Error: Unable to allocate memory\n");
                fclose(accounts_file);
                inputFSM->currentState = ERROR;
            }

            /* Read the JSON content into the allocated memory */
            fread(jsonContent, 1, fileSize, accounts_file);

            /* Null-terminate the JSON content */
            jsonContent[fileSize] = '\0';

            /* Close the file */
            fclose(accounts_file);

            /* Parse the JSON content */

            parse_accountsjson(jsonContent, inputFSM->accounts, &num_accounts);

            /* parse expense json */

            expenses_file = fopen(expenseFile, "r");
            if (expenses_file == NULL)
            {
                printf("Error: Unable to open %s\n", expenseFile);
                inputFSM->currentState = ERROR;
            }

            /* Get the file size */
            fseek(expenses_file, 0L, SEEK_END);
            fileSize2 = ftell(expenses_file);
            rewind(expenses_file);

            /* Allocate memory for JSON content */
            jsonContent2 = (char *)malloc(fileSize2 + 1); /* +1 for null terminator */
            if (jsonContent2 == NULL)
            {
                printf("Error: Unable to allocate memory\n");
                fclose(expenses_file);
                inputFSM->currentState = ERROR;
            }

            /* Read the JSON content into the allocated memory */
            fread(jsonContent2, 1, fileSize2, expenses_file);

            /* Null-terminate the JSON content */
            jsonContent2[fileSize2] = '\0';

            /* Close the file */
            fclose(expenses_file);

            parse_expensesjson(jsonContent2, inputFSM->expenses, &num_expenses);
            printf("account.json file and expenses.json file are successfully parsed.\n");

            inputFSM->currentState = Validate_ACCOUNT;

            break;
        }

        case Validate_ACCOUNT:
        {
            if (validateAccounts(inputFSM->accounts, num_accounts))
            {
                printf("All accounts fields are validated to be true.\n");
                inputFSM->currentState = Validate_EXPENSES;
            }

            else
            {

                inputFSM->currentState = ERROR;
            }

            break;
        }

        case Validate_EXPENSES:
        {
            if (validateExpenses(inputFSM->expenses, num_expenses))
            {
                printf("All expenses fields are validated to be true.\n");
                inputFSM->currentState = COMPLETED;
            }

            else
            {
                inputFSM->currentState = ERROR;
            }
            break;
        }

        case COMPLETED:
            /*Accepting state,exiting the loop*/
            break;

        case ERROR:
            /*Error state, exiting the loop*/
            break;
        }
    }
}

int runInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile)
{
    processInputState(inputFSM, accountFile, expenseFile);
    return inputFSM->currentState == COMPLETED;
}

/* Print all the information from accounts and expenses */
/* // void printAccountsAndExpenses(InputFSM *inputFSM)
// {
//     if (inputFSM->accounts != NULL)
//     {
//         int numAccounts;
//         int i;
//         printf("Accounts:\n");
//         numAccounts = cJSON_GetArraySize(inputFSM->accountsJson);
//         for (i = 0; i < numAccounts; ++i)
//         {
//             printf("Account ID: %d\n", inputFSM->accounts[i].account_id);
//             printf("User ID: %d\n", inputFSM->accounts[i].user_id);
//             printf("Name: %s\n", inputFSM->accounts[i].name);
//             printf("Currency: %s\n", inputFSM->accounts[i].default_currency);
//             printf("Balance: %.2f\n", inputFSM->accounts[i].balance);
//             printf("\n");
//         }
//     }
//     if (inputFSM->expenses != NULL)
//     {
//         int numExpenses;
//         int i;
//         printf("Expenses:\n");

//         numExpenses = cJSON_GetArraySize(inputFSM->expensesJson);

//         for (i = 0; i < numExpenses; i++)
//         {
//             printf("Account ID: %d\n", inputFSM->expenses[i].account_id);
//             printf("Date: %s\n", inputFSM->expenses[i].date);
//             printf("Description: %s\n", inputFSM->expenses[i].description);
//             printf("Currency: %s\n", inputFSM->expenses[i].currency);
//             printf("Amount Spent: %.2f\n", inputFSM->expenses[i].amount_spent);
//             printf("\n");
//         }
//     }
// }*/

int main(int argc, char const *argv[])
{
    char *accountFile;
    char *expenseFile;
    InputFSM inputFSM;
    initInputFSM(&inputFSM); /*Initialize the fsm*/

    accountFile = "ParserAccounts/Accounts.json";
    expenseFile = "ParserExpenses/Expenses.json";

    /*Run the fsm to see if it is in the accepting state(successfully parsing the 2 files)*/

    if (runInputState(&inputFSM, accountFile, expenseFile))
    {
        printf("Successfully processed %s and %s.", accountFile, expenseFile);
        /* printAccountsAndExpenses(&inputFSM); */
    }

    return 0;
}
