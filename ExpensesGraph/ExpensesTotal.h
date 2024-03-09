#ifndef EXPENSES_TOTAL_H
#define EXPENSES_TOTAL_H
#include "../ParserExpenses/ParserExpenses.h"
#include "../ParserAccounts/ParserAccounts.h"

#define MAX_ACCOUNTS 10
#define MAX_USERS 50


typedef struct {
    double totalSGD;
    double totalUSD;
    double totalEUR;
} CurrencyTotals;


void calculateCurrencyTotal(Expenses *expenses, int numExpenses);
void printAllCurrencyTotals(Expenses *expenses, int numExpenses, Account *accounts, int numAccounts);
void printCurrencyTotal(int accountId, double totalSGD, double totalUSD, double totalEUR);
   
#endif /* EXPENSES_TOTAL_H */
