#ifndef EXPENSES_H
#define EXPENSES_H

typedef struct {
    int account_id;
    char date[20];
    char description[100];
    char currency[4];
    float amount_spent;
} Expenses;

void parse_json(const char *json, Expenses expenses[], int *num_expenses);

#endif /* EXPENSES_H */
