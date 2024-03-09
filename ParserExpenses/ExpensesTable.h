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
    float totalFood;
    float totalTransport;
    float totalShopping;
    float totalOthers;
} AccountExpenses;

void categorizeExpenses(Expenses *expenses, int numExpenses);
void printAllExpenses(Expenses *expenses, int numExpenses);
void printExpenses(int accountId, double totalFood, double totalTransport, double totalShopping, double totalOthers);
   
#endif /* EXPENSES_TABLE_H */
