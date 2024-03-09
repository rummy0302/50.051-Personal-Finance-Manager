#include "cJSON.h"

typedef struct
{
    int account_id;
    int user_id;
    char name[50];    /* We use fixed-size arrays for name, currency because we assume there are only fixed set of account types and 3 characters for currency code */
    char default_currency[4]; /* Assuming 3 characters for country code */
    double balance;
} Account;

cJSON *parseAccountsJSONfile(const char *filename); /* Helper function to get the json object for the Accounts.json file */

Account *processAccountsData(cJSON *json, int *numAccounts); /* Helper function to process the parsed data and store into the account struct */
