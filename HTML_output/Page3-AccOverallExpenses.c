#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Page3-AccOverallExpenses.h"
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"
#include "Common.h"

/* This file is to generate the account expense graphs (total expense in SGD, USD, EUR) for all years for each account -
    1. Calculates total expense in SGD, EUR, USD for each account for each year
    2. Displays a scatter plot showing the total expense in SGD, EUR, USD for each account for each year
*/

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <unistd.h>
#endif

AccountInfo accountInfoPage3[MAX_ACCOUNTS];
int numAccountsPage3 = 0;

typedef enum
{
    SGD,
    USD,
    EUR,
    NUM_CURRENCIES,
    UNKNOWN_CURRENCY
} CurrencyType;

const char *currencyStrings[NUM_CURRENCIES] = {
    "SGD",
    "USD",
    "EUR"};

CurrencyType getCurrencyType(const char *currencyStr)
{
    int i;
    for (i = 0; i < NUM_CURRENCIES; i++)
    {
        if (strcmp(currencyStr, currencyStrings[i]) == 0)
        {
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
    {"Clothing Store", "Shopping"}};

AccountExpenses accountTotals[MAX_ACCOUNTS];

void categorizeExpenses(Expenses *expenses, int numExpenses)
{
    int i, j;

    memset(accountTotals, 0, sizeof(accountTotals));

    for (i = 0; i < numExpenses; i++)
    {
        double amount = expenses[i].amount_spent;
        int found = 0;

        for (j = 0; j < sizeof(shops) / sizeof(shops[0]); j++)
        {
            if (strcmp(expenses[i].description, shops[j].shopName) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {

            CurrencyType currency = getCurrencyType(expenses[i].currency);

            switch (currency)
            {
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
        }
        else
        {

            CurrencyType currency = getCurrencyType(expenses[i].currency);

            switch (currency)
            {
            case SGD:
                if (strcmp(shops[j].category, "Food") == 0)
                {
                    accountTotals[expenses[i].account_id].totalFoodSGD += amount;
                }
                else if (strcmp(shops[j].category, "Transport") == 0)
                {
                    accountTotals[expenses[i].account_id].totalTransportSGD += amount;
                }
                else if (strcmp(shops[j].category, "Shopping") == 0)
                {
                    accountTotals[expenses[i].account_id].totalShoppingSGD += amount;
                }
                else
                {
                    accountTotals[expenses[i].account_id].totalOthersSGD += amount;
                }
                break;
            case USD:
                if (strcmp(shops[j].category, "Food") == 0)
                {
                    accountTotals[expenses[i].account_id].totalFoodUSD += amount;
                }
                else if (strcmp(shops[j].category, "Transport") == 0)
                {
                    accountTotals[expenses[i].account_id].totalTransportUSD += amount;
                }
                else if (strcmp(shops[j].category, "Shopping") == 0)
                {
                    accountTotals[expenses[i].account_id].totalShoppingUSD += amount;
                }
                else
                {
                    accountTotals[expenses[i].account_id].totalOthersUSD += amount;
                }
                break;
            case EUR:
                if (strcmp(shops[j].category, "Food") == 0)
                {
                    accountTotals[expenses[i].account_id].totalFoodEUR += amount;
                }
                else if (strcmp(shops[j].category, "Transport") == 0)
                {
                    accountTotals[expenses[i].account_id].totalTransportEUR += amount;
                }
                else if (strcmp(shops[j].category, "Shopping") == 0)
                {
                    accountTotals[expenses[i].account_id].totalShoppingEUR += amount;
                }
                else
                {
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

void generateHTMLForAccount(Expenses *expenses, int numExpenses, int accountId, FILE *htmlFile)
{
    double totalFoodSGD, totalFoodUSD, totalFoodEUR;
    double totalTransportSGD, totalTransportUSD, totalTransportEUR;
    double totalShoppingSGD, totalShoppingUSD, totalShoppingEUR;
    double totalOthersSGD, totalOthersUSD, totalOthersEUR;

    int i;

    /* Define arrays to store the total expense of each year for each currency : */
    double totalExpensesSGD[MAX_YEARS] = {0};
    double totalExpensesUSD[MAX_YEARS] = {0};
    double totalExpensesEUR[MAX_YEARS] = {0};

    /* Write HTML header */

    fprintf(htmlFile, "<!DOCTYPE html>\n");
    fprintf(htmlFile, "<html>\n");
    fprintf(htmlFile, "<head>\n");
    fprintf(htmlFile, "<title>Overall Expenses Table and Graph</title>\n");

    /* Call plotly library from js to render the graphs : */
    fprintf(htmlFile, "<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n");

    fprintf(htmlFile, "</head>\n");
    fprintf(htmlFile, "<body>\n");

    fprintf(htmlFile, "<h1>Expenses Table & Graph for Account ID: %d</h1>\n", accountId);
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
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodSGD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodUSD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Transport</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportSGD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportUSD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Shopping</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingSGD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingUSD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Others</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersSGD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersUSD);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersEUR);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "</table>\n");

    fprintf(htmlFile, "<h2 style=\"margin-top: 100px;\">Overall Expenses Graph</h2>\n");

    fprintf(htmlFile, "<div id=\"plotly_graph\"></div>\n");

    /* Iterate through the Expenses.json file and get the total expense for each year for each currency : */

    for (i = 0; i < numExpenses; i++)
    {
        int expenseYear = atoi(strtok(expenses[i].date, "-"));

        CurrencyType currency = getCurrencyType(expenses[i].currency);

        if (expenses[i].account_id == accountId)
        { /* Check if the account Id matches the account Id provided by the user */

            switch (currency)
            {
            case SGD:
                totalExpensesSGD[expenseYear - MIN_YEAR] += expenses[i].amount_spent;
                break;
            case USD:
                totalExpensesUSD[expenseYear - MIN_YEAR] += expenses[i].amount_spent;
                break;
            case EUR:
                totalExpensesEUR[expenseYear - MIN_YEAR] += expenses[i].amount_spent;
                break;
            default:
                break;
            }
        }
    }

    fprintf(htmlFile, "<script>\n");
    fprintf(htmlFile, "var data = [\n");

    /* Plot the scatter plot with Year vs Expense amount with 3 different lines for each currency : */

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [%d, %d, %d, %d, %d],\n", MIN_YEAR, MIN_YEAR + 1, MIN_YEAR + 2, MIN_YEAR + 3, MIN_YEAR + 4);
    fprintf(htmlFile, "tickmode: 'linear',\n");
    fprintf(htmlFile, "dtick: 1,\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f, %.2f, %.2f],\n", totalExpensesSGD[0], totalExpensesSGD[1], totalExpensesSGD[2], totalExpensesSGD[3], totalExpensesSGD[4]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'SGD',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: 'blue'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "},\n");

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [%d, %d, %d, %d, %d],\n", MIN_YEAR, MIN_YEAR + 1, MIN_YEAR + 2, MIN_YEAR + 3, MIN_YEAR + 4);
    fprintf(htmlFile, "tickmode: 'linear',\n");
    fprintf(htmlFile, "dtick: 1,\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f,%.2f, %.2f],\n", totalExpensesUSD[0], totalExpensesUSD[1], totalExpensesUSD[2], totalExpensesUSD[3], totalExpensesUSD[4]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'USD',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: 'green'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "},\n");

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [%d, %d, %d, %d, %d],\n", MIN_YEAR, MIN_YEAR + 1, MIN_YEAR + 2, MIN_YEAR + 3, MIN_YEAR + 4);
    fprintf(htmlFile, "tickmode: 'linear',\n");
    fprintf(htmlFile, "dtick: 1,\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f, %.2f, %.2f],\n", totalExpensesEUR[0], totalExpensesEUR[1], totalExpensesEUR[2], totalExpensesEUR[3], totalExpensesEUR[4]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'EUR',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: 'red'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "];\n");
    fprintf(htmlFile, "var layout = {\n");
    fprintf(htmlFile, "xaxis: {\n");
    fprintf(htmlFile, "title: 'Year',\n");
    fprintf(htmlFile, "tickmode: 'linear',\n");
    fprintf(htmlFile, "dtick: 1,\n");
    fprintf(htmlFile, "},\n");
    fprintf(htmlFile, "yaxis: {\n");
    fprintf(htmlFile, "title: 'Expense',\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "};\n");
    fprintf(htmlFile, "Plotly.newPlot('plotly_graph', data, layout);\n");
    fprintf(htmlFile, "</script>\n");

    fprintf(htmlFile, "</body>\n");
    fprintf(htmlFile, "</html>\n");
}

/* int main(int argc, char **argv)
{
    int numExpenses;
    Expenses *expenses;
    cJSON *json;
    FILE *htmlFile;
    int inputAccountId, userId;
    int i;
    cJSON *accountsJson;
    Account *accountsData;

    accountsJson = parseAccountsJSONfile("../ParserAccounts/Accounts.json");
    if (accountsJson == NULL)
    {
        printf("Error: Failed to parse Accounts.json\n");
        return 1;
    }

    accountsData = processAccountsData(accountsJson, &numAccountsPage3);
    cJSON_Delete(accountsJson);

    for (i = 0; i < numAccountsPage3; i++)
    {
        accountInfoPage3[i].account_id = accountsData[i].account_id;
        accountInfoPage3[i].user_id = accountsData[i].user_id;
    }
    free(accountsData);

    printf("Enter the user ID: ");
    scanf("%d", &userId);
    printf("Enter the account ID: ");
    scanf("%d", &inputAccountId);

    json = parseExpensesJSONfile("../ParserExpenses/Expenses.json");
    if (json == NULL)
    {
        printf("Error: Failed to parse Expenses.json\n");
        return 1;
    }

    expenses = processExpensesData(json, &numExpenses);
    cJSON_Delete(json);

    categorizeExpenses(expenses, numExpenses);

    htmlFile = fopen("Page3-AccOverallExpenses.html", "w");
    if (htmlFile == NULL)
    {
        printf("Error: Unable to create HTML file\n");
        return 1;
    }

    generateHTMLForAccount(expenses, numExpenses, inputAccountId, htmlFile);

    fclose(htmlFile);

    free(expenses);

#ifdef _WIN32
    system("start Page3-AccOverallExpenses.html");
#elif __linux__ || __APPLE__
#ifdef __linux__
    system("xdg-open Page3-AccOverallExpenses.html");
#elif __APPLE__
    system("open Page3-AccOverallExpenses.html");
#endif
#endif

    return 0;
}*/
