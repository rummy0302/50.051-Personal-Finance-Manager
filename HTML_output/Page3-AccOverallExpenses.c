#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Page3-AccOverallExpenses.h"
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"
#include "Common.h"

/* This file is to generate the categorization table by account and display account expense graphs (total expense in SGD, USD, EUR) for all years for each account -

    Categorization Table
    1. Categorizes the expenses according to the Shop dictionary
    2. Updates the total value for each category
    3. Displays the final total for each category in a table format

    Account Expenses Graph
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

/* Define the types of currencies */
typedef enum
{
    SGD,
    USD,
    EUR,
    NUM_CURRENCIES,
    UNKNOWN_CURRENCY
} CurrencyType;

/* Array to store currency strings */
const char *currencyStrings[NUM_CURRENCIES] = {
    "SGD",
    "USD",
    "EUR"};

/* Function to get the currency type based on its string representation */
CurrencyType getCurrencyType(const char *currencyStr)
{
    int i;
    /* Iterate through each currency string in the currencyStrings array */
    for (i = 0; i < NUM_CURRENCIES; i++)
    {
        /* Compare the input currency string with the current currency string */
        if (strcmp(currencyStr, currencyStrings[i]) == 0)
        {
            /* If a match is found, return the corresponding CurrencyType */
            return (CurrencyType)i;
        }
    }
    /* If no match is found, return UNKNOWN_CURRENCY */
    return UNKNOWN_CURRENCY;
}

/* Array of shops and categorization */
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

/* Function: categorizeExpenses
 * ----------------------------
 * This function categorizes expenses based on the provided list of expenses
 * and updates account totals accordingly.
 *
 * expenses:  Pointer to the array of Expenses structures containing expense data.
 * numExpenses: Number of expenses in the array.
 */

void categorizeExpenses(Expenses *expenses, int numExpenses)
{
    int i, j;

    memset(accountTotals, 0, sizeof(accountTotals)); /* Reset accountTotals array to zero */

    for (i = 0; i < numExpenses; i++) /* Loop through each expense */
    {
        double amount = expenses[i].amount_spent;
        int found = 0;

        for (j = 0; j < sizeof(shops) / sizeof(shops[0]); j++) /* Check if the expense matches any shop */
        {
            if (strcmp(expenses[i].description, shops[j].shopName) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found) /* If the expense is not from a shop, categorize it as 'Others' */
        {

            CurrencyType currency = getCurrencyType(expenses[i].currency); /* Get the currency type */

            switch (currency) /* Update account totals based on currency */
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

            CurrencyType currency = getCurrencyType(expenses[i].currency); /* Get the currency type */

            switch (currency) /* Categorize the expense based on the shop's category and update account totals */
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
    double totalFoodSGD = 0 , totalFoodUSD=0, totalFoodEUR=0;
    double totalTransportSGD=0, totalTransportUSD=0, totalTransportEUR=0;
    double totalShoppingSGD=0, totalShoppingUSD=0, totalShoppingEUR=0;
    double totalOthersSGD=0, totalOthersUSD=0, totalOthersEUR=0;

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

    fprintf(htmlFile, "h2 {\n");
    fprintf(htmlFile, "    font-size: 18px;\n");
    fprintf(htmlFile, "    padding: 10px 20px;\n");
    fprintf(htmlFile, "    padding-bottom: 5px;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "h3 {\n");
    fprintf(htmlFile, "    font-size: 16px;\n");
    fprintf(htmlFile, "    padding: 5px 20px;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, ".lineGraphTitle {\n");
    fprintf(htmlFile, "    padding-bottom:0px ;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "table {\n");
    fprintf(htmlFile, "    border-collapse: collapse;\n");
    fprintf(htmlFile, "    padding: 5px 20px;\n");
    fprintf(htmlFile, "    margin-left: 20px;\n");
    fprintf(htmlFile, "    font-size: 14px;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "th, td {\n");
    fprintf(htmlFile, "    border: 1px solid #ddd;\n");
    fprintf(htmlFile, "    padding: 5px 10px;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "th {\n");
    fprintf(htmlFile, "    background-color: #f2f2f2;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, ".plotly-graph-container {\n");
    fprintf(htmlFile, "    padding: 0px;\n");
    fprintf(htmlFile, "    width: 90%%; \n");
    fprintf(htmlFile, "    height: 400px !important;\n");

    fprintf(htmlFile, "}\n");

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
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, ".account-id {\n");
    fprintf(htmlFile, "    font-weight: bold;\n");
    fprintf(htmlFile, "    color: #004F99;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "</style>\n");

    fprintf(htmlFile, "</head>\n<body>\n");

    fprintf(htmlFile, "<div class=\"headbar\">\n");
    fprintf(htmlFile, "  <div class=\"appName\">WealthWise</div>\n");
    fprintf(htmlFile, "</div>\n");

    fprintf(htmlFile, "</head>\n");
    fprintf(htmlFile, "<body>\n");

    fprintf(htmlFile, "<h1 class=\"main-header\">Welcome to your financial overview!</h1>\n");
    fprintf(htmlFile, "<p class=\"account-overview\">Here's the overall expenses for <span class=\"account-id\"> Account ID %d</span>.</p>\n", accountId);

    fprintf(htmlFile, "<h3>Overall Expenses Table</h3>\n");
    fprintf(htmlFile, "<p class=\"account-overview\"> The Overall Expenses Table shows a breakdown of the expenses according to different categories over the various currencies.</p>\n");
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

    fprintf(htmlFile, "<h3 class=\"lineGraphTitle\">Overall Expenses Graph</h3>\n");
    fprintf(htmlFile, "<p class=\"account-overview\"> The Overall Expenses Graph shows a breakdown of the expenses according to different currencies over the years.</p>\n");

    fprintf(htmlFile, "<div id=\"plotly_graph\" class=\"plotly-graph-container\"></div>\n");

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
    fprintf(htmlFile, "color: '#2370F7'\n");
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
    fprintf(htmlFile, "color: '#76D7C4'\n");
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
    fprintf(htmlFile, "color: '#F7DC6F'\n");
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

/*
int main(int argc, char **argv)
{
    int inputAccountId, userId;
    int i;
    FILE *htmlFile;

    int num_expenses = 0;
    int num_accounts = 0;

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

    for (i = 0; i < numAccountsPage3; i++)
    {
        accountInfoPage3[i].account_id = accounts[i].account_id;
        accountInfoPage3[i].user_id = accounts[i].user_id;
    }

    printf("Enter the user ID: ");
    scanf("%d", &userId);
    printf("Enter the account ID: ");
    scanf("%d", &inputAccountId);

    categorizeExpenses(expenses, num_expenses);

    htmlFile = fopen("Page3-AccOverallExpenses.html", "w");
    if (htmlFile == NULL)
    {
        printf("Error: Unable to create HTML file\n");
        return 1;
    }

    generateHTMLForAccount(expenses, num_expenses, inputAccountId, htmlFile);

    fclose(htmlFile);

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
} */
