#ifndef EXPENSES_H
#define EXPENSES_H

#define MAX_EXPENSES 400 /* Maximum number of expenses to store */
typedef struct
{
    int account_id;
    char date[20];
    char description[100];
    char currency[4];
    float amount_spent;
} Expenses;

void parse_expensesjson(const char *json, Expenses expenses[], int *num_expenses, int *error);
int isValidExpenseKey(const char *current_pos, const char **next_pos, char *expected_key);
int isValidAmountSpent(const float amount_spend);
int isValidCurrency(const char *currency);
int isValidDescription(const char *description);
int isValidDate(const char *date);
int isValidAccountID(const int account_id);

#endif /* EXPENSES_H */
