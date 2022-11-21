#include "paging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Your implementation goes in here!


void* pt_init() {
	//allocate memory for page table (number of page table rows) * (page table entry size)
	void *table = malloc(PAGETABLE_ROWS * TABLEROW_BITS);

	return table;
}

uint16_t virtual_to_physical(void* table, uint16_t virtual_address) {


}

void map_page_to_frame(void* table, uint16_t page_number, uint16_t frame_number, bool readonly, bool executable) {
	bool address[9];

	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=8; i++){
		if (frame_number > 0){
			address[8-i] = frame_number % 2;
			frame_number = frame_number/2;
		}
        else{
            address[8-i] = 0;
        }
	}

	//the row corresponding to the given page number
	int *row = (int*)table + (16*page_number);


	for (int i = 0; i < 9; i++){
		*(row+i) = address[i];
	}

	*(row+9) = readonly;
	*(row+10) = executable;
}

void print_table(void* table) {
}

void unmap_page(void* table, uint16_t page_number) {
}

void store_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {

}

void read_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {
}

