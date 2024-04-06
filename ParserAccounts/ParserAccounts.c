#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "ParserAccounts.h"

/* Parsing technique - Parse the Accounts.json file (containing information about user's multiple accounts) using the cJSON library
    1. Ensure the cJSON.h header file is imported to the project directory (downloaded from open source github repository)
    2. Define a struct to represent the different key-value pairs in a JSON object (defined in the header file)
    3. Call the helper function parseJSONfile() to get the json object for the Accounts.json file :
            3.1 Open the Accounts.json file
            3.2 Get the file size inorder to malloc space for the char * to store the file contents
            3.3 Read the file content and store it in the jsonContent array
            3.4 Parse the file data into a json object and return the json object
    4. Call the helper function processData() to process the parsed data and store as an array of Account struct :
            4.1 Get the size of the JSON array (i.e number of elements/accounts present in the Accounts.json file) and store it in the address pointed to by the numAccounts pointer
            4.2 Dynamically allocate memory for the accountsArray that is an array of structs (storing information about each account)
            4.3 Then iteratively in a for loop, retrieve the individual JSON object at index i.
            4.4 For each struct element in accountsArray at index i, populate the values of the struct datatype (accountsArray[i].accountid, etc.)
            4.5 Return the accountsArray.
    5. In the main() function, call the helper functions parseJSONfile() and processData() functions
    6. Then similarly, iterate through the accounts array and print the individual attributes of each account struct
    7. Free the space allocated for the accounts array
    8. Delete the json object after end of program.

*/

/* We define the Account struct in the parseAccounts.h header file to avoid errors when defining the prototype of the Account * processData() function in the header file
typedef struct {
    int account_id;
    int user_id;
    char name[50];  We use fixed-size arrays for name, currency because we assume there are only fixed set of account types and 3 characters for currency code
    char currency[4]; Assuming 3 characters for country code
    double balance;
} Account; */


cJSON *parseAccountsJSONfile(const char *filename)
{

    long fileSize;

    char *jsonContent;

    cJSON *json;

    /* Open the file Accounts.json : */

    FILE *accounts_file = fopen(filename, "r");
    if (accounts_file == NULL)
    {
        printf("Error: Unable to open Accounts.json file\n");
        return NULL;
    }

    /* Get the file size : */

    fseek(accounts_file, 0L, SEEK_END); /* Go to end of file */
    fileSize = ftell(accounts_file);

    /* Malloc fileSize amount of space to store the file contents : */

    jsonContent = (char *)malloc(fileSize + 1);
    if (jsonContent == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        return NULL;
    }

    /* Read the contents of the Accounts.json file : */

    fseek(accounts_file, 0L, SEEK_SET); /* Go to beginning of file before reading */

    fread(jsonContent, 1, fileSize, accounts_file);

    fclose(accounts_file); /* Close file after reading */

    /* Add NULL terminator to the jsonContent string : */
    jsonContent[fileSize] = '\0';

    /* Parse data into a json object : */
    json = cJSON_Parse(jsonContent);

    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        return NULL;
    }

    /* Validate JSON format */
    if (!validateAccountsJSON(json))
    {
        
        cJSON_Delete(json);
        free(jsonContent);
        return NULL;
    }

    free(jsonContent); /* Free the space used to store the JSON string */

    return json; /* Return the json object*/
}

/* Function to validate the format of Accounts JSON */
int validateAccountsJSON(cJSON *json)
{
    int i, numAccounts;
    int lineNumber;
    cJSON *currencyItem;
    const char *currency;
    

    /* Check if JSON is an array */
    if (!cJSON_IsArray(json))
    {
        printf("Error in Accounts.json : JSON is not an array.\n");
        return 0;
    }

    /* Get the number of elements in the array */
    numAccounts = cJSON_GetArraySize(json);

    lineNumber = 1; /* Starting [ */
    /* Each element in the json spans 7 lines (including {}) */

    /* Iterate over each element to validate its format */
    for (i = 0; i < numAccounts; i++)
    {
        
        cJSON *accountObject = cJSON_GetArrayItem(json, i);
        lineNumber++; /* Starting { */

        /* Check if all required fields are present and are in the correct format */

        if (!cJSON_HasObjectItem(accountObject, "account_id")){
                printf("\n");
                printf("Error in Accounts.json : Missing required account_id field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of account_id */
                if (!cJSON_IsNumber(cJSON_GetObjectItem(accountObject, "account_id")) ||
                    cJSON_GetObjectItem(accountObject, "account_id")->valuedouble < 10 ||
                    cJSON_GetObjectItem(accountObject, "account_id")->valuedouble >= 100)
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid account_id format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the account_id is of type int and is within the range of 1-90.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;
            if (!cJSON_HasObjectItem(accountObject, "user_id")){
                printf("\n");
                printf("Error in Accounts.json : Missing required user_id field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of user_id  */
                if (!cJSON_IsNumber(cJSON_GetObjectItem(accountObject, "user_id")) ||
                    cJSON_GetObjectItem(accountObject, "user_id")->valuedouble >= 50)
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid user_id format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the user_id is of type int and is within the range of 1-50.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;
            if (!cJSON_HasObjectItem(accountObject, "name")){
                printf("\n");
                printf("Error in Accounts.json : Missing required name field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of name */
                if (!cJSON_IsString(cJSON_GetObjectItem(accountObject, "name")))
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid name format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the name is a string.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;
            if (!cJSON_HasObjectItem(accountObject, "default_currency")){
                printf("\n");
                printf("Error in Accounts.json : Missing required default_currency field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of default_currency */
                currencyItem = cJSON_GetObjectItem(accountObject, "currency");
                if (!cJSON_IsString(currencyItem)) {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid currency format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the currency is a 3-letter string. \n");
                    printf("\n");
                    return 0;
                }

                currency = currencyItem->valuestring;
                if (strlen(currency) != 3) {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid currency format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the currency is a 3-letter string. \n");
                    printf("\n");
                    return 0;
                }

            }
            lineNumber++;
            if (!cJSON_HasObjectItem(accountObject, "balance")){
                printf("\n");
                printf("Error in Accounts.json : Missing required balance field at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                printf("\n");
                return 0;
            }
            else{
                /* Check format of balance */
                if (!cJSON_IsNumber(cJSON_GetObjectItem(accountObject, "balance")))
                {
                    printf("\n");
                    printf("Error in Accounts.json : Invalid balance format at entry number: %d and line number: %d.\n", i+1, lineNumber+1);
                    printf("Please ensure that the balance is a float.\n");
                    printf("\n");
                    return 0;
                }
            }
            lineNumber++;

        /* Check if there are extra fields */
        if (cJSON_GetArraySize(accountObject) > 5)
        {
            
            lineNumber += (cJSON_GetArraySize(accountObject) - 5);
            printf("\n");
            printf("Error in Accounts.json : Extra fields at entry number %d and line number %d.\n", i+1, lineNumber);
            printf("\n");
            return 0;
        }


        lineNumber++; /* Ending } */
        
    }
    lineNumber++; /* Ending ] */

    return 1; /* JSON format is valid */
}

Account *processAccountsData(cJSON *json, int *numAccounts)
{

    int i;

    Account *accountsArray;

    *numAccounts = cJSON_GetArraySize(json); /* *numAccounts will access the value at numAccounts pointer and store the size of the array there */

    accountsArray = (Account *)malloc((*numAccounts) * sizeof(Account)); /* Allocate space for the array of structs */

    for (i = 0; i < *numAccounts; i++)
    {

        cJSON *accountObject = cJSON_GetArrayItem(json, i); /* Retrieve the individual JSON object at index i */

        /* Populate the struct at index i with the attributes/values of each account : */

        accountsArray[i].account_id = cJSON_GetObjectItem(accountObject, "account_id")->valueint;
        accountsArray[i].user_id = cJSON_GetObjectItem(accountObject, "user_id")->valueint;
        strncpy(accountsArray[i].name, cJSON_GetObjectItem(accountObject, "name")->valuestring, sizeof(accountsArray[i].name));
        strncpy(accountsArray[i].default_currency, cJSON_GetObjectItem(accountObject, "default_currency")->valuestring, sizeof(accountsArray[i].default_currency));
        accountsArray[i].balance = cJSON_GetObjectItem(accountObject, "balance")->valuedouble;
    }

    return accountsArray;
}
