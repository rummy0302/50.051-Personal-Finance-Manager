#ifndef EXPENSES_H
#define EXPENSES_H
#include <stdbool.h>

#define MAX_EXPENSES 400 // Maximum number of expenses to store
typedef struct
{
    int account_id;
    char date[20];
    char description[100];
    char currency[4];
    float amount_spent;
} Expenses;

void parse_expensesjson(const char *json, Expenses expenses[], int *num_expenses);
bool validateExpenses(Expenses expenses[], int num_expenses);

#endif /* EXPENSES_H */
