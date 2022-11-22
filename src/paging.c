#include "paging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Your implementation goes in here!


void* pt_init() {
	//allocate memory for page table (number of page table rows) * (page table entry size)
	void *table = malloc(PAGETABLE_ROWS * TABLEROW_BITS);

	return table;
}

uint16_t virtual_to_physical(void* table, uint16_t virtual_address) {
	bool vaddress[16];

	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (virtual_address > 0){
			vaddress[15-i] = virtual_address % 2;
			virtual_address = virtual_address/2;
		}
        else{vaddress[15-i] = 0; }
	}

	uint16_t page_number = 0;
	// extract page_number from first 9 bits of virtual address
	for (int i = 0; i < 9; i++){
		page_number += (vaddress[i]) * pow(2, 8 - i);
	}

	//physical address binary form 
	bool paddress[16];
	//the row in page table container frame number 
	int *row = (int*)table + (page_number * 16);

	//extract frame number bits from table row and offset from virtual address
	for (int i = 0; i < 16; i++){
		if (i < 9) paddress[i] = *(row + i);
		if (i >= 9) paddress[i] = vaddress[i];
	}

	uint16_t physical_address = 0;
	//convert physical address bits into integer form
	for (int i = 0; i < 16; i++){
		physical_address += paddress[i] * pow(2, 15-i);
	}

	return physical_address;
}

void map_page_to_frame(void* table, uint16_t page_number, uint16_t frame_number, bool readonly, bool executable) {
	//the row in page table correspomding to given page_number
	int *row = (int*)table + (16*page_number);
	
	//convert frame_number into 9-bit binary and add bits to row in page table 
	for (int i = 0; i<=8; i++){
		if (frame_number > 0){
			*(row + (8-i)) = frame_number % 2;
			frame_number = frame_number/2;
		}
        else{
            *(row + (8-i)) = 0;
        }
	}

	//add protection bits to page table row 
	*(row+9) = readonly;
	*(row+10) = executable;
}

void print_table(void* table) {
}

void unmap_page(void* table, uint16_t page_number) {
	int *row = (int*)table + (page_number * 16);

	//set all bits in row to 0
	for (int i =0; i < 16; i++) {*(row + i) = 0;}
}

void store_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {
	uint16_t physical_address = virtual_to_physical(table, virtual_address);
	
	bool paddress[16];
	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (physical_address > 0){
			paddress[15-i] = physical_address % 2;
			physical_address = physical_address/2;
		}
        else{
			paddress[15-i] = 0; 
		}
	}

	uint16_t frame_number = 0;
	uint16_t offset = 0;

	//extract page_number & offset from virtual address 
	for (int i = 0; i <=15; i++){
		if (i < 9){
			frame_number += paddress[i] * pow(2,8-i);
		} 
		else {
			offset += paddress[i] * pow(2, 15-i);
		}
	}

	char* b = (char*)buffer;
	int* frame = (int*)table + (frame_number * 128) + offset; 

	//write the data to physical frame
	for (int i =0; i < length; i++){
		*(frame + i) = *(b + i);
	}
}

void read_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {
	uint16_t physical_address = virtual_to_physical(table, virtual_address);

	bool paddress[16];
	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (physical_address > 0){
			paddress[15-i] = physical_address % 2;
			physical_address = physical_address/2;
		}
        else{
			paddress[15-i] = 0; 
		}
	}

	uint16_t frame_number = 0;
	uint16_t offset = 0;

	//extract page_number & offset from virtual address 
	for (int i = 0; i <=15; i++){
		if (i < 9){
			frame_number += paddress[i] * pow(2,8-i);
		} 
		else {
			offset += paddress[i] * pow(2, 15-i);
		}
	}

	int* b = (int*)buffer;
	int* frame = (int*)table + (frame_number * 128) + offset; 

	for (int i = 0; i < length; i++){
		*(b + i) = *(frame + i);
	}
}


