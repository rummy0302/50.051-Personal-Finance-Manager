#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_Input.h"

/*This file parses and processes data from two input JSON files representing accounts and expenses respectively.*/

void initInputFSM(InputFSM *inputFsm)
{
    inputFsm->currentState = INITIAL_STATE;
    inputFsm->accountsJson = NULL;
    inputFsm->expensesJson = NULL;
    inputFsm->accounts = NULL;
    inputFsm->expenses = NULL;
}

void processInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile)
{
    while ((inputFSM->currentState != COMPLETED) && (inputFSM->currentState != ERROR))
    {
        switch (inputFSM->currentState)
        {
        case INITIAL_STATE:
        {
            inputFSM->accountsJson = parseAccountsJSONfile(accountFile);
            inputFSM->expensesJson = parseExpensesJSONfile(expenseFile);

            if (inputFSM->accountsJson != NULL && inputFSM->expensesJson != NULL)
            {
                printf("Successfully parsed %s file and %s file to cJSON objects. \n", accountFile, expenseFile);
                /*Transition to the next state*/
                inputFSM->currentState = READING_ACCOUNT;
            }

            else
            {
                printf("Failed in parsed json files to cJSON objects, please try again.\n");
                inputFSM->currentState = ERROR;
            }

            break;
        }

        case READING_ACCOUNT:
        {
            int numAccounts;
            inputFSM->accounts = processAccountsData(inputFSM->accountsJson, &numAccounts);
            if (inputFSM->accounts != NULL)
            {
                printf("Successfully processed Accounts cJSON object into Accounts array.\n");
                /*Transition to the next state*/
                inputFSM->currentState = READING_EXPENSES;
            }
            else
            {
                printf("Error: Unable to process Accounts cJSON object into Accounts array, please try again.\n");
                inputFSM->currentState = ERROR;
            }
            break;
        }

        case READING_EXPENSES:
        {
            int numExpenses;
            inputFSM->expenses = processExpensesData(inputFSM->expensesJson, &numExpenses);

            if (inputFSM->expenses != NULL)
            {
                printf("Successfully processed Expenses cJSON object into Expenses array.\n");
                /*Transition to the next state*/
                inputFSM->currentState = COMPLETED;
            }
            else
            {
                printf("Error: Unable to process Expenses cJSON object into Expenses array, please try again.\n");
                inputFSM->currentState = ERROR;
            }
            break;

        case COMPLETED:
            /*Accepting state,exiting the loop*/
            break;
        }

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
void printAccountsAndExpenses(InputFSM *inputFSM)
{
    if (inputFSM->accounts != NULL)
    {
        int numAccounts;
        int i;
        printf("Accounts:\n");
        numAccounts = cJSON_GetArraySize(inputFSM->accountsJson);
        for (i = 0; i < numAccounts; ++i)
        {
            printf("Account ID: %d\n", inputFSM->accounts[i].account_id);
            printf("User ID: %d\n", inputFSM->accounts[i].user_id);
            printf("Name: %s\n", inputFSM->accounts[i].name);
            printf("Currency: %s\n", inputFSM->accounts[i].default_currency);
            printf("Balance: %.2f\n", inputFSM->accounts[i].balance);
            printf("\n");
        }
    }
    if (inputFSM->expenses != NULL)
    {
        int numExpenses;
        int i;
        printf("Expenses:\n");

        numExpenses = cJSON_GetArraySize(inputFSM->expensesJson);

        for (i = 0; i < numExpenses; i++)
        {
            printf("Account ID: %d\n", inputFSM->expenses[i].account_id);
            printf("Date: %s\n", inputFSM->expenses[i].date);
            printf("Description: %s\n", inputFSM->expenses[i].description);
            printf("Currency: %s\n", inputFSM->expenses[i].currency);
            printf("Amount Spent: %.2f\n", inputFSM->expenses[i].amount_spent);
            printf("\n");
        }
    }
}

void cleanupInputFSM(InputFSM *inputFSM)
{
    if (inputFSM->accountsJson != NULL)
    {
        cJSON_Delete(inputFSM->accountsJson);
        inputFSM->accountsJson = NULL;
    }
    if (inputFSM->expensesJson != NULL)
    {
        cJSON_Delete(inputFSM->expensesJson);
        inputFSM->expensesJson = NULL;
    }
    if (inputFSM->accounts != NULL)
    {
        free(inputFSM->accounts);
        inputFSM->accounts = NULL;
    }
    if (inputFSM->expenses != NULL)
    {
        free(inputFSM->expenses);
        inputFSM->expenses = NULL;
    }
}

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
        /*printAccountsAndExpenses(&inputFSM);*/
    }

    cleanupInputFSM(&inputFSM);
    return 0;
}
