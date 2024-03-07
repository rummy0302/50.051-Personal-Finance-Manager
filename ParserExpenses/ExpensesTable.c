#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExpensesTable.h"
#include "ParserExpenses.h"

Shop shops[] = {
    {"Pizza Hut", "Food"},
    {"Jollibee", "Food"},
    {"Ice Cream Parlor", "Food"},
    {"Grocery Store", "Food"},
    {"Cafeteria", "Food"},
    {"Italian Restaurant", "Food"},
    {"Coffee Shop", "Food"},
    {"Barbershop", "Food"},
    {"Family Restaurant", "Food"},
    {"Fashion Store", "Shopping"},
    {"Starbucks", "Shopping"},
    {"Gas Station", "Shopping"},
    {"Retail Store", "Shopping"},
    {"Music Store", "Shopping"},
    {"Pharmacy", "Shopping"},
    {"Car Wash", "Shopping"},
    {"Flower Shop", "Shopping"},
    {"Home Decor Store", "Shopping"},
    {"Toy Store", "Shopping"},
    {"MRT", "Transport"},
    {"Taxi Service", "Transport"},
    {"Park Entrance Fee", "Others"},
    {"Movie Theater", "Others"},
    {"Online Subscription", "Others"},
    {"Auto Repair Shop", "Others"},
    {"Clinic", "Others"},
};

AccountExpenses accountTotals[MAX_ACCOUNTS];

void categorizeExpenses(Expenses *expenses, int numExpenses) {
    int i, j;
    for (i = 0; i < numExpenses; i++) {
        double amount = expenses[i].amount_spent;
        int found = 0;

        for (j = 0; j < sizeof(shops) / sizeof(shops[0]); j++) {
            if (strcmp(expenses[i].description, shops[j].shopName) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            accountTotals[expenses[i].account_id].totalOthers += amount;
        } else {
            if (strcmp(shops[j].category, "Food") == 0) {
                accountTotals[expenses[i].account_id].totalFood += amount;
            } else if (strcmp(shops[j].category, "Transport") == 0) {
                accountTotals[expenses[i].account_id].totalTransport += amount;
            } else if (strcmp(shops[j].category, "Shopping") == 0) {
                accountTotals[expenses[i].account_id].totalShopping += amount;
            } else {
                accountTotals[expenses[i].account_id].totalOthers += amount;
            }
        }
    }
}

void printExpenses(int accountId, double totalFood, double totalTransport, double totalShopping, double totalOthers) {
    printf("Account ID: %d\n\n", accountId);
    printf("Category                Amount of Expense\n");
    printf("%-24s%.2lf\n", "Food", totalFood);
    printf("%-24s%.2lf\n", "Transport", totalTransport);
    printf("%-24s%.2lf\n", "Shopping", totalShopping);
    printf("%-24s%.2lf\n", "Others", totalOthers);
    printf("\n");
}

void printAllExpenses(Expenses *expenses, int numExpenses) {
    double totalFood, totalTransport, totalShopping, totalOthers;
    int i, j;
    int uniqueAccounts[MAX_ACCOUNTS] = {0}; 
    int numUniqueAccounts = 0;


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


    for (i = 0; i < numUniqueAccounts; i++) {
        int accountId = uniqueAccounts[i];
        totalFood = accountTotals[accountId].totalFood;
        totalTransport = accountTotals[accountId].totalTransport;
        totalShopping = accountTotals[accountId].totalShopping;
        totalOthers = accountTotals[accountId].totalOthers;

        if (totalFood + totalTransport + totalShopping + totalOthers > 0) {
            printExpenses(accountId, totalFood, totalTransport, totalShopping, totalOthers);
        }
    }
}


int main() {
    int numExpenses;
    Expenses *expenses;

    cJSON *json = parseExpensesJSONfile("Expenses.json"); 
    if (json == NULL) {
        printf("Error: Failed to parse Expenses.json\n");
        return 1;
    }

    expenses = processExpensesData(json, &numExpenses); 
    cJSON_Delete(json);

    categorizeExpenses(expenses, numExpenses);

    printAllExpenses(expenses, numExpenses); 

    free(expenses);

    return 0;
}
