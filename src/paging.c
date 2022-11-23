#include "paging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//allocate memory for our simulated hard disk 
// void *disk = malloc(1<<20);
void *disk;

//Free Frame List 
int freeFrames[512] = {1};

/**
 * Retrieves the frame number of a free frame in physical memory 
 * @return : the frame number of a free memory frame or -1 if there are none
*/
int getFreeFrame(){
	//search for free frame
	for (int i = 0; i < 512; i++){
		if (freeFrames[i] != -1){
			freeFrames[i] = -1;
			return i;
		}
	}
	return -1;
}

void* pt_init() {
	//allocate memory for page table (number of page table rows) * (page table entry size)
	void *table = malloc(PAGETABLE_ROWS * TABLEROW_BITS);

	return table;
}

uint16_t virtual_to_physical(void* table, uint16_t virtual_address) {
	return -1;
}

/**
 * Given a page number, returns corresponding frame number from page table
 * If page i marked as invalid, it will find a free frame and map page number to the newly found frame in memory
 * @param (table) : page table
 * @param (page_number) : uint16_t representing page_number in page table
*/
uint16_t page_to_frame_num(void* table, uint16_t page_number){
	int* row = (int*)table + (page_number * 16);

	uint16_t frame_number = 0;

	if (*(row+15) != 1){
		frame_number = getFreeFrame();

		if (frame_number == 65535){
			return -1;
		}
		map_page_to_frame(table, page_number, frame_number, 1,1);//by default set protection bits to 1

		return frame_number;
	}

	for (int i = 0; i<9; i++){
		frame_number += (*(row + i)) * pow(2, 8-i); 
	}

	return frame_number;
}

/**
 * Given a page number, finds the corresponding frame in table. 
 * If page is marked as invalid in table, the function will look through disk for the requested frame 
 * If the frame is found in disk, it will be first written to memory, then the corresponding 
 * memory frame number will be returned.
 * 
*/
uint16_t readFrame(void* table, uint16_t page_number){
	int* row = (int*)table + (page_number * 16);


}

/**
 * Given an address, returns the page/frame number
 * @param (address) : the address to extract page/frame number from
 * @return : the page number as a uint16_t type
*/
uint16_t getPageNumber(uint16_t address){
	uint16_t page_number = 0;

	for (int i = 0; i<=15; i++){
		if (i >= 7){page_number += (address % 2) * pow(2 , (i+1)-8 );}
		address = address/2;
	}
	return page_number;
}


/**
 * Given an address, extracts and returns the offset
 * @param (address) : the address to extract offset from
 * @return : the offset as a uint16_t type
*/
uint16_t getOffset(uint16_t address){
	uint16_t offset = 0;

	for (int i = 0; i <= 15; i++){
		if (i<7){offset += (address%2) * pow(2, i);}
		address = address/2;
	}
	return offset;
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
	uint16_t frame_number = page_to_frame_num(table, page_number);
	uint16_t offset = getOffset(virtual_address);
	int num_pages = ceil((length + offset)/128.0);

	//if data being written to physical memory is larger than size of physical frame 
	if (length > 128 - offset){
		//allocated number of frames necessary to hold data
		for (int i = 1; i <= num_pages; i++){
			page_to_frame_num(table, page_number + i);
		}
	}

	char *b = buffer;
	char *frame = store; 

	int j = 0;//page index
	int i = 0;//index from buffer
	int k = offset; //index into each frame

	//iterate all pages 
	while (j < num_pages){
		while (k < 128 && i < length){
			frame[(frame_number * 128) + k] = b[i];
			k++;
			i++;
		}
		k = 0;
		j+=1;
		if (j == num_pages) break;
		frame_number = page_to_frame_num(table, page_number + j);
		frame = (char*)store + (128 * frame_number);
		
	}
}

void read_data(void* table, void* store, void* buffer, uint16_t virtual_address, size_t length) {
	uint16_t page_number = getPageNumber(virtual_address);
	uint16_t frame_number = page_to_frame_num(table, page_number);
	uint16_t offset = getOffset(virtual_address);

	int num_pages = ceil((length + offset)/128.0);
	
	char *b = buffer;
	char *frame = store;

	int j = 0;//page index
	int i = 0;//index from buffer
	int k = offset; //index into each frame

	//iterate all pages
	while (j < num_pages){
		while (k < 128 && i < length){
			// printf("%c", frame[(frame_number * 128) + k]);
			b[i] = frame[(frame_number * 128) + k];
			k++;
			i++;
		}
		k = 0;
		j++;
		if (j == num_pages) break;
		frame_number = page_to_frame_num(table, page_number + j);
		frame = (char*)store + (128 * frame_number);
	}
}


