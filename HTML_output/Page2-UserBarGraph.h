#ifndef PAGE2__USERBARGRAPH_H
#define PAGE2__USERBARGRAPH_H
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"

#define MAX_USERS 50

typedef struct
{
    double totalExpenseInSGD;
} ExpenseTotalsSGD;

void calculateExpenseTotal(Expenses *expenses, int numExpenses);
void printExpenseTotal(int accountId, double totalExpenseInSGD);
void printAllExpenseTotals(int userIdInput, Expenses *expenses, int numExpenses, Account *accounts, int numAccounts, FILE *htmlFile);

#endif /* PAGE2__USERBARGRAPH_H */
