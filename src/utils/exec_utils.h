#pragma once

#include "setup_utils.h"

#include <stdbool.h>
#include <string.h>

MetaCommandResult do_meta_command(InputBuffer *inputBuffer) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
        printf("Exiting...\n");
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    } else if (strcmp(inputBuffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    } else {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
}

void execute_statement(Statement *statement) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            printf("This is an insert statement.\n");
            break;
        case STATEMENT_SELECT:
            printf("This is a select statement.\n");
            break;
    }
    return;
}
