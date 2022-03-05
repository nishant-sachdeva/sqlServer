#pragma once

#include "setup.h"
#include "row.h"
#include "table.h"

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
        int argsAssigned = sscanf(
                                inputBuffer->buffer,
                                "insert %d %s %s",
                                &(statement->rowToInsert.id),
                                statement->rowToInsert.userName,
                                statement->rowToInsert.email
                            );
    
        if (argsAssigned < 3)
            return PREPARE_SYNTAX_ERROR;
        return PREPARE_SUCCESS;
    } else if (strcmp(inputBuffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    } else {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
}


ExecuteResult execute_insert(Statement* statement, Table* table) {
    if (table->numRows >= TABLE_MAX_ROWS)
        return EXECUTE_TABLE_FULL;
    
    Row* rowToBeInserted = &(statement->rowToInsert);

    serialize_row(rowToBeInserted, row_slot(table, table->numRows));
    table->numRows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table) {
    Row row;
    for (uint32_t i = 0; i < table->numRows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table * table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        case STATEMENT_SELECT:
            return execute_select(statement, table);
    }
}
