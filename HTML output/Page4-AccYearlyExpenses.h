#ifndef CUR_EXP_TABLE_PAGE_H
#define CUR_EXP_TABLE_PAGE_H
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"
#include <stdio.h>
#define MAX_ACCOUNTS 100




typedef struct {
    double totalFoodYear;
    double totalTransportYear;
    double totalShoppingYear;
    double totalOthersYear;
} AccountTotalsYear;


typedef struct {
    char shopName[100];
    char category[20];
} Shop;


typedef struct {
    double totalSGD;
    double totalUSD;
    double totalEUR;
} Currency;


typedef struct {
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

void categorizeExpensesByYear(Expenses *expenses, int numExpenses, int year) ;

void generateHTMLTableForAccountYear(int accountId, FILE *htmlFile, int year);

#endif /* CUR_EXP_TABLE_PAGE_H */
