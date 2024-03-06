#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_Input.h"

void initInputFSM(InputFSM *inputFsm)
{
    inputFsm->currentState = INITIAL_STATE;
    inputFsm->accountsJson = parseAccountsJSONfile("ParserAccounts/Accounts.json");
    inputFsm->expensesJson = parseExpensesJSONfile("Expenses.json");
}

void processInputState(InputFSM *inputFSM)
{
    while (inputFSM->currentState != COMPLETED)
    {
        switch (inputFSM->currentState)
        {
        case INITIAL_STATE:

            if (inputFSM->accountsJson != NULL && inputFSM->expensesJson != NULL)
            {
                printf("Successfully parsed Accounts.json and Expenses.json");
                // Transition to the next state
                inputFSM->currentState = READING_ACCOUNT;
            }
            // Stay in Idle state

            break;

        case READING_ACCOUNT:
            int numAccounts;
            inputFSM->accounts = processAccountsData(inputFSM->accountsJson, &numAccounts);
            if (inputFSM->accounts != NULL)
            {
                printf("Successfully processed accounts\n");
                // Transition to the next state
                inputFSM->currentState = READING_EXPENSES;
            }
            else
            {
                printf("Error processing accounts\n");
                // Stay in ReadingAccounts
            }
            break;

        case READING_EXPENSES:
            int numExpenses;
            inputFSM->expenses = processExpensesData(inputFSM->expensesJson, &numExpenses);

            if (inputFSM->expenses != NULL)
            {
                printf("Successfully processed expenses\n");
                // Transition to the next state
                inputFSM->currentState = COMPLETED;
            }
            else
            {
                printf("Error processing expenses\n");
                // Stay in ReadingExpenses
            }
            break;

        case COMPLETED:
            // Accepting state,exiting the loop
            break;
        }
    }
}

int runInputState(InputFSM *inputFSM)
{
    processInputState(inputFSM);
    return inputFSM->currentState == COMPLETED;
}

int main(int argc, char const *argv[])
{
    InputFSM inputFSM;
    initInputFSM(&inputFSM);
    if (runInputState(&inputFSM))
    {
        printf("Successfully processing Accounts and Expenses");
    }
    else
        printf("Failed in processing,please try again");
    return 0;
}
