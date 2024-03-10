#ifndef EXPENSES_TABLE_H
#define EXPENSES_TABLE_H
#include "ParserExpenses.h"
#define MAX_ACCOUNTS 10


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
void printAllExpenses(Expenses *expenses, int numExpenses);



#endif /* EXPENSES_TABLE_H */
