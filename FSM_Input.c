#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_Input.h"

/*This file parses and and validates data from two input JSON files representing accounts and expenses respectively.*/

void initInputFSM(InputFSM *inputFsm)
{
    inputFsm->currentState = INITIAL_STATE;

    memset(inputFsm->accounts, 0, sizeof(inputFsm->accounts));
    memset(inputFsm->expenses, 0, sizeof(inputFsm->expenses));
}

void processInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile)
{
    long fileSize;
    long fileSize2;
    FILE *expenses_file;
    FILE *accounts_file;
    char *accountjsonContent;
    char *expensejsonContent;
    int num_expenses = 0;
    int num_accounts = 0;
    int accountError = 0;
    int expenseError = 0;

    while ((inputFSM->currentState != COMPLETED) && (inputFSM->currentState != ERROR))
    {

        switch (inputFSM->currentState)
        {
        case INITIAL_STATE:
        {
            /* read account json */
            accounts_file = fopen(accountFile, "r");
            if (accounts_file == NULL)
            {
                printf("Error: Unable to open %s\n", accountFile);
                inputFSM->currentState = ERROR;
            }

            /* read expense json */
            expenses_file = fopen(expenseFile, "r");
            if (expenses_file == NULL)
            {
                printf("Error: Unable to open %s\n", expenseFile);
                inputFSM->currentState = ERROR;
            }

            printf("account.json file and expenses.json file are successfully read.\n");

            inputFSM->currentState = PARSE_ACCOUNT;

            break;
        }

        case PARSE_ACCOUNT:
        {
            /* Get the file size */
            fseek(accounts_file, 0L, SEEK_END);
            fileSize = ftell(accounts_file);
            rewind(accounts_file);

            /* Allocate memory for JSON content */
            accountjsonContent = (char *)malloc(fileSize + 1); /* +1 for null terminator */
            if (accountjsonContent == NULL)
            {
                printf("Error: Unable to allocate memory\n");
                fclose(accounts_file);
                inputFSM->currentState = ERROR;
            }

            /* Read the JSON content into the allocated memory */
            fread(accountjsonContent, 1, fileSize, accounts_file);

            /* Null-terminate the JSON content */
            accountjsonContent[fileSize] = '\0';

            /* Close the file */
            fclose(accounts_file);

            /* Parse and validate the fields of json file */
            parse_accountsjson(accountjsonContent, inputFSM->accounts, &num_accounts, &accountError);

            if (!accountError)
            {
                printf("All accounts fields are successfully parsed and validated.\n");
                inputFSM->currentState = PARSE_EXPENSES;
            }

            else
            {

                inputFSM->currentState = ERROR;
                printf("Failed in parse and validate the fields in %s", accountFile);
            }

            break;
        }

        case PARSE_EXPENSES:
        {
            /* Get the file size */
            fseek(expenses_file, 0L, SEEK_END);
            fileSize2 = ftell(expenses_file);
            rewind(expenses_file);

            /* Allocate memory for JSON content */
            expensejsonContent = (char *)malloc(fileSize2 + 1); /* +1 for null terminator */
            if (expensejsonContent == NULL)
            {
                printf("Error: Unable to allocate memory\n");
                fclose(expenses_file);
                inputFSM->currentState = ERROR;
            }

            /* Read the JSON content into the allocated memory */
            fread(expensejsonContent, 1, fileSize2, expenses_file);

            /* Null-terminate the JSON content */
            expensejsonContent[fileSize2] = '\0';

            /* Close the file */
            fclose(expenses_file);

            parse_expensesjson(expensejsonContent, inputFSM->expenses, &num_expenses, &expenseError);

            if (!expenseError)
            {
                printf("All expenses fields are successfully parsed and validated.\n");
                inputFSM->currentState = COMPLETED;
            }

            else
            {
                inputFSM->currentState = ERROR;
                printf("Failed in parse and validate the fields in %s", expenseFile);
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

int main(int argc, char const *argv[])
{
    char *accountFile;
    char *expenseFile;
    InputFSM inputFSM;
    initInputFSM(&inputFSM); /*Initialize the fsm*/

    accountFile = "ParserAccounts/Accounts.json";
    expenseFile = "ParserExpenses/Expenses.json";

    /*Run the fsm to see if it is in the accepting state(successfully read and parsed the 2 files)*/

    if (runInputState(&inputFSM, accountFile, expenseFile))
    {
        printf("Successfully processed %s and %s.", accountFile, expenseFile);
    }

    return 0;
}
