#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Page4-AccYearlyExpenses.h"
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"


#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <unistd.h>
#endif

typedef struct {
    int account_id;
    int user_id;
} AccountInfo;

AccountInfo accountInfo[MAX_ACCOUNTS];
int numAccounts = 0;


int isValidAccount(int accountId, int userId) {
    int i;
    for (i = 0; i < numAccounts; i++) {
        if (accountInfo[i].account_id == accountId && accountInfo[i].user_id == userId) {
            return 1; 
        }
    }
    return 0; 
}


typedef enum {
    SGD,
    USD,
    EUR,
    NUM_CURRENCIES,
    UNKNOWN_CURRENCY
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
    return UNKNOWN_CURRENCY;
}

Shop shops[] = {
    {"Pizza Hut", "Food"},
    {"Jollibee", "Food"},
    {"Bakery", "Food"},
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
    {"Travel Agency", "Others"},
    {"Electronics Store", "Others"},
    {"Utility Company", "Others"},
    {"Gift Shop", "Others"},
    {"Gym", "Others"},
    {"Bookstore", "Others"},
    {"Convenience Store", "Others"},
    {"Pet Store", "Others"},
    {"Amazon", "Others"},
    {"Netflix", "Others"},
    {"Uber Eats", "Food"},
    {"McDonald's", "Food"},
    {"Apple Store", "Others"},
    {"Walmart", "Others"},
    {"Movie Tickets", "Others"},
    {"Clothing Boutique", "Shopping"},
    {"Hardware Store", "Others"},
    {"Stationery Shop", "Others"},
    {"Sports Equipment Store", "Shopping"},
    {"Furniture Store", "Shopping"},
    {"Appliance Store", "Shopping"},
    {"Shoe Store", "Shopping"},
    {"Jewelry Store", "Shopping"},
    {"Cosmetics Store", "Shopping"},
    {"Bus Fare", "Transport"},
    {"Taxi Ride", "Transport"},
    {"Clothing Store", "Shopping"}
};


AccountExpensesYear accountTotalsYear[MAX_ACCOUNTS];

void categorizeExpensesByYear(Expenses *expenses, int numExpenses, int year) {
    int i, j;


    memset(accountTotalsYear, 0, sizeof(accountTotalsYear));

    for (i = 0; i < numExpenses; i++) {
        double amount = expenses[i].amount_spent;
        int found = 0;

        int expenseYear = atoi(strtok(expenses[i].date, "-"));
        if (expenseYear != year) {
            continue; 
        }

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
                    accountTotalsYear[expenses[i].account_id].totalOthersSGDYear += amount;
                    break;
                case USD:
                    accountTotalsYear[expenses[i].account_id].totalOthersUSDYear += amount;
                    break;
                case EUR:
                    accountTotalsYear[expenses[i].account_id].totalOthersEURYear += amount;
                    break;
                default:
                    accountTotalsYear[expenses[i].account_id].totalOthersSGDYear += amount;
                    break;
            }
        } else {

            CurrencyType currency = getCurrencyType(expenses[i].currency);
            switch (currency) {
                case SGD:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalFoodSGDYear += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalTransportSGDYear += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalShoppingSGDYear += amount;
                    } else {
                        accountTotalsYear[expenses[i].account_id].totalOthersSGDYear += amount;
                    }
                    break;
                case USD:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalFoodUSDYear += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalTransportUSDYear += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalShoppingUSDYear += amount;
                    } else {
                        accountTotalsYear[expenses[i].account_id].totalOthersUSDYear += amount;
                    }
                    break;
                case EUR:
                    if (strcmp(shops[j].category, "Food") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalFoodEURYear += amount;
                    } else if (strcmp(shops[j].category, "Transport") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalTransportEURYear += amount;
                    } else if (strcmp(shops[j].category, "Shopping") == 0) {
                        accountTotalsYear[expenses[i].account_id].totalShoppingEURYear += amount;
                    } else {
                        accountTotalsYear[expenses[i].account_id].totalOthersEURYear += amount;
                    }
                    break;
                default:
                    accountTotalsYear[expenses[i].account_id].totalOthersSGDYear += amount;
                    break;
            }
        }
    }
}


void generateHTMLTableForAccountYear(int accountId, FILE *htmlFile, int year) {
    double totalFoodSGDYear = 0, totalFoodUSDYear = 0, totalFoodEURYear = 0;
    double totalTransportSGDYear = 0, totalTransportUSDYear = 0, totalTransportEURYear = 0;
    double totalShoppingSGDYear = 0, totalShoppingUSDYear = 0, totalShoppingEURYear = 0;
    double totalOthersSGDYear = 0, totalOthersUSDYear = 0, totalOthersEURYear = 0;

    fprintf(htmlFile, "<!DOCTYPE html>\n");
    fprintf(htmlFile, "<html>\n");
    fprintf(htmlFile, "<head>\n");
    fprintf(htmlFile, "<title>Expenses Table</title>\n");
    fprintf(htmlFile, "</head>\n");
    fprintf(htmlFile, "<body>\n");

    fprintf(htmlFile, "<h1>Expenses Table for Account ID: %d</h1>\n", accountId);
    fprintf(htmlFile, "<h2>Year: %d</h2>\n", year);
    fprintf(htmlFile, "<table border=\"1\">\n");
    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<th>Category</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (SGD)</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (USD)</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (EUR)</th>\n");
    fprintf(htmlFile, "</tr>\n");

    totalFoodSGDYear = accountTotalsYear[accountId].totalFoodSGDYear;
    totalFoodUSDYear = accountTotalsYear[accountId].totalFoodUSDYear;
    totalFoodEURYear = accountTotalsYear[accountId].totalFoodEURYear;
    totalTransportSGDYear = accountTotalsYear[accountId].totalTransportSGDYear;
    totalTransportUSDYear = accountTotalsYear[accountId].totalTransportUSDYear;
    totalTransportEURYear = accountTotalsYear[accountId].totalTransportEURYear;
    totalShoppingSGDYear = accountTotalsYear[accountId].totalShoppingSGDYear;
    totalShoppingUSDYear = accountTotalsYear[accountId].totalShoppingUSDYear;
    totalShoppingEURYear = accountTotalsYear[accountId].totalShoppingEURYear;
    totalOthersSGDYear = accountTotalsYear[accountId].totalOthersSGDYear;
    totalOthersUSDYear = accountTotalsYear[accountId].totalOthersUSDYear;
    totalOthersEURYear = accountTotalsYear[accountId].totalOthersEURYear;

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Food</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodSGDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodUSDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Transport</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportSGDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportUSDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Shopping</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingSGDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingUSDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Others</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersSGDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersUSDYear);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "</table>\n");
    fprintf(htmlFile, "</body>\n");
    fprintf(htmlFile, "</html>\n");
}


int main(int argc, char **argv) {
    int numExpenses;
    Expenses *expenses;
    cJSON *json;
    FILE *htmlFile;
    int inputAccountId, userId;
    int i;
    int year;
    cJSON *accountsJson ;
    Account *accountsData;


    accountsJson = parseAccountsJSONfile("../ParserAccounts/Accounts.json");
    if (accountsJson == NULL) {
        printf("Error: Failed to parse Accounts.json\n");
        return 1;
    }


    accountsData = processAccountsData(accountsJson, &numAccounts);
    cJSON_Delete(accountsJson);

   
    for (i = 0; i < numAccounts; i++) {
        accountInfo[i].account_id = accountsData[i].account_id;
        accountInfo[i].user_id = accountsData[i].user_id;
    }
    free(accountsData);

    
    printf("Enter the user ID: ");
    scanf("%d", &userId);
    printf("Enter the account ID: ");
    scanf("%d", &inputAccountId);


    json = parseExpensesJSONfile("../ParserExpenses/Expenses.json");
    if (json == NULL) {
        printf("Error: Failed to parse Expenses.json\n");
        return 1;
    }

    expenses = processExpensesData(json, &numExpenses);
    cJSON_Delete(json);


    htmlFile = fopen("Page4-AccYearlyExpenses.html", "w");
    if (htmlFile == NULL) {
        printf("Error: Unable to create HTML file\n");
        return 1;
    }

    printf("Enter the year: ");
    scanf("%d", &year);

    categorizeExpensesByYear(expenses, numExpenses, year);

    generateHTMLTableForAccountYear(inputAccountId, htmlFile, year);


    fclose(htmlFile);

    free(expenses);

#ifdef _WIN32
    system("start Page4-AccYearlyExpenses.html");
#elif __linux__ || __APPLE__
    #ifdef __linux__
    system("xdg-open Page4-AccYearlyExpenses.html");
    #elif __APPLE__
    system("open Page4-AccYearlyExpenses.html");
    #endif
#endif

    return 0;
}
