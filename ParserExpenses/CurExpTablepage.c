#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "CurExpTablepage.h"
#include "ParserExpenses.h"
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


void generateHTMLTableForAccount(int accountId, FILE *htmlFile) {
    double totalFoodSGD, totalFoodUSD, totalFoodEUR;
    double totalTransportSGD, totalTransportUSD, totalTransportEUR;
    double totalShoppingSGD, totalShoppingUSD, totalShoppingEUR;
    double totalOthersSGD, totalOthersUSD, totalOthersEUR;
    fprintf(htmlFile, "<!DOCTYPE html>\n");
    fprintf(htmlFile, "<html>\n");
    fprintf(htmlFile, "<head>\n");
    fprintf(htmlFile, "<title>Expenses Table</title>\n");
    fprintf(htmlFile, "</head>\n");
    fprintf(htmlFile, "<body>\n");

    fprintf(htmlFile, "<h1>Expenses Table for Account ID: %d</h1>\n", accountId);
    fprintf(htmlFile, "<h2>Overall Expenses Table</h2>\n");
    fprintf(htmlFile, "<table border=\"1\">\n");
    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<th>Category</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (SGD)</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (USD)</th>\n");
    fprintf(htmlFile, "<th>Amount of Expense (EUR)</th>\n");
    fprintf(htmlFile, "</tr>\n");

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

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Food</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodSGD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodUSD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalFoodEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Transport</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportSGD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportUSD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalTransportEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Shopping</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingSGD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingUSD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalShoppingEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Others</td>\n");
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersSGD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersUSD);
    fprintf(htmlFile, "<td>%.2lf</td>\n", totalOthersEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "</table>\n");
    fprintf(htmlFile, "</body>\n");
    fprintf(htmlFile, "</html>\n");

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

 
    if (!isValidAccount(inputAccountId, userId)) {
        printf("Error: Invalid account ID\n");
        return 1;
    }

    json = parseExpensesJSONfile("Expenses.json");
    if (json == NULL) {
        printf("Error: Failed to parse Expenses.json\n");
        return 1;
    }

    expenses = processExpensesData(json, &numExpenses);
    cJSON_Delete(json);

    categorizeExpenses(expenses, numExpenses);

    htmlFile = fopen("expenses_table.html", "w");
    if (htmlFile == NULL) {
        printf("Error: Unable to create HTML file\n");
        return 1;
    }

    generateHTMLTableForAccount(inputAccountId, htmlFile);

    printf("Enter the year: ");
    scanf("%d", &year);

    categorizeExpensesByYear(expenses, numExpenses, year);

    generateHTMLTableForAccountYear(inputAccountId, htmlFile, year);


    fclose(htmlFile);

    free(expenses);

#ifdef _WIN32
    system("start expenses_table.html");
#elif __linux__ || __APPLE__
    #ifdef __linux__
    system("xdg-open expenses_table.html");
    #elif __APPLE__
    system("open expenses_table.html");
    #endif
#endif

    return 0;
}
