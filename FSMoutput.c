#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM_output.h"
#include "HTML_output/Page2-UserBarGraph.h"
#include "HTML_output/Page3-AccOverallExpenses.h"
#include "HTML_output/Page4-AccYearlyExpenses.h"
#include "HTML_output/Common.h"
#include "ParserExpenses/ExpenseParser.h"
#include "ParserAccounts/AccountsParser.h"

/* This file is the entry point for users to use this program
        1.Page flow: Page1 Mainpage->Page2 UserBarGraph->Page3 AccOverallExpenses->Page AccYearlyExpenses.
        2.Enter 'exit' at any time to exit the program.
*/

AccountInfo accountInfo[MAX_ACCOUNTS];
int num_expenses = 0;
int num_accounts = 0;
Account accounts[MAX_ACCOUNTS];
Expenses expenses[MAX_EXPENSES];

int isValidAccount(int accountId, int userId)
{
    int i;
    for (i = 0; i < num_expenses; i++)
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
    for (i = 0; i < num_accounts; i++)
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

    int i;

    const char *accountsfilename = "ParserAccounts/Accounts.json";
    const char *expensesfilename = "ParserExpenses/Expenses.json";
    FILE *accounts_file = fopen(accountsfilename, "r");

    fseek(accounts_file, 0L, SEEK_END);
    long accountsexpensesfileSize = ftell(accounts_file);
    rewind(accounts_file);

    char *accountsjsonContent = (char *)malloc(accountsexpensesfileSize + 1);

    size_t accountsbytesRead = fread(accountsjsonContent, 1, accountsexpensesfileSize, accounts_file);

    accountsjsonContent[accountsexpensesfileSize] = '\0';

    fclose(accounts_file);
    memset(accounts, 0, sizeof(accounts));

    parse_accountsjson(accountsjsonContent, accounts, &num_accounts);

    free(accountsjsonContent);

    FILE *expenses_file = fopen(expensesfilename, "r");

    fseek(expenses_file, 0L, SEEK_END);
    long expensesfileSize = ftell(expenses_file);
    rewind(expenses_file);

    char *expensesjsonContent = (char *)malloc(expensesfileSize + 1);

    size_t expensesbytesRead = fread(expensesjsonContent, 1, expensesfileSize, expenses_file);

    expensesjsonContent[expensesfileSize] = '\0';

    fclose(expenses_file);

    memset(expenses, 0, sizeof(expenses));

    parse_expensesjson(expensesjsonContent, expenses, &num_expenses);

    free(expensesjsonContent);

    for (i = 0; i < num_accounts; i++)
    {
        accountInfo[i].account_id = accounts[i].account_id;
        accountInfo[i].user_id = accounts[i].user_id;
    }
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

        if (isValidUser(input))
        {
            FILE *htmlFile;
            htmlFile = fopen("HTML_output/Page2-userBarGraph.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create Page2-userBarGraph file\n");
                exit(EXIT_FAILURE);
            }

            calculateExpenseTotal(expenses, num_expenses);
            printAllExpenseTotals(input, expenses, num_expenses, accounts, num_accounts, htmlFile);

            openHTMLPage("HTML_output/Page2-userBarGraph.html");

            printf("Enter the account ID: ");
            OutputFSM->currentState = CHECK_ACCOUNTID;
            OutputFSM->userID = input;
        }
        else
        {
            printf("The user ID %d does not exist!\n", input);
            printf("Please enter the user ID again: ");
        }
        break;
    }

    case CHECK_ACCOUNTID:
    {

        if (isValidAccount(input, OutputFSM->userID))
        {
            FILE *htmlFile;
            htmlFile = fopen("HTML_output/Page3-AccOverallExpenses.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create Page3-AccOverallExpenses file\n");
                exit(EXIT_FAILURE);
            }

            OutputFSM->accountID = input;
            categorizeExpenses(expenses, num_expenses);
            generateHTMLForAccount(expenses, num_expenses, input, htmlFile);

            fclose(htmlFile);

            openHTMLPage("HTML_output/Page3-AccOverallExpenses.html");
            OutputFSM->currentState = CHECK_YEAR;
            printf("Enter the year you want to review (eg.2024): ");
        }
        else
        {
            printf("The account ID %d does not exist!\n", input);
            printf("Please enter the account ID again: ");
        }
        break;
    }

    case CHECK_YEAR:
    {
        int numYears;
        int *years;

        years = getYearsFromExpenses(expenses, num_expenses, OutputFSM->accountID, &numYears);

        if ((input > 999 && input < 2000) || (input > 2024 && input <= 9999))
        {
            printf("Year is not within the valid range of 2000-2024. \n");
            printf("Please enter the year again (eg.2024): ");
        }
        else if (input > 9999)
        {
            printf("Year should not exceed four digits. \n");
            printf("Please enter the year again (eg.2024): ");
        }
        else if (input <= 999)
        {
            printf("Year should not be less than four digits. \n");
            printf("Please enter the year again (eg.2024): ");
        }

        else if (isYearInList(years, numYears, input))
        {
            FILE *htmlFile;
            htmlFile = fopen("HTML_output/Page4-AccYearlyExpenses.html", "w");
            if (htmlFile == NULL)
            {
                printf("Error: Unable to create HTML file\n");
                exit(EXIT_FAILURE);
            }

            categorizeExpensesByYear(expenses, num_expenses, input);
            generateHTMLForAccountYear(expenses, num_expenses, OutputFSM->accountID, htmlFile, input);

            fclose(htmlFile);

            openHTMLPage("HTML_output/Page4-AccYearlyExpenses.html");
            printf("Please enter 'exit' to terminate the Finance Overview Session: ");
        }
        else
        {
            printf("There are no existing expenses for the year %d!\n", input);
            printf("Please enter the year again (eg.2024): ");
        }
        break;
    }
    }
}

int main(int argc, char const *argv[])
{
    char inputStr[100];
    OutputFSM fsm;
    initOutputFSM(&fsm);

    openHTMLPage("HTML_output/Page1-MainPage.html");
    printf("Please enter 'exit' to terminate the Finance Overview Session at any time. \n");
    printf("Enter the user ID: ");

    while (1)
    {
        if (fgets(inputStr, sizeof(inputStr), stdin))
        {
            int input;
            char *endPtr;
            if (strncmp(inputStr, "exit", 4) == 0)
            {
                break;
            }

            input = (int)strtol(inputStr, &endPtr, 10);
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
