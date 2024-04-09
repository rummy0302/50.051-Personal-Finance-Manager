#ifndef PAGE4_SOMEOTHERHEADER_H
#define PAGE4_SOMEOTHERHEADER_H
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"
#include <stdio.h>

#define MAX_MONTHS 13 /* Expense data for 12 months of the year */

typedef struct
{
    double totalFoodYear;
    double totalTransportYear;
    double totalShoppingYear;
    double totalOthersYear;
} AccountTotalsYear;

typedef struct
{
    double totalFoodSGDYear;
    double totalTransportSGDYear;
    double totalShoppingSGDYear;
    double totalOthersSGDYear;
    double totalFoodUSDYear;
    double totalTransportUSDYear;
    double totalShoppingUSDYear;
    double totalOthersUSDYear;
    double totalFoodEURYear;
    double totalTransportEURYear;
    double totalShoppingEURYear;
    double totalOthersEURYear;
} AccountExpensesYear;

void categorizeExpensesByYear(Expenses *expenses, int numExpenses, int year);

void generateHTMLForAccountYear(Expenses *expenses, int numExpenses, int accountId, FILE *htmlFile, int year);

int *getYearsFromExpenses(Expenses *expenses, int numExpenses, int accountId, int *numYears);

int isYearInList(int *years, int numYears, int year);

#endif /* PAGE4_SOMEOTHERHEADER_H */
