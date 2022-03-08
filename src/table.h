#pragma once

#include "row.h"
#include "pager.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t numRows;
    Pager* pager;
} Table;

void* row_slot(Table* table, const uint32_t rowNum) {
    uint32_t pageNum = rowNum/ROWS_PER_PAGE;
    void* page = get_page(table->pager, pageNum);    
    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return (page + byteOffset);
}
