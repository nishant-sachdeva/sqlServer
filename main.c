#include "src/input.h"
#include "src/setup.h"
#include "src/exec.h"
#include "src/table.h"

#include <stdio.h>

void print_prompt() { printf("db > "); }

int main(int argc, char **argv) {
    Table* table = new_table();
    InputBuffer* inputBuffer = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(inputBuffer);

        if (inputBuffer->buffer[0] == '.') {
            switch (do_meta_command(inputBuffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", inputBuffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(inputBuffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->buffer);
                continue;
        }
        
        switch (execute_statement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: Table full.\n");
                break;
        }
    }
    return 0;
}