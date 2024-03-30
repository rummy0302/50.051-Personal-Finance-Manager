#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_output.h"
#include "HTML_output/Page2-UserBarGraph.h"
#include "HTML_output/Page3-AccOverallExpenses.h"
#include "HTML_output/Page4-AccYearlyExpenses.h"
#include "HTML_output/Common.h"

/* This file is the entry point for users to use this program
        1.Page flow: Page1 Mainpage->Page2 UserBarGraph->Page3 AccOverallExpenses->Page AccYearlyExpenses.
        2.Enter 'exit' at any time to exit the program.
*/

AccountInfo accountInfo[MAX_ACCOUNTS];
int numAccounts;
int numExpenses;
Expenses *expenses;
Account *accounts;
cJSON *json_expenses;
cJSON *json_accounts;

int isValidAccount(int accountId, int userId)
{
    int i;
    for (i = 0; i < numAccounts; i++)
    {
        if (accountInfo[i].account_id == accountId && accountInfo[i].user_id == userId)
        {
            return 1;
        }
    }
    return 0;
}

int isValidUser(int userId)
{
    int i;
    for (i = 0; i < numAccounts; i++)
    {
        if (accountInfo[i].user_id == userId)
        {
            return 1;
        }
    }
    return 0;
}

int isYearInList(int *years, int numYears, int year)
{
    int i;
    for (i = 0; i < numYears; i++)
    {
        if (years[i] == year)
        {
            return 1;
        }
    }
    return 0;
}

void initOutputFSM(OutputFSM *OutputFsm)
{
    OutputFsm->currentState = CHECK_USERID;
    OutputFsm->userID = -1;
    OutputFsm->accountID = -1;
}

void openHTMLPage(const char *pageName)
{
    char command[100];
#ifdef _WIN32
    sprintf(command, "start %s", pageName);
    system(command);
#elif __linux__ || __APPLE__
#ifdef __linux__
    sprintf(command, "xdg-open %s &", pageName);
#elif __APPLE__
    sprintf(command, "open %s &", pageName);
#endif
    system(command);
#endif
}

void handleInput(OutputFSM *OutputFSM, int input)
{
    switch (OutputFSM->currentState)
    {

    case CHECK_USERID:
    {

        int i;
        json_expenses = parseExpensesJSONfile("ParserExpenses/Expenses.json");
        json_accounts = parseAccountsJSONfile("ParserAccounts/Accounts.json");
        expenses = processExpensesData(json_expenses, &numExpenses);
        cJSON_Delete(json_expenses);
        accounts = processAccountsData(json_accounts, &numAccounts);
        cJSON_Delete(json_accounts);

        for (i = 0; i < numAccounts; i++)
        {
            accountInfo[i].account_id = accounts[i].account_id;
            accountInfo[i].user_id = accounts[i].user_id;
        }

        /*check if the input is valid userID*/
        if (isValidUser(input))
        {
            FILE *htmlFile;
            htmlFile = fopen("HTML_output/Page2-userBarGraph.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create Page2-userBarGraph file\n");
                exit(EXIT_FAILURE);
            }

            calculateExpenseTotal(expenses, numExpenses);

            printAllExpenseTotals(input, expenses, numExpenses, accounts, numAccounts, htmlFile);

            free(expenses);
            free(accounts);

            /*open the UserBar graph html*/
            openHTMLPage("HTML_output/Page2-userBarGraph.html");

            printf("Enter the account ID: ");
            OutputFSM->currentState = CHECK_ACCOUNTID;
            OutputFSM->userID = input; /*store the userID which can be used to check the accountID*/
        }

        else
        {
            /*else stay at current state and current page (mainPage)*/
            printf("The user ID %d does not exist!\n", input);
            printf("Please enter the user ID again: ");
        }

        break;
    }

    case CHECK_ACCOUNTID:

    {

        json_expenses = parseExpensesJSONfile("ParserExpenses/Expenses.json");
        json_accounts = parseAccountsJSONfile("ParserAccounts/Accounts.json");
        expenses = processExpensesData(json_expenses, &numExpenses);
        cJSON_Delete(json_expenses);
        accounts = processAccountsData(json_accounts, &numAccounts);
        cJSON_Delete(json_accounts);

        if (isValidAccount(input, OutputFSM->userID))
        {

            /*generate the corresponding expense graph inside the html and open the html page*/

            FILE *htmlFile;

            htmlFile = fopen("HTML_output/Page3-AccOverallExpenses.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create Page3-AccOverallExpenses file\n");
                exit(EXIT_FAILURE);
            }

            /*store the accountID which can be used to check the year*/
            OutputFSM->accountID = input;

            categorizeExpenses(expenses, numExpenses);

            generateHTMLForAccount(expenses, numExpenses, input, htmlFile);

            fclose(htmlFile);
            free(expenses);
            free(accounts);

            openHTMLPage("HTML_output/Page3-AccOverallExpenses.html");
            OutputFSM->currentState = CHECK_YEAR;
            printf("Enter the year you want to review (eg.2024): ");
        }

        else
        {
            /*else stay at current state and current page(UserId graph Page)*/
            printf("The account ID %d does not exist!\n", input);
            printf("Please enter the account ID anagin: ");
        }

        break;
    }

    case CHECK_YEAR:
    {

        int numYears;
        int *years;
        json_expenses = parseExpensesJSONfile("ParserExpenses/Expenses.json");
        json_accounts = parseAccountsJSONfile("ParserAccounts/Accounts.json");
        expenses = processExpensesData(json_expenses, &numExpenses);
        cJSON_Delete(json_expenses);
        accounts = processAccountsData(json_accounts, &numAccounts);
        cJSON_Delete(json_accounts);

        years = getYearsFromExpenses(expenses, numExpenses, OutputFSM->accountID, &numYears);

        if (isYearInList(years, numYears, input))
        {
            FILE *htmlFile;
            htmlFile = fopen("HTML_output/Page4-AccYearlyExpenses.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create HTML file\n");
                exit(EXIT_FAILURE);
            }

            categorizeExpensesByYear(expenses, numExpenses, input);

            generateHTMLForAccountYear(expenses, numExpenses, OutputFSM->accountID, htmlFile, input);

            fclose(htmlFile);
            free(expenses);
            free(accounts);

            openHTMLPage("HTML_output/Page4-AccYearlyExpenses.html");

            printf("Please enter 'exit' to terminate the Finance Overview Session: ");
        }

        else
        {
            /*else stay at current state and current page(ExpenseGraph Page)*/
            printf("There are no existing expenses for the year %d!\n", input);
            printf("Please enter the year again (eg.2024): ");
        }

        break;
    }
    }
}

int main(int argc, char const *argv[])
{
    char inputStr[100]; /*Used to read an entire line of input*/
    OutputFSM fsm;
    initOutputFSM(&fsm);

    openHTMLPage("HTML_output/Page1-MainPage.html");
    printf("Please enter 'exit' to terminate the Finance Overview Session at any time. \n");
    printf("Enter the user ID: ");

    while (1)
    {
        if (fgets(inputStr, sizeof(inputStr), stdin)) /*Read the line of input*/
        {
            int input;
            char *endPtr;
            /*heck if the user enter the 'exit' command*/
            if (strncmp(inputStr, "exit", 4) == 0)
            {
                break;
            }

            input = (int)strtol(inputStr, &endPtr, 10); /*Convert the input to an integer*/

            /*Check if the input is a integer before implementing handleInput function*/
            if (endPtr != inputStr && *endPtr == '\n')
            {
                handleInput(&fsm, input);
            }
            else
            {
                printf("Invalid input! Please enter an integer: ");
            }
        }
    }

    return 0;
}
