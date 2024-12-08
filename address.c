#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4.h"


Page_Entry* page_table;
TLB_Table tlb;
Main_Memory main_mem;
FILE* backing_store_fp;
int iteration = 1;

// Implement get_page_from() and get_offset_from() functions below this line
unsigned char get_page_from(int logical_address) {
    return (logical_address >> 8) & 0x00FF;
}

unsigned char get_offset_from(int logical_address) {
    return logical_address & 0x00FF;
}


// Implement the page table functions below this line
void init_page_table() {
    page_table = malloc((TLB_LEN * TLB_LEN) * sizeof(Page_Entry));
    for (int i = 0; i < TLB_LEN * TLB_LEN; i++) {
        page_table[i].is_valid = 0;
    }
    
}

void close_page_table() {
    free(page_table);
}

short page_table_lookup(unsigned char page) {
    short result = tlb_lookup(page);
    if (iteration == 1) {
        increment_total_page_request();
        if (result != 1) {
            increment_tlb_hit();
            return result;
        }
    }
    if (page_table[page].is_valid == 1) {
        result = page_table[page].frame;
        update_tlb(page, result);
        return result;
    }
    else {
        increment_page_fault_count();
        unsigned char buffer[256];
        long address = page * 256;
        fseek(backing_store_fp, address, SEEK_SET);
        fread(buffer, sizeof(unsigned char), 256, backing_store_fp);
        int next = main_mem.next_available_frame;
        memcpy(main_mem.mem + next * 256, buffer, 256);
        update_page_table(page, next);
        update_tlb(page, next);
        main_mem.next_available_frame = (next + 1) % 256;
        return next;
    }

}

void update_page_table(unsigned char page, unsigned char frame) {
    page_table[page].frame = frame;
    page_table[page].is_valid = 1;
}


// Implement the TLB functions below this line
void init_tlb() {

}

short tlb_lookup(unsigned char page) {

}

void update_tlb(unsigned char page, unsigned char frame) {

}

void close_tlb() {

}


// Implement the Physical Memory functions below this line
void init_physical_memory() {

}

void free_physical_memory() {

}

signed char get_value(int logical_address) {

}


// Implement the Backing Store functions below this line
void init_backing_store(char* filename) {

}

int roll_in(unsigned char page, unsigned char frame) {

}

void close_backing_store() {
    
}
