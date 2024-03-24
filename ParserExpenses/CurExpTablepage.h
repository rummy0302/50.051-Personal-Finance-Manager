#ifndef CUR_EXP_TABLE_PAGE_H
#define CUR_EXP_TABLE_PAGE_H
#include "ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"
#include <stdio.h>
#define MAX_ACCOUNTS 100



typedef struct {
    double totalFood;
    double totalTransport;
    double totalShopping;
    double totalOthers;
} AccountTotals;

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

int isValidAccount(int accountId, int userId);

void categorizeExpenses(Expenses *expenses, int numExpenses);
void printAllExpenses(Expenses *expenses, int numExpenses);
void categorizeExpensesByYear(Expenses *expenses, int numExpenses, int year) ;

void printExpenses(int accountId, double totalFoodSGD, double totalFoodUSD, double totalFoodEUR,
                   double totalTransportSGD, double totalTransportUSD, double totalTransportEUR,
                   double totalShoppingSGD, double totalShoppingUSD, double totalShoppingEUR,
                   double totalOthersSGD, double totalOthersUSD, double totalOthersEUR);


void generateHTMLTableForAccount(int accountId, FILE *htmlFile); 

void generateHTMLTableForAccountYear(int accountId, FILE *htmlFile, int year);

#endif /* CUR_EXP_TABLE_PAGE_H */
