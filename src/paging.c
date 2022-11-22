#include "paging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Your implementation goes in here!

//Free Frame List 
int frame_status[512] = {1};
int freeFrames[512] = {-1};
int freeFrameIndex = 0;

/**
 * Retrieves the frame number of a free frame in physical memory 
 * @return : the frame number of a free memory frame or -1 if there are none
*/
int getFreeFrame(){
	if (freeFrameIndex >= 512){return -1;}
	else{
		int free_frame = freeFrames[freeFrameIndex];
		freeFrames[freeFrameIndex] = -1;//remove free frame from free frame list 
		freeFrameIndex += 1;//increase index
		return free_frame;
	}
}

void* pt_init() {
	//initialize free frame list
	for (int i =0; i < 512;i++){
		freeFrames[i] = i;
	}


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

	//if valid bit of row in table is 0 (invalid), then we allocate row (pure demand paging)
	if (*(row + 15) == 0){
		int frame_number = getFreeFrame();

		if (frame_number == -1){
			return -1;
		}
		map_page_to_frame(table, page_number, frame_number, 1,1);//by default set protection bits to 1
	}

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

/**
 * Given a page number, returns corresponding frame number from page table
 * @param (table) : page table
 * @param (page_number) : uint16_t representing page_number in page table
*/
uint16_t page_to_frame_num(void* table, uint16_t page_number){
	int* row = (int*)table + (page_number * 16);

	uint16_t frame_number = 0;

	for (int i = 0; i<9; i++){
		frame_number += (*(row + i)) * pow(2, 8-i); 
	}

	return frame_number;
}

/**
 * Given virtual address, returns the page number
 * @param (virtual_address) : the virtual address to extract page number from
 * @return : the page number as a uint16_t type
*/
uint16_t getPageNumber(uint16_t virtual_address){
	uint16_t page_number = 0;

	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (i >= 7){page_number += (virtual_address % 2) * pow(2 , (i+1)-8 );}
		virtual_address = virtual_address/2;
	}

	return page_number;
}

/**
 * Given physical address, returns the frame number
 * @param (physical_address) : the physical address to extract frame number from
 * @return : the frame number as a uint16_t type
*/
uint16_t getFrameNumber(uint16_t physical_address){
	uint16_t frame_number = 0;

	//convert frame_number into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (i >= 7){frame_number += (physical_address % 2) * pow(2 , (i+1)-8 );}
		physical_address = physical_address/2;
	}

	return frame_number;
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
	*(row+13) = readonly;
	*(row+14) = executable;
	*(row+15) = 1;//set the valid bit 
}

void print_table(void* table) {
}

void unmap_page(void* table, uint16_t page_number) {
	int *row = (int*)table + (page_number * 16);

	//set all bits in row to 0
	for (int i =0; i < 16; i++) {*(row + i) = 0;}
}

void store_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {
	uint16_t page_number = getPageNumber(virtual_address);
	uint16_t physical_address = virtual_to_physical(table, virtual_address);

	if (physical_address == 65535){
		printf("MEMORY OVERFLOW - NO MORE FREE FRAMES!");
		return;
	}
	
	bool paddress[16];
	//convert physical into 16-bit binary 
	for (int i = 0; i<=15; i++){
		if (physical_address > 0){
			paddress[15-i] = physical_address % 2;
			physical_address = physical_address/2;
		}
        else{paddress[15-i] = 0; }
	}

	uint16_t frame_number = 0;
	uint16_t offset = 0;

	//extract page_number & offset from physical address 
	for (int i = 0; i <=15; i++){
		if (i < 9){frame_number += paddress[i] * pow(2,8-i);} 
		else {offset += paddress[i] * pow(2, 15-i);}
	}

	//if data being written to physical memory is larger than size of physical frame 
	if (length > 128 - offset){
		//allocated number of frames necessary to hold data
		for (int i = 1; i<=ceil(length/(128.0 - offset)); i++){
			//get a free frame 
			int free_frame = getFreeFrame();
			//if there are no free frames left
			if (free_frame == -1){
				printf("MEMORY OVERFLOW - NORE MORE FREE FRAMES!");
				return;
			} 
			map_page_to_frame(table, page_number + i, free_frame, 1, 1);
		}
	}

	char* b = (char*)buffer;
	int* frame = (int*)store + (frame_number * 128) + offset; 

	int i = 0;
	//write into first frame in physical memory
	for (i = 0; i < 128-offset;i++){
		*(frame + i) = *(b + i);
	}

	//write remaining data to physical memory
	for (int j = 1; j <= ceil(length/128.0); j++){
		//extract next physical frame to continue writing data to 
		frame_number = page_to_frame_num(table, page_number + i);
		frame = (int*)store + (frame_number * 128);
		for (i = i; i < length; i++){
			*(frame + i) = *(b + i);
		}
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


	char* b = (char*)buffer;
	int* frame = (int*)table + (frame_number * 128) + offset; 
	//Read data from physical memory into buffer
	for (int i = 0; i < length; i++){
		*(b + i) = *(frame + i);
	}
}


