
#ifndef FSM_INPUT_H
#define FSM_INPUT_H

#include "ParserExpenses/cJSON.h"
#include "ParserAccounts/parserAccounts.h"
#include "ParserExpenses/ParserExpenses.h"

typedef enum
{
    INITIAL_STATE,
    READING_ACCOUNT,
    READING_EXPENSES,
    COMPLETED,
} InputState;

typedef struct
{
    InputState currentState;
    cJSON *accountsJson;
    cJSON *expensesJson;
    Account *accounts;
    Expenses *expenses;
} InputFSM;

void initInputFSM(InputFSM *inputFsm);
void processInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile);
int runInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile);

#endif // FSM_INPUT_H
