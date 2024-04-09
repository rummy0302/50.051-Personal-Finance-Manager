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

void parse_accountsjson(const char *json, Account account[], int *num_accounts);
int validateAccounts( Account account[], int num_accounts);

#endif /* ACCOUNTS_H */
