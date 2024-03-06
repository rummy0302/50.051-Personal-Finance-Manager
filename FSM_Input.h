
#ifndef FSM_INPUT_H
#define FSM_INPUT_H

#include "cJSON.h"
#include "ParserAccounts/parseAccounts.h"
#include "Expenses.h"

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
void processInputState(InputFSM *inputFSM);
int runInputState(InputFSM *inputFSM);

#endif // FSM_INPUT_H
