#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Page2-UserBarGraph.h"
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"
#include "Common.h"

/* This file is to generate the user expense graphs (total expense in SGD) for each account -
    1. Calculates total expense in SGD (after currency conversion for expense in USD and EUR) for each account
    2. Displays a bar graph showing the total expense in SGD for each account held by the user
*/

ExpenseTotalsSGD expensetotalsSGD[MAX_ACCOUNTS];

/* Uses the exchange rate at the time of writing this code (14.03.2024, 10:49am UTC) : */
double USD_TO_SGD_RATE = 1.33;

double EUR_TO_SGD_RATE = 1.46;

void calculateExpenseTotal(Expenses *expenses, int numExpenses)
{
    int i;

    for (i = 0; i < numExpenses; i++)
    {
        int accountID = expenses[i].account_id;
        char *currency = expenses[i].currency;
        double amount = expenses[i].amount_spent;

        if (strcmp(currency, "SGD") == 0)
        {
            expensetotalsSGD[accountID].totalExpenseInSGD += amount;
        }
        else if (strcmp(currency, "USD") == 0)
        {
            expensetotalsSGD[accountID].totalExpenseInSGD += amount * USD_TO_SGD_RATE;
        }
        else if (strcmp(currency, "EUR") == 0)
        {
            expensetotalsSGD[accountID].totalExpenseInSGD += amount * EUR_TO_SGD_RATE;
        }
        else
        {
            printf("Error : This currency is not supported by this system\n");
        }
    }
}

void printExpenseTotal(int accountId, double totalExpenseInSGD)
{
    printf("Account ID: %d\n\n", accountId);
    printf("%-24s%.2f\n", "Total Expense in SGD: ", totalExpenseInSGD);
    printf("\n\n");
}

void printAllExpenseTotals(int userIdInput, Expenses *expenses, int numExpenses, Account *accounts, int numAccounts, FILE *htmlFile)
{

    double totalExpenseInSGD;

    int i, j;
    int uniqueAccounts[MAX_ACCOUNTS] = {0};
    int numUniqueAccounts = 0;

    int uniqueUsers[MAX_USERS] = {0};
    int numUniqueUsers = 0;

    int *accountsPerUser; /* Create a dynamic array to store the number of accounts held by each user (we use malloc because the number of unique users is only known later in the code)*/

    int **user_accounts; /* Dynamic array to store the user and their corresponding accountids of the accounts they hold */

    int accountIndex = 0;

    int userIndex = -1;

    /* Calculate number of unique user_ids from the Accounts.json file : */

    for (i = 0; i < numAccounts; i++)
    {
        int isNewUser = 1;
        int userId = accounts[i].user_id;
        for (j = 0; j < numUniqueUsers; j++)
        {
            if (uniqueUsers[j] == userId)
            {
                isNewUser = 0;
                break;
            }
        }
        if (isNewUser)
        {
            uniqueUsers[numUniqueUsers] = userId;
            numUniqueUsers++;
        }
    }

    /* Calculate number of unique account_ids from the Expenses.json file : */

    for (i = 0; i < numExpenses; i++)
    {
        int isNewAccount = 1;
        int accountId = expenses[i].account_id;
        for (j = 0; j < numUniqueAccounts; j++)
        {
            if (uniqueAccounts[j] == accountId)
            {
                isNewAccount = 0;
                break;
            }
        }
        if (isNewAccount)
        {
            uniqueAccounts[numUniqueAccounts] = accountId;
            numUniqueAccounts++;
        }
    }

    /* Calculate number of accounts held by each user : */

    accountsPerUser = (int *)malloc(numUniqueUsers * sizeof(int));
    if (accountsPerUser == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    memset(accountsPerUser, 0, numUniqueUsers * sizeof(int));

    for (i = 0; i < numAccounts; ++i)
    {
        int user_id = accounts[i].user_id;
        accountsPerUser[user_id - 1]++;
    }

    /* Populate the 2D array with userid, accountid respectively */
    user_accounts = (int **)malloc(numUniqueUsers * sizeof(int *));

    for (i = 0; i < numUniqueUsers; i++)
    {
        user_accounts[i] = (int *)malloc(accountsPerUser[i] * sizeof(int));
        if (user_accounts[i] == NULL)
        {
            printf("Error: Unable to allocate memory\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < numUniqueUsers; ++i)
    {
        int userId = uniqueUsers[i];

        accountIndex = 0;

        for (j = 0; j < numAccounts; j++)
        {
            if (accounts[j].user_id == userId)
            {
                user_accounts[i][accountIndex++] = accounts[j].account_id;
            }
        }
    }

    /* Code to write the HTML page and render the corresponding expense graphs : */

    /* Write HTML header */

    fprintf(htmlFile, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(htmlFile, "<title>User Expense Graphs</title>\n");

    /* Call plotly library from js to render the graphs : */
    fprintf(htmlFile, "<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n");

    /* CSS of head bar */
    fprintf(htmlFile, "<style>\n");
    fprintf(htmlFile, "body {\n");
    fprintf(htmlFile, "    font-family: Arial, sans-serif;\n");
    fprintf(htmlFile, "    margin: 0;\n");
    fprintf(htmlFile, "    padding: 0;\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "\n");
    fprintf(htmlFile, ".headbar {\n");
    fprintf(htmlFile, "    padding: 10px 20px;\n");
    fprintf(htmlFile, "    display: flex;\n");
    fprintf(htmlFile, "    justify-content: space-between;\n");
    fprintf(htmlFile, "    align-items: center;\n");
    fprintf(htmlFile, "    box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "\n");
    fprintf(htmlFile, ".appName {\n");
    fprintf(htmlFile, "    font-size: 20px;\n");
    fprintf(htmlFile, "    font-weight: bold;\n");
    fprintf(htmlFile, "    color:#004F99;\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "\n");

    fprintf(htmlFile, ".main-header {\n");
    fprintf(htmlFile, "    font-size: 28px; \n");
    fprintf(htmlFile, "    font-weight: bold;\n");
    fprintf(htmlFile, "    padding: 10px 20px;\n");
    fprintf(htmlFile, "    margin-top: 20px;\n");
    fprintf(htmlFile, "    font-style: italic;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, ".account-overview {\n");
    fprintf(htmlFile, "    font-size: 16px;\n");
    fprintf(htmlFile, "    color: rgba(0,0,0,0.6);\n");
    fprintf(htmlFile, "    padding-left: 20px;\n");
    fprintf(htmlFile, "    margin-bottom: 40px;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, ".user-id {\n");
    fprintf(htmlFile, "    font-weight: bold;\n");
    fprintf(htmlFile, "    color: #004F99;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "</style>\n");

    fprintf(htmlFile, "</head>\n<body>\n");

    fprintf(htmlFile, "<div class=\"headbar\">\n");
    fprintf(htmlFile, "  <div class=\"appName\">WealthWise</div>\n");
    fprintf(htmlFile, "  </div>\n");
    fprintf(htmlFile, "</div>\n");

    fprintf(htmlFile, "<h1 class=\"main-header\">Welcome to your financial overview!</h1>\n");
    fprintf(htmlFile, "<p class=\"account-overview\">Here's an overview of expenses in the Accounts belonging to <span class=\"user-id\">User ID %d</span>.</p>\n", userIdInput);

    /* Find the index of the user with the specified userIdInput */
    for (i = 0; i < numUniqueUsers; i++)
    {
        if (uniqueUsers[i] == userIdInput)
        {
            userIndex = i;
            break;
        }
    }

    /* Generate overall user graphs with different accounts and their total expenses in SGD */
    if (userIndex != -1)
    { /* UserId exists */
        fprintf(htmlFile, "<div id=\"user%d_graph\"></div>\n", userIdInput);
        fprintf(htmlFile, "<script>\n");
        fprintf(htmlFile, "var colors = ['#76D7C4', '#F7DC6F', '#85C1E9', '#2370F7','#D2B4DE', '#F0B27A',  '#F5B7B1'];\n");
        fprintf(htmlFile, "var data_user_%d = [\n", userIdInput);

        for (j = 0; j < accountsPerUser[userIndex]; j++)
        {
            int accountId = user_accounts[userIndex][j];
            totalExpenseInSGD = expensetotalsSGD[accountId].totalExpenseInSGD;

            fprintf(htmlFile, "{\n");
            fprintf(htmlFile, "x: ['Account %d'],\n", accountId);
            fprintf(htmlFile, "y: [%.2f],\n", totalExpenseInSGD);
            fprintf(htmlFile, "type: 'bar',\n"); /* Generate bar graph */
            fprintf(htmlFile, "name: 'Account %d',\n", accountId);
            fprintf(htmlFile, "marker: {color: colors[%d %% colors.length]},\n", j);
            fprintf(htmlFile, "},\n");
        }

        fprintf(htmlFile, "];\n");
        fprintf(htmlFile, "var layout_user_%d = {\n", userIdInput);
        fprintf(htmlFile, "    title: {\n");
        fprintf(htmlFile, "        text: 'Total Expense Graph in SGD',\n");
        fprintf(htmlFile, "        font: {\n");
        fprintf(htmlFile, "            size: 14,\n");
        fprintf(htmlFile, "        }\n");
        fprintf(htmlFile, "    }\n");
        fprintf(htmlFile, "};\n");
        fprintf(htmlFile, "Plotly.newPlot('user%d_graph', data_user_%d, layout_user_%d);\n", userIdInput, userIdInput, userIdInput);
        fprintf(htmlFile, "</script>\n");
    }

    /* Write HTML footer */
    fprintf(htmlFile, "</body>\n</html>");

    /* Close HTML file */
    fclose(htmlFile);

    for (i = 0; i < numUniqueUsers; i++)
    {
        free(user_accounts[i]);
    }
    free(user_accounts);

    free(accountsPerUser);
}

/*
int main(int argc, char **argv)
{
    int num_expenses = 0;
    int num_accounts = 0;
    FILE *htmlFile;
    int userIdInput;
    Account accounts[MAX_ACCOUNTS];
    Expenses expenses[MAX_EXPENSES];

    const char *accountsfilename = "../ParserAccounts/Accounts.json";
    const char *expensesfilename = "../ParserExpenses/Expenses.json";
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

    calculateExpenseTotal(expenses, num_expenses);

    userIdInput = 1;

    htmlFile = fopen("Page2-userBarGraph.html", "w");
    if (htmlFile == NULL)
    {
        printf("Error: Unable to create HTML file\n");
        exit(EXIT_FAILURE);
    }

    printAllExpenseTotals(userIdInput, expenses, num_expenses, accounts, num_accounts, htmlFile);

    return 0;
} */
