#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#define MAX_ACCOUNTS 10

typedef struct
{
    int account_id;
    int user_id;
    char name[50];            /* We use fixed-size arrays for name, currency because we assume there are only fixed set of account types and 3 characters for currency code */
    char default_currency[4]; /* Assuming 3 characters for country code */
    float balance;
} Account;

void parse_accountsjson(const char *json, Account account[], int *num_accounts, int *error);
int isValidAccountsID(const int account_id);
int isValidUserID(const int user_id);
int isValidname(const char name[]);
int isValidDefaultCurrency(const char *currency);
int isValidBalance(const float balance);
int isValidAccountKey(const char *current_pos, const char **next_pos, char *expected_key);

#endif /* ACCOUNTS_H */
