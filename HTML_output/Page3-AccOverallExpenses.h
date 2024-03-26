#ifndef CUR_EXP_TABLE_PAGE_H
#define CUR_EXP_TABLE_PAGE_H
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"
#include <stdio.h>
#define MAX_ACCOUNTS 100

#define MAX_YEARS 6 /* Expense data ranges from 2020 to 2024 */
#define MIN_YEAR 2020 /* Expense data starts from year 2020 */




typedef struct {
    double totalFood;
    double totalTransport;
    double totalShopping;
    double totalOthers;
} AccountTotals;


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

void generateHTMLForAccount(Expenses *expenses, int numExpenses,int accountId, FILE *htmlFile); 

#endif /* CUR_EXP_TABLE_PAGE_H */
