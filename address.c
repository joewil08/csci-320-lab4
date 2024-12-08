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
    tlb.entry = (TLB_Entry*)malloc(TLB_LEN * sizeof(TLB_Entry));
    tlb.head = 0;
    tlb.tail = 0;
    tlb.length = 0;
    tlb.is_full = 0;
    for (int i = 0; i < TLB_LEN; i++) {
        tlb.entry[i].is_valid = 0;
        tlb.entry[i].page = 0;
        tlb.entry[i].frame = 0;
    }
}

short tlb_lookup(unsigned char page) {
    for (int i = 0; i < TLB_LEN; i++) {
        TLB_Entry current = tlb.entry[i];
        if (current.is_valid && current.page == page) {
            return current.frame;
        }
    }
    return -1;
}

void update_tlb(unsigned char page, unsigned char frame) {
    TLB_Entry new = {page, frame, 1};
    if (tlb.is_full) {
        tlb.head = (tlb.head + 1) % TLB_LEN;
    }
    tlb.entry[tlb.tail] = new;
    tlb.tail = (tlb.tail + 1) % TLB_LEN;
    if (tlb.tail == tlb.head) {
        tlb.is_full = 1;
    } else {
        tlb.is_full =  0;
    }
}

void close_tlb() {
    free(tlb.entry);
    tlb.head = 0;
    tlb.tail = 0;
    tlb.length = 0;
    tlb.is_full = 0;
}


// Implement the Physical Memory functions below this line
void init_physical_memory() {
    main_mem.mem = (char*)malloc(256*256*sizeof(char));
    main_mem.next_available_frame = 0;
}

void free_physical_memory() {
    free(main_mem.mem);
}

signed char get_value(int logical_address) {
    unsigned char pg = get_page_from(logical_address);
    unsigned char offset = get_offset_from(logical_address);
    iteration = 1;
    int frame = page_table_lookup(pg);
    iteration = 0;
    return main_mem.mem[frame * 256 + offset];
}


// Implement the Backing Store functions below this line
void init_backing_store(char* filename) {
    backing_store_fp = fopen(filename, "r");
}

int roll_in(unsigned char page, unsigned char frame) {
    return -1;
}

void close_backing_store() {
    fclose(backing_store_fp);
}
