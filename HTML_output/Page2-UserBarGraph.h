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

#endif /* PAGE2__USERBARGRAPH_H */
