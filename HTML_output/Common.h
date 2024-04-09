#ifndef CUR_EXP_TABLE_PAGE_H
#define CUR_EXP_TABLE_PAGE_H
#include "../ParserExpenses/ExpenseParser.h"
#include "../ParserAccounts/AccountsParser.h"
#include <stdio.h>

#define MAX_YEARS 5 /* Expense data ranges from 2020 to 2024 */

typedef struct
{
    char shopName[100];
    char category[20];
} Shop;

typedef struct
{
    double totalSGD;
    double totalUSD;
    double totalEUR;
} Currency;

typedef struct
{
    int account_id;
    int user_id;
} AccountInfo;

#endif /* CUR_EXP_TABLE_PAGE_H */
