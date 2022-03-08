#pragma once

#include "table.h"
#include "pager.h"

Table* db_open(const char* fileName) {
    Pager* pager = pager_open(fileName);
    uint32_t numRows = pager->fileLength / ROW_SIZE;

    Table* table = (Table*)malloc(sizeof(Table));
    table->pager = pager;
    table->numRows = numRows;
    
    return table;
}


void db_close(Table* table) {
    Pager* pager = table->pager;
    uint32_t numFullPages = table->numRows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < numFullPages; i++) {
        if (!pager->pages[i])
            continue;
        
        pager_flush(pager, i, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }
    // There may be a partial page to write to at the end of the file
    // This should not be needed once we switch to B-Trees

    uint32_t numAdditionalRows = table->numRows % ROWS_PER_PAGE;
    if (numAdditionalRows > 0) {
        uint32_t pageNum = numFullPages;
        if (pager->pages[pageNum] != NULL) {
            pager_flush(pager, pageNum, numAdditionalRows*ROW_SIZE);
            free(pager->pages[pageNum]);
            pager->pages[pageNum] = NULL;
        }
    }
    int result = close(pager->fileDescriptor);
    if (result == -1) {
        printf("Error closing file\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        if (pager->pages[i] != NULL) {
            free(pager->pages[i]);
            pager->pages[i] = NULL;
        }
    }
    free(pager);
    free(table);
}