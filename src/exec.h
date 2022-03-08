#pragma once

#include "setup.h"
#include "row.h"
#include "table.h"
#include "database.h"

#include <stdbool.h>
#include <string.h>

MetaCommandResult do_meta_command(InputBuffer *inputBuffer, Table *table) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
        db_close(table);
        printf("Exiting...\n");
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(InputBuffer* inputBuffer, Statement* statement) {
    statement->type = STATEMENT_INSERT;

    char* keyword = strtok(inputBuffer->buffer, " ");
    char* idString = strtok(NULL, " ");
    char* userName = strtok(NULL, " ");
    char* email = strtok(NULL, " ");

    if (idString == NULL || userName == NULL || email == NULL)
        return PREPARE_SYNTAX_ERROR;
    
    int id = atoi(idString);
    if (id<0)
        return PREPARE_NEGATIVE_ID;
    
    if (strlen(userName) > COLUMN_USERNAME_SIZE)
        return PREPARE_STRING_TOO_LONG;
    
    if (strlen(email) > COLUMN_EMAIL_SIZE)
        return PREPARE_STRING_TOO_LONG;
    
    statement->rowToInsert.id = id;
    strcpy(statement->rowToInsert.userName, userName);
    strcpy(statement->rowToInsert.email, email);

    return PREPARE_SUCCESS;
}


PrepareResult prepare_statement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0)
            return prepare_insert(inputBuffer, statement);
    else if (strcmp(inputBuffer->buffer, "select") == 0) {
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
