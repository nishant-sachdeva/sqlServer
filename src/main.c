#include "input.h"
#include "utils/setup_utils.h"
#include "utils/exec_utils.h"

#include <stdio.h>

void print_prompt() { printf("db > "); }

int main(int argc, char **argv) {
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
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->buffer);
                continue;
        }
        
        execute_statement(&statement);
        printf("Command executed.\n");
    }

    return 0;
}