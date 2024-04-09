#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExpenseParser.h"

#define MAX_EXPENSES 100 // Maximum number of expenses to store

void parse_json(const char *json, Expenses expenses[], int *num_expenses) {
    const char *ptr = json;

    // Skip leading whitespace
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
        ptr++;
    }

    // Check if JSON starts with [
    if (*ptr != '[') {
        printf("Invalid JSON\n");
        return;
    }

    ptr++; // Skip [

    // Parse array elements
    while (*ptr != '\0' && *ptr != ']' && *num_expenses < MAX_EXPENSES) {
        // Parse expense object
        Expenses *expense = &expenses[*num_expenses];

        // Parse account_id
        while (*ptr != ':') {
            ptr++;
        }
        ptr++; // Skip :

        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ptr++;
        }

        // Parse numeric value
        expense->account_id = atoi(ptr);

        // Move ptr to next token
        while (*ptr != ',' && *ptr != ']') {
            ptr++;
        }

        // Parse date
        while (*ptr != ':') {
            ptr++;
        }
        ptr++; // Skip :

        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ptr++;
        }

        // Parse string value
        sscanf(ptr, "\"%[^\"]\"", expense->date);

        // Move ptr to next token
        while (*ptr != ',' && *ptr != ']') {
            ptr++;
        }

        // Parse description
        while (*ptr != ':') {
            ptr++;
        }
        ptr++; // Skip :

        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ptr++;
        }

        // Parse string value
        sscanf(ptr, "\"%[^\"]\"", expense->description);

        // Move ptr to next token
        while (*ptr != ',' && *ptr != ']') {
            ptr++;
        }

        // Parse currency
        while (*ptr != ':') {
            ptr++;
        }
        ptr++; // Skip :

        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ptr++;
        }

        // Parse string value
        sscanf(ptr, "\"%[^\"]\"", expense->currency);

        // Move ptr to next token
        while (*ptr != ',' && *ptr != ']') {
            ptr++;
        }

        // Parse amount_spent
        while (*ptr != ':') {
            ptr++;
        }
        ptr++; // Skip :

        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            ptr++;
        }

        // Parse numeric value
        sscanf(ptr, "%f", &expense->amount_spent);

        // Move ptr to next token
        while (*ptr != ',' && *ptr != ']') {
            ptr++;
        }

        // Increment the number of expenses parsed
        (*num_expenses)++;
    }
}

int main() {
    const char *filename = "expenses.json";
    FILE *expenses_file = fopen(filename, "r");
    if (expenses_file == NULL) {
        printf("Error: Unable to open %s\n", filename);
        return 1;
    }

    // Get the file size
    fseek(expenses_file, 0L, SEEK_END);
    long fileSize = ftell(expenses_file);
    rewind(expenses_file);

    // Allocate memory for JSON content
    char *jsonContent = (char *)malloc(fileSize + 1); // +1 for null terminator
    if (jsonContent == NULL) {
        printf("Error: Unable to allocate memory\n");
        fclose(expenses_file);
        return 1;
    }

    // Read the JSON content into the allocated memory
    size_t bytesRead = fread(jsonContent, 1, fileSize, expenses_file);
    if (bytesRead != fileSize) {
        printf("Error: Failed to read JSON content\n");
        free(jsonContent);
        fclose(expenses_file);
        return 1;
    }

    // Null-terminate the JSON content
    jsonContent[fileSize] = '\0';

    // Close the file
    fclose(expenses_file);

    // Array to store parsed expenses
    Expenses expenses[MAX_EXPENSES];
    int num_expenses = 0;

    // Parse the JSON content
    parse_json(jsonContent, expenses, &num_expenses);

    // Access the parsed expenses
    for (int i = 0; i < num_expenses; i++) {
        printf("Expense %d:\n", i+1);
        printf("Account ID: %d\n", expenses[i].account_id);
        printf("Date: %s\n", expenses[i].date);
        printf("Description: %s\n", expenses[i].description);
        printf("Currency: %s\n", expenses[i].currency);
        printf("Amount Spent: %.2f\n", expenses[i].amount_spent);
        printf("\n");
    }

    // Free the allocated memory
    free(jsonContent);

    return 0;
}
