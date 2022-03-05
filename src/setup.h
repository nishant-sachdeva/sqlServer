#pragma once

#include "row.h"

#include <stdint.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;


typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL   
} ExecuteResult;


typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row rowToInsert; // only to be used by insert statement
} Statement;
