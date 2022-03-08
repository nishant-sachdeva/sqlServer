#pragma once

#include "table.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>


#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    int fileDescriptor;
    uint32_t fileLength;
    void* pages[TABLE_MAX_PAGES];
} Pager;

Pager* pager_open(const char* fileName) {
    int fd = open(fileName,
                    O_RDWR | // Read/Write Mode 
                    O_CREAT, // Create File if it does not exists
                    S_IWUSR | // User Write Permission
                    S_IRUSR // User Read Permission
            );
    if (fd == -1){
        printf("Error: Could not open file %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    off_t fileLength = lseek(fd, 0, SEEK_END);

    Pager* pager = (Pager*)malloc(sizeof(Pager));
    pager->fileDescriptor = fd;
    pager->fileLength = (uint32_t)fileLength;

    for (uint32_t i  = 0; i < TABLE_MAX_PAGES; i++)
        pager->pages[i] = NULL;
    
    return pager;
}

void* get_page(Pager* pager, uint32_t pageNum) {
    if (pageNum > TABLE_MAX_PAGES) {
        printf("Error: Page number %d is out of bounds\n", pageNum);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[pageNum] == NULL) {
        // this is a cache miss. Allocate memory and load from file.
        void* page = malloc(PAGE_SIZE);
        uint32_t numPages = pager->fileLength / PAGE_SIZE;

        // we might have a partial page at the end of the file
        if (pager->fileLength % PAGE_SIZE)
            numPages++;
        
        if (pageNum <= numPages) {
            lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);
            ssize_t bytesRead = read(pager->fileDescriptor, page, PAGE_SIZE);
            if (bytesRead == -1) {
                printf("Error: Could not read from file\n");
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[pageNum] = page;
    }
    return pager->pages[pageNum];
}

void pager_flush(Pager* pager, uint32_t pageNum, uint32_t size) {
    if (pager->pages[pageNum] == NULL) {
        printf("Error: Cannot flush null page\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(pager->fileDescriptor, pager->pages[pageNum], size);
    
    if (bytesWritten == -1) {
        printf("Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}