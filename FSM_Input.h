
#ifndef FSM_INPUT_H
#define FSM_INPUT_H

#include "ParserAccounts/AccountsParser.h"
#include "ParserExpenses/ExpenseParser.h"

typedef enum
{
    INITIAL_STATE,
    PARSE_ACCOUNT,
    PARSE_EXPENSES,
    COMPLETED,
    ERROR
} InputState;

typedef struct
{
    InputState currentState;
    Account accounts[MAX_ACCOUNTS];
    Expenses expenses[MAX_EXPENSES];

} InputFSM;

void initInputFSM(InputFSM *inputFsm);
void processInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile);
int runInputState(InputFSM *inputFSM, char *accountFile, char *expenseFile);

#endif /* FSM_INPUT_H */
