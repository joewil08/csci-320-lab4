#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4.h"


Page_Entry* page_table;
TLB_Table tlb;
Main_Memory main_mem;
FILE* backing_store_fp;

// Implement get_page_from() and get_offset_from() functions below this line
unsigned char get_page_from(int logical_address) {

}

unsigned char get_offset_from(int logical_address) {

}


// Implement the page table functions below this line
void init_page_table() {

}

void close_page_table() {

}

short page_table_lookup(unsigned char) {

}

void update_page_table(unsigned char page, unsigned char frame) {

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
