#ifndef FSM_OUTPUT_H
#define FSM_OUTPUT_H

typedef enum
{
    CHECK_USERID,
    CHECK_ACCOUNTID,
    CHECK_YEAR
} OutputState;

typedef struct
{
    OutputState currentState;
    int userID;
    int accountID;
} OutputFSM;

int isValidAccount(int accountId, int userId);
int isValidUser(int userId);
int isYearInList(int *years, int numYears, int year);
void initOutputFSM(OutputFSM *OutputFsm);
void openHTMLPage(const char *pageName);
void handleInput(OutputFSM *OutputFSM, int input);

#endif /*FSM_OUTPUT_H*/
