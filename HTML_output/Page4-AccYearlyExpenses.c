#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Page4-AccYearlyExpenses.h"
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"
#include "Common.h"

/* This file is to generate the categorization table by account for a particular year and display account expense graphs (total expense in SGD, USD, EUR) for 1 particular year for each account -

    Yearly Categorization Table
    1. Categorizes the expenses according to the Shop dictionary
    2. Updates the total value for each category according to year
    3. Displays the final total for each category in a table format

    Account Yearly Expenses Graph
    1. Calculates total expense in SGD, EUR, USD for each account for that particular year
    2. Displays a scatter plot showing the total expense in SGD, EUR, USD for each account for each month in that year
*/

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <unistd.h>
#endif

AccountInfo accountInfoPage4[MAX_ACCOUNTS];
int numAccountsPage4;

/*Function to extract unique years from expenses for a given account ID*/
int *getYearsFromExpenses(Expenses *expenses, int numExpenses, int accountId, int *numYears)
{
    int i;
    static int years[MAX_YEARS];
    int tempYears[MAX_YEARS];
    *numYears = 0;

    for (i = 0; i < numExpenses; i++)
    {
        if (expenses[i].account_id == accountId)
        {
            /* Extract the year from the date*/
            char yearStr[5];
            int year;
            int yearExists;
            int j;
            strncpy(yearStr, expenses[i].date, 4);
            yearStr[4] = '\0';
            year = atoi(yearStr);

            /* Check if this year is already in our list*/
            yearExists = 0;

            for (j = 0; j < *numYears; j++)
            {
                if (tempYears[j] == year)
                {
                    yearExists = 1;
                    break;
                }
            }
            /* If the year is not already in our list, add it*/
            if (!yearExists)
            {
                tempYears[*numYears] = year;
                (*numYears)++;
            }
        }
    }

    /*Copy the years to the static array*/
    for (i = 0; i < *numYears; i++)
    {
        years[i] = tempYears[i];
    }

    return years;
}

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
const char *currencyStrings_[NUM_CURRENCIES] = {
    "SGD",
    "USD",
    "EUR"};

/* Function to get the currency type based on its string representation */
CurrencyType getCurrencyType_(const char *currencyStr)
{
    int i;
    /* Iterate through each currency string in the currencyStrings array */
    for (i = 0; i < NUM_CURRENCIES; i++)
    {
        /* Compare the input currency string with the current currency string */
        if (strcmp(currencyStr, currencyStrings_[i]) == 0)
        {
            /* If a match is found, return the corresponding CurrencyType */
            return (CurrencyType)i;
        }
    }
    /* If no match is found, return UNKNOWN_CURRENCY */
    return UNKNOWN_CURRENCY;
}

/* Array of shops and categorization */
Shop shops_[] = {
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

AccountExpensesYear accountTotalsYear[MAX_ACCOUNTS];

/* Function: categorizeExpensesByYear
 * -----------------------------------
 * This function categorizes expenses for a specific year based on the provided list of expenses
 * and updates account totals accordingly.
 *
 * expenses:    Pointer to the array of Expenses structures containing expense data.
 * numExpenses: Number of expenses in the array.
 * year:        The year to categorize expenses for.
 */
void categorizeExpensesByYear(Expenses *expenses, int numExpenses, int year)
{
    int i, j;

    memset(accountTotalsYear, 0, sizeof(accountTotalsYear)); /* Reset accountTotalsYear array to zero */

    for (i = 0; i < numExpenses; i++) /* Loop through each expense */
    {
        double amount = expenses[i].amount_spent;
        int found = 0;

        int expenseYear = atoi(strtok(expenses[i].date, "-")); /* Extract the year from the expense date and compare with the specified year */
        if (expenseYear != year)
        {
            continue; /* Skip the expense if it doesn't belong to the specified year*/
        }

        for (j = 0; j < sizeof(shops_) / sizeof(shops_[0]); j++) /* Check if the expense matches any shop */
        {
            if (strcmp(expenses[i].description, shops_[j].shopName) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found) /* If the expense is not from a shop, categorize it as 'Others' */

        {

            CurrencyType currency = getCurrencyType_(expenses[i].currency); /* Get the currency type */
            switch (currency)                                               /* Update account totals based on currency */
            {
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
        }

        else
        {

            CurrencyType currency = getCurrencyType_(expenses[i].currency); /* Get the currency type */
            switch (currency)                                               /* Categorize the expense based on the shop's category and update account totals */
            {
            case SGD:
                if (strcmp(shops_[j].category, "Food") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalFoodSGDYear += amount;
                }
                else if (strcmp(shops_[j].category, "Transport") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalTransportSGDYear += amount;
                }
                else if (strcmp(shops_[j].category, "Shopping") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalShoppingSGDYear += amount;
                }
                else
                {
                    accountTotalsYear[expenses[i].account_id].totalOthersSGDYear += amount;
                }
                break;
            case USD:
                if (strcmp(shops_[j].category, "Food") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalFoodUSDYear += amount;
                }
                else if (strcmp(shops_[j].category, "Transport") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalTransportUSDYear += amount;
                }
                else if (strcmp(shops_[j].category, "Shopping") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalShoppingUSDYear += amount;
                }
                else
                {
                    accountTotalsYear[expenses[i].account_id].totalOthersUSDYear += amount;
                }
                break;
            case EUR:
                if (strcmp(shops_[j].category, "Food") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalFoodEURYear += amount;
                }
                else if (strcmp(shops_[j].category, "Transport") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalTransportEURYear += amount;
                }
                else if (strcmp(shops_[j].category, "Shopping") == 0)
                {
                    accountTotalsYear[expenses[i].account_id].totalShoppingEURYear += amount;
                }
                else
                {
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

void generateHTMLForAccountYear(Expenses *expenses, int numExpenses, int accountId, FILE *htmlFile, int year)
{
    double totalFoodSGDYear = 0, totalFoodUSDYear = 0, totalFoodEURYear = 0;
    double totalTransportSGDYear = 0, totalTransportUSDYear = 0, totalTransportEURYear = 0;
    double totalShoppingSGDYear = 0, totalShoppingUSDYear = 0, totalShoppingEURYear = 0;
    double totalOthersSGDYear = 0, totalOthersUSDYear = 0, totalOthersEURYear = 0;

    int i;

    CurrencyType currency;

    int expenseYear, expenseMonth;

    /* Define arrays to store the total expense of each month for the particular year for each currency : */

    double totalExpensesSGD[MAX_MONTHS] = {0};
    double totalExpensesUSD[MAX_MONTHS] = {0};
    double totalExpensesEUR[MAX_MONTHS] = {0};

    fprintf(htmlFile, "<!DOCTYPE html>\n");
    fprintf(htmlFile, "<html>\n");
    fprintf(htmlFile, "<head>\n");
    fprintf(htmlFile, "<title>Expenses Table</title>\n");

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
    fprintf(htmlFile, "    width: 80%%; \n");

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

    fprintf(htmlFile, ".year {\n");
    fprintf(htmlFile, "    font-weight: bold;\n");
    fprintf(htmlFile, "    color: #004F99;\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "</style>\n");

    fprintf(htmlFile, "</head>\n<body>\n");

    fprintf(htmlFile, "<div class=\"headbar\">\n");
    fprintf(htmlFile, "  <div class=\"appName\">WealthWise</div>\n");

    fprintf(htmlFile, "  </div>\n");
    fprintf(htmlFile, "</div>\n");

    fprintf(htmlFile, "</head>\n");
    fprintf(htmlFile, "<body>\n");

    fprintf(htmlFile, "<h1 class=\"main-header\">Welcome to your financial overview!</h1>\n");
    fprintf(htmlFile, "<p class=\"account-overview\"> Here's a detailed breakdown of expenses for  <span class=\"year\"> Account ID %d</span> in <span class=\"year\"> Year %d</span>.</p>\n", accountId, year);

    fprintf(htmlFile, "<h3>Overall Expenses Table</h3>\n");
    fprintf(htmlFile, "<p class=\"account-overview\"> The Overall Expenses Table shows a breakdown of the expenses according to different categories over the various currencies for a particular year.</p>\n");

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
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodSGDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodUSDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalFoodEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Transport</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportSGDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportUSDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalTransportEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Shopping</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingSGDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingUSDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalShoppingEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "<tr>\n");
    fprintf(htmlFile, "<td>Others</td>\n");
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersSGDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersUSDYear);
    fprintf(htmlFile, "<td>%.2f</td>\n", totalOthersEURYear);
    fprintf(htmlFile, "</tr>\n");

    fprintf(htmlFile, "</table>\n");

    fprintf(htmlFile, "<h3 class=\"lineGraphTitle\">Overall Expenses Graph</h3>\n");
    fprintf(htmlFile, "<p class=\"account-overview\"> The Overall Expenses Graph shows a breakdown of the expenses according to different currencies for a particular year.</p>\n");

    fprintf(htmlFile, "<div id=\"plotly_graph\" class=\"plotly-graph-container\"></div>\n");

    /* Iterate through the Expenses.json file and get the total expense for each month for that year for each currency : */

    for (i = 0; i < numExpenses; i++)
    {
        expenseYear = atoi(strtok(expenses[i].date, "-"));
        expenseMonth = (expenses[i].date[5] - '0') * 10 + (expenses[i].date[6] - '0'); /* Get the month from the date string */

        currency = getCurrencyType_(expenses[i].currency);

        if ((expenses[i].account_id == accountId) && (expenseYear == year))
        { /* Check if the account Id and year matches the account Id and year provided by the user */

            switch (currency)
            {
            case SGD:
                totalExpensesSGD[expenseMonth] += expenses[i].amount_spent;
                break;
            case USD:
                totalExpensesUSD[expenseMonth] += expenses[i].amount_spent;
                break;
            case EUR:
                totalExpensesEUR[expenseMonth] += expenses[i].amount_spent;
                break;
            default:
                break;
            }
        }
    }

    fprintf(htmlFile, "<script>\n");
    fprintf(htmlFile, "var data = [\n");

    /* Plot the scatter plot with Month vs Expense amount with 3 different lines for each currency for that particular year: */

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [\"Jan\", \"Feb\", \"Mar\", \"Apr\", \"May\", \"Jun\", \"Jul\", \"Aug\", \"Sep\", \"Oct\", \"Nov\", \"Dec\"],\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f],\n", totalExpensesSGD[1], totalExpensesSGD[2], totalExpensesSGD[3], totalExpensesSGD[4], totalExpensesSGD[5], totalExpensesSGD[6], totalExpensesSGD[7], totalExpensesSGD[8], totalExpensesSGD[9], totalExpensesSGD[10], totalExpensesSGD[11], totalExpensesSGD[12]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'SGD',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: '#2370F7'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "},\n");

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [\"Jan\", \"Feb\", \"Mar\", \"Apr\", \"May\", \"Jun\", \"Jul\", \"Aug\", \"Sep\", \"Oct\", \"Nov\", \"Dec\"],\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f],\n", totalExpensesUSD[1], totalExpensesUSD[2], totalExpensesUSD[3], totalExpensesUSD[4], totalExpensesUSD[5], totalExpensesUSD[6], totalExpensesUSD[7], totalExpensesUSD[8], totalExpensesUSD[9], totalExpensesUSD[10], totalExpensesUSD[11], totalExpensesUSD[12]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'USD',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: '#76D7C4'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "},\n");

    fprintf(htmlFile, "{\n");
    fprintf(htmlFile, "x: [\"Jan\", \"Feb\", \"Mar\", \"Apr\", \"May\", \"Jun\", \"Jul\", \"Aug\", \"Sep\", \"Oct\", \"Nov\", \"Dec\"],\n");
    fprintf(htmlFile, "y: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f],\n", totalExpensesEUR[1], totalExpensesEUR[2], totalExpensesEUR[3], totalExpensesEUR[4], totalExpensesEUR[5], totalExpensesEUR[6], totalExpensesEUR[7], totalExpensesEUR[8], totalExpensesEUR[9], totalExpensesEUR[10], totalExpensesEUR[11], totalExpensesEUR[12]);
    fprintf(htmlFile, "mode: 'lines',\n");
    fprintf(htmlFile, "name: 'EUR',\n");
    fprintf(htmlFile, "line: {\n");
    fprintf(htmlFile, "color: '#F7DC6F'\n");
    fprintf(htmlFile, "}\n");
    fprintf(htmlFile, "}\n");

    fprintf(htmlFile, "];\n");
    fprintf(htmlFile, "var layout = {\n");
    fprintf(htmlFile, "xaxis: {\n");
    fprintf(htmlFile, "title: 'Month',\n");
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

    FILE *htmlFile;
    int inputAccountId, userId;
    int i;
    int year;

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

    for (i = 0; i < num_accounts; i++)
    {
        accountInfoPage4[i].account_id = accounts[i].account_id;
        accountInfoPage4[i].user_id = accounts[i].user_id;
    }

    htmlFile = fopen("Page4-AccYearlyExpenses.html", "w");
    if (htmlFile == NULL)
    {
        printf("Error: Unable to create HTML file\n");
        return 1;
    }

    printf("Enter the user ID: ");
    scanf("%d", &userId);
    printf("Enter the account ID: ");
    scanf("%d", &inputAccountId);

    printf("Enter the year: ");
    scanf("%d", &year);

    categorizeExpensesByYear(expenses, num_expenses, year);

    generateHTMLForAccountYear(expenses, num_expenses, inputAccountId, htmlFile, year);

    fclose(htmlFile);

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
} */
