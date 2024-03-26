#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Page2-UserBarGraph.h"
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"

/* This file is to generate the user expense graphs (total expense in SGD) for each account - 
    1. Calculates total expense in SGD (after currency conversion for expense in USD and EUR) for each account
    2. Displays a bar graph showing the total expense in SGD for each account held by the user
*/


ExpenseTotalsSGD expensetotalsSGD[MAX_ACCOUNTS];

/* Uses the exchange rate at the time of writing this code (14.03.2024, 10:49am UTC) : */
double USD_TO_SGD_RATE = 1.33;

double EUR_TO_SGD_RATE = 1.46;

void calculateExpenseTotal(Expenses *expenses, int numExpenses) {
    int i;


    for (i = 0; i < numExpenses; i++) {
        int accountID = expenses[i].account_id;
        char *currency = expenses[i].currency;
        double amount = expenses[i].amount_spent;
        
        if (strcmp(currency, "SGD") == 0){
            expensetotalsSGD[accountID].totalExpenseInSGD += amount;
 
        } else if (strcmp(currency, "USD") == 0){
            expensetotalsSGD[accountID].totalExpenseInSGD += amount * USD_TO_SGD_RATE;

        } else if (strcmp(currency, "EUR") == 0){
            expensetotalsSGD[accountID].totalExpenseInSGD += amount * EUR_TO_SGD_RATE;

        } else {
            printf("Error : This currency is not supported by this system\n");
        }

    }
}

void printExpenseTotal(int accountId, double totalExpenseInSGD) {
    printf("Account ID: %d\n\n", accountId);
    printf("%-24s%.2lf\n", "Total Expense in SGD: ",totalExpenseInSGD);
    printf("\n\n");
}

void printAllExpenseTotals(int userIdInput, Expenses *expenses, int numExpenses, Account *accounts, int numAccounts) {

    double totalExpenseInSGD;

    int i, j;
    int uniqueAccounts[MAX_ACCOUNTS] = {0}; 
    int numUniqueAccounts = 0;

    int uniqueUsers[MAX_USERS] = {0};
    int numUniqueUsers = 0;

    int *accountsPerUser; /* Create a dynamic array to store the number of accounts held by each user (we use malloc because the number of unique users is only known later in the code)*/

    int **user_accounts; /* Dynamic array to store the user and their corresponding accountids of the accounts they hold */
    
    int accountIndex = 0;

    FILE *htmlFile;

    int userIndex = -1;


    /* Calculate number of unique user_ids from the Accounts.json file : */

    for (i = 0; i < numAccounts; i++) {
        int isNewUser = 1; 
        int userId = accounts[i].user_id;
        for (j = 0; j < numUniqueUsers; j++) {
            if (uniqueUsers[j] == userId) {
                isNewUser = 0;
                break;
            }
        }
        if (isNewUser) {
            uniqueUsers[numUniqueUsers] = userId;
            numUniqueUsers++;
        }
    }

    /* Calculate number of unique account_ids from the Expenses.json file : */

    for (i = 0; i < numExpenses; i++) {
        int isNewAccount = 1; 
        int accountId = expenses[i].account_id;
        for (j = 0; j < numUniqueAccounts; j++) {
            if (uniqueAccounts[j] == accountId) {
                isNewAccount = 0;
                break;
            }
        }
        if (isNewAccount) {
            uniqueAccounts[numUniqueAccounts] = accountId;
            numUniqueAccounts++;
        }
    }
    
    /* Calculate number of accounts held by each user : */

    accountsPerUser = (int *)malloc(numUniqueUsers * sizeof(int));
    if (accountsPerUser == NULL){
        printf("Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < numAccounts; ++i) {
        int user_id = accounts[i].user_id;
        accountsPerUser[user_id - 1]++;
    }


    /* Populate the 2D array with userid, accountid respectively */
    user_accounts = (int **)malloc(numUniqueUsers * sizeof(int *)); 

    for (i = 0; i < numUniqueUsers; i++){
        user_accounts[i] = (int *)malloc(accountsPerUser[i] * sizeof(int));
        if (user_accounts[i] == NULL){
            printf("Error: Unable to allocate memory\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < numUniqueUsers; ++i) {
        int userId = uniqueUsers[i];

        accountIndex = 0;

        
        for (j = 0; j < numAccounts; j++) {
            if (accounts[j].user_id == userId) {
                user_accounts[i][accountIndex++] = accounts[j].account_id;
            }
        }
    }

    /* For each user, print the different accounts held and their total expense in the deafult currency SGD (after currency conversion for USD and EUR) : */

    for (i = 0; i < numUniqueUsers; i++){
        printf("User ID: %d\n\n", uniqueUsers[i]);

        for (j = 0; j < accountsPerUser[i]; j++){
            int accountId = user_accounts[i][j];
            totalExpenseInSGD = expensetotalsSGD[accountId].totalExpenseInSGD;


            if ((totalExpenseInSGD  > 0)) {
                printExpenseTotal(accountId, totalExpenseInSGD);
            }
        }
        printf("--------------------------------\n");

    }

    /* Code to write the HTML page and render the corresponding expense graphs : */

    /* Open the html page in write mode : */
    htmlFile = fopen("page2_userBarGraph.html", "w");
    if (htmlFile == NULL) {
        printf("Error: Unable to create HTML file\n");
        exit(EXIT_FAILURE);
    }

    /* Write HTML header */

    fprintf(htmlFile, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(htmlFile, "<title>User Expense Graphs</title>\n");

    /* Call plotly library from js to render the graphs : */
    fprintf(htmlFile, "<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n");
    fprintf(htmlFile, "</head>\n<body>\n");


    /* Find the index of the user with the specified userIdInput */
    for (i = 0; i < numUniqueUsers; i++) {
        if (uniqueUsers[i] == userIdInput) {
            userIndex = i;
            break;
        }
    }
    
     /* Generate overall user graphs with different accounts and their total expenses in SGD */
    if (userIndex != -1) { /* UserId exists */
        fprintf(htmlFile, "<h2>User ID: %d</h2>\n", userIdInput);
        fprintf(htmlFile, "<div id=\"user%d_graph\"></div>\n", userIdInput);
        fprintf(htmlFile, "<script>\n");
        fprintf(htmlFile, "var data_user_%d = [\n", userIdInput);
        
        for (j = 0; j < accountsPerUser[userIndex]; j++) {
            int accountId = user_accounts[userIndex][j];
            totalExpenseInSGD = expensetotalsSGD[accountId].totalExpenseInSGD;

            fprintf(htmlFile, "{\n");
            fprintf(htmlFile, "x: ['Account %d'],\n", accountId);
            fprintf(htmlFile, "y: [%.2lf],\n", totalExpenseInSGD);
            fprintf(htmlFile, "type: 'bar',\n"); /* Generate bar graph */
            fprintf(htmlFile, "name: 'Account %d'\n", accountId);
            fprintf(htmlFile, "},\n");
        }

        fprintf(htmlFile, "];\n");
        fprintf(htmlFile, "var layout_user_%d = {\n", userIdInput);
        fprintf(htmlFile, "title: 'Total Expense Graph in SGD'");
        fprintf(htmlFile, "};\n");
        fprintf(htmlFile, "Plotly.newPlot('user%d_graph', data_user_%d, layout_user_%d);\n", userIdInput, userIdInput, userIdInput);
        fprintf(htmlFile, "</script>\n");
        fprintf(htmlFile, "<hr />\n");
    }

    /* Write HTML footer */
    fprintf(htmlFile, "</body>\n</html>");

    /* Close HTML file */
    fclose(htmlFile);



    for (i = 0; i < numUniqueUsers; i++){
        free(user_accounts[i]);
    }
    free(user_accounts);

    free(accountsPerUser);



}





int main(int argc, char **argv) {
    int numExpenses;
    int numAccounts;
    Expenses *expenses;
    Account *accounts;

    cJSON *json_expenses;
    cJSON *json_accounts;

    int userIdInput;

    json_expenses = parseExpensesJSONfile("../ParserExpenses/Expenses.json"); 
    if (json_expenses == NULL) {
        printf("Error: Failed to parse Expenses.json\n");
        return 1;
    }

    json_accounts = parseAccountsJSONfile("../ParserAccounts/Accounts.json"); 
    if (json_accounts == NULL) {
        printf("Error: Failed to parse Accounts.json\n");
        return 1;
    }

    expenses = processExpensesData(json_expenses, &numExpenses); 
    cJSON_Delete(json_expenses);


    accounts = processAccountsData(json_accounts, &numAccounts); 
    cJSON_Delete(json_accounts);

    calculateExpenseTotal(expenses, numExpenses);

    /* Hardcoding the userId now : (can retrieve from input and then pass it as argument to printAllExpenseTotals function)*/

    userIdInput = 3;

    printAllExpenseTotals(userIdInput, expenses, numExpenses, accounts, numAccounts); 

    free(expenses);
    free(accounts);

    return 0;
}
