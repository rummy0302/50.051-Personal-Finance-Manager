#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "CurrencyExpensesTable.h"
#include "ParserExpenses.h"

typedef enum {
    SGD,
    USD,
    EUR,
    NUM_CURRENCIES
} CurrencyType;

const char* currencyStrings[NUM_CURRENCIES] = {
    "SGD",
    "USD",
    "EUR"
};

CurrencyType getCurrencyType(const char* currencyStr) {
    int i;
    for (i = 0; i < NUM_CURRENCIES; i++) {
        if (strcmp(currencyStr, currencyStrings[i]) == 0) {
            return (CurrencyType)i;
        }
    }
    return -1;
}

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


    memset(accountTotals, 0, sizeof(accountTotals));

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
      
            CurrencyType currency = getCurrencyType(expenses[i].currency);

            switch (currency) {
                case SGD:
                    accountTotals[expenses[i].account_id].totalOthersSGD += amount;
                    break;
                case USD:
                    accountTotals[expenses[i].account_id].totalOthersUSD += amount;
                    break;
                case EUR:
                    accountTotals[expenses[i].account_id].totalOthersEUR += amount;
                    break;
                default:
                    accountTotals[expenses[i].account_id].totalOthersSGD += amount;
                    break;
    }
        } else {
 
            CurrencyType currency = getCurrencyType(expenses[i].currency);

            switch (currency) {
                case SGD:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotals[expenses[i].account_id].totalFoodSGD += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotals[expenses[i].account_id].totalTransportSGD += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotals[expenses[i].account_id].totalShoppingSGD += amount;
                    } else {
                        accountTotals[expenses[i].account_id].totalOthersSGD += amount;
                    }
                    break;
                case USD:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotals[expenses[i].account_id].totalFoodUSD += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotals[expenses[i].account_id].totalTransportUSD += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotals[expenses[i].account_id].totalShoppingUSD += amount;
                    } else {
                        accountTotals[expenses[i].account_id].totalOthersUSD += amount;
                    }
                    break;
                case EUR:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotals[expenses[i].account_id].totalFoodEUR += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotals[expenses[i].account_id].totalTransportEUR += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotals[expenses[i].account_id].totalShoppingEUR += amount;
                    } else {
                        accountTotals[expenses[i].account_id].totalOthersEUR += amount;
                    }
                    break;
                default:
                    accountTotals[expenses[i].account_id].totalOthersSGD += amount;
                    break;
            }
        }
    }
}

void printExpenses(int accountId, double totalFoodSGD, double totalFoodUSD, double totalFoodEUR,
                   double totalTransportSGD, double totalTransportUSD, double totalTransportEUR,
                   double totalShoppingSGD, double totalShoppingUSD, double totalShoppingEUR,
                   double totalOthersSGD, double totalOthersUSD, double totalOthersEUR) {
    printf("Account ID: %d\n\n", accountId);
    printf("Category                Amount of Expense(SGD)   Amount of Expense(USD)   Amount of Expense(EUR)\n");
    printf("%-24s%12.2lf%25.2lf%25.2lf\n", "Food", totalFoodSGD, totalFoodUSD, totalFoodEUR);
    printf("%-24s%12.2lf%25.2lf%25.2lf\n", "Transport", totalTransportSGD, totalTransportUSD, totalTransportEUR);
    printf("%-24s%12.2lf%25.2lf%25.2lf\n", "Shopping", totalShoppingSGD, totalShoppingUSD, totalShoppingEUR);
    printf("%-24s%12.2lf%25.2lf%25.2lf\n", "Others", totalOthersSGD, totalOthersUSD, totalOthersEUR);
    printf("\n");
}

void printAllExpenses(Expenses *expenses, int numExpenses) {
    double totalFoodSGD, totalFoodUSD, totalFoodEUR;
    double totalTransportSGD, totalTransportUSD, totalTransportEUR;
    double totalShoppingSGD, totalShoppingUSD, totalShoppingEUR;
    double totalOthersSGD, totalOthersUSD, totalOthersEUR;
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
        totalFoodSGD = accountTotals[accountId].totalFoodSGD;
        totalFoodUSD = accountTotals[accountId].totalFoodUSD;
        totalFoodEUR = accountTotals[accountId].totalFoodEUR;
        totalTransportSGD = accountTotals[accountId].totalTransportSGD;
        totalTransportUSD = accountTotals[accountId].totalTransportUSD;
        totalTransportEUR = accountTotals[accountId].totalTransportEUR;
        totalShoppingSGD = accountTotals[accountId].totalShoppingSGD;
        totalShoppingUSD = accountTotals[accountId].totalShoppingUSD;
        totalShoppingEUR = accountTotals[accountId].totalShoppingEUR;
        totalOthersSGD = accountTotals[accountId].totalOthersSGD;
        totalOthersUSD = accountTotals[accountId].totalOthersUSD;
        totalOthersEUR = accountTotals[accountId].totalOthersEUR;

        if (totalFoodSGD + totalFoodUSD + totalFoodEUR +
            totalTransportSGD + totalTransportUSD + totalTransportEUR +
            totalShoppingSGD + totalShoppingUSD + totalShoppingEUR +
            totalOthersSGD + totalOthersUSD + totalOthersEUR > 0) {
            printExpenses(accountId, totalFoodSGD, totalFoodUSD, totalFoodEUR,
                          totalTransportSGD, totalTransportUSD, totalTransportEUR,
                          totalShoppingSGD, totalShoppingUSD, totalShoppingEUR,
                          totalOthersSGD, totalOthersUSD, totalOthersEUR);
        }
    }
}

int main(int argc, char **argv) {
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




