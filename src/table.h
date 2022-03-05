#pragma once

#include "row.h"

#include <stdint.h>
#include <stdlib.h>

#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    uint32_t numRows;
    void* pages[TABLE_MAX_PAGES];
} Table;

void* row_slot(Table* table, uint32_t rowNum) {
    uint32_t pageNum = rowNum/ROWS_PER_PAGE;
    void* page = table->pages[pageNum];
    
    // allocate memory only when we try to access page
    if (!page)
        page = table->pages[pageNum] = malloc(PAGE_SIZE);
    
    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return (page + byteOffset);
}

Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->numRows = 0;
    for (uint32_t i = 0; i<TABLE_MAX_PAGES; i++)
        table->pages[i] = NULL;
    return table;
}

void free_table(Table *table) {
    for (int i = 0; table->pages[i]; i++)
        free(table->pages[i]);
    free(table);
}