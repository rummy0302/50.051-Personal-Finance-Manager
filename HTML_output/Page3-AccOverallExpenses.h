#ifndef PAGE3_ACCOVERALLEXPENSES_H
#define PAGE3_ACCOVERALLEXPENSES_H
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"
#include <stdio.h>

#define MIN_YEAR 2020 /* Expense data starts from year 2020 */

typedef struct
{
    double totalFood;
    double totalTransport;
    double totalShopping;
    double totalOthers;
} AccountTotals;

typedef struct
{
    double totalFoodSGD;
    double totalTransportSGD;
    double totalShoppingSGD;
    double totalOthersSGD;
    double totalFoodUSD;
    double totalTransportUSD;
    double totalShoppingUSD;
    double totalOthersUSD;
    double totalFoodEUR;
    double totalTransportEUR;
    double totalShoppingEUR;
    double totalOthersEUR;
} AccountExpenses;

void categorizeExpenses(Expenses *expenses, int numExpenses);

void generateHTMLForAccount(Expenses *expenses, int numExpenses, int accountId, FILE *htmlFile);

#endif /* PAGE3_ACCOVERALLEXPENSES_H */
