CC=gcc
CFLAGS=-ansi -pedantic -Wall -Werror

SOURCES_output=FSMoutput.c HTML_output/Page2-UserBarGraph.c HTML_output/Page3-AccOverallExpenses.c HTML_output/Page4-AccYearlyExpenses.c ParserExpenses/ExpenseParser.c ParserAccounts/AccountsParser.c
OBJECTS_output=$(SOURCES_output:.c=.o)
EXECUTABLE_output=FSM_output

SOURCES_input=FSM_Input.c ParserAccounts/AccountsParser.c ParserExpenses/ExpenseParser.c
OBJECTS_input=$(SOURCES_input:.c=.o)
EXECUTABLE_input=FSM_input

all: $(EXECUTABLE_output) $(EXECUTABLE_input)

$(EXECUTABLE_output): $(OBJECTS_output)
	$(CC) $(CFLAGS) $(OBJECTS_output) -o $@

$(EXECUTABLE_input): $(OBJECTS_input)
	$(CC) $(CFLAGS) $(OBJECTS_input) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS_output) $(OBJECTS_input) $(EXECUTABLE_output) $(EXECUTABLE_input)


