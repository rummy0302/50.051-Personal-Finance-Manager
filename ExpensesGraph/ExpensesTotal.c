#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExpensesTotal.h"
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"

CurrencyTotals currencytotals[MAX_ACCOUNTS];

void calculateCurrencyTotal(Expenses *expenses, int numExpenses) {
    int i;

    for (i = 0; i < numExpenses; i++) {
        int accountID = expenses[i].account_id;
        char *currency = expenses[i].currency;
        double amount = expenses[i].amount_spent;
        
        if (strcmp(currency, "SGD") == 0){
            currencytotals[accountID].totalSGD += amount;
        } else if (strcmp(currency, "USD") == 0){
            currencytotals[accountID].totalUSD += amount;
        } else if (strcmp(currency, "EUR") == 0){
            currencytotals[accountID].totalEUR += amount;
        } else {
            printf("Error : This currency is not supported by this system\n");
        }

    }
}

void printCurrencyTotal(int accountId, double totalSGD, double totalUSD, double totalEUR) {
    printf("Account ID: %d\n\n", accountId);
    printf("Currency                Total Expense\n");
    printf("%-24s%.2lf\n", "SGD", totalSGD);
    printf("%-24s%.2lf\n", "USD", totalUSD);
    printf("%-24s%.2lf\n", "EUR", totalEUR);
    printf("\n");
}

void printAllCurrencyTotals(Expenses *expenses, int numExpenses, Account *accounts, int numAccounts) {

    double totalSGD, totalUSD, totalEUR;

    int i, j;
    int uniqueAccounts[MAX_ACCOUNTS] = {0}; 
    int numUniqueAccounts = 0;

    int uniqueUsers[MAX_USERS] = {0};
    int numUniqueUsers = 0;

    int *accountsPerUser; /* Create a dynamic array to store the number of accounts held by each user (we use malloc because the number of unique users is only known later in the code)*/

    int **user_accounts; /* Dynamic array to store the user and their corresponding accountids of the accounts they hold */
    
    int accountIndex = 0;


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

    /* For each user, print the different accounts held and their total expense in different currencies (SGD, USD, EUR) : */

    for (i = 0; i < numUniqueUsers; i++){
        printf("User ID: %d\n\n", uniqueUsers[i]);

        for (j = 0; j < accountsPerUser[i]; j++){
            int accountId = user_accounts[i][j];
            totalSGD = currencytotals[accountId].totalSGD;
            totalUSD = currencytotals[accountId].totalUSD;
            totalEUR = currencytotals[accountId].totalEUR;


            if (totalSGD + totalUSD + totalEUR  > 0) {
                printCurrencyTotal(accountId, totalSGD, totalUSD, totalEUR);
            }
        }
        printf("--------------------------------\n");

    }



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

    calculateCurrencyTotal(expenses, numExpenses);

    printAllCurrencyTotals(expenses, numExpenses, accounts, numAccounts); 

    free(expenses);
    free(accounts);

    return 0;
}
