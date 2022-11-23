#include "paging.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	// Allocate 32 KiB of memory to serve as our store
	void *store = malloc(1<<15);

	// Initialise our mapping table
	void *table = pt_init();

	// Add some basic mappings to the table
	// map_page_to_frame(table, 0, 4, false, false);
	// map_page_to_frame(table, 1, 3, false, false);
	// map_page_to_frame(table, 2, 0, false, false);
	// map_page_to_frame(table, 3, 2, false, false);
	// map_page_to_frame(table, 4, 15, false, false);
	print_table(table);

	

	/**------------------------------- TEST 1 ------------------------------
	 * Now try to write something to our store. Our implementation should transparently use the
	 * page table to store the virtual pages into correct physical frames. This example should fit into a single page
	 */ 
	printf("\n");
	printf("--------------------------- TEST 1 ---------------------------\n");
	char text[]  = "It was the best of times, it was the worst of times";
	size_t length = strlen(text);
	char *buffer = malloc(length+1);
	store_data(table, store, text, 0, length);
	read_data(table, store, buffer, 0, length);
	printf("%s\n", buffer);

	free(buffer);

	/**------------------------------- TEST 2 -------------------------------
	 * Now try something longer and see what happens. The data read out should be exactly the same as what was put in
	 */
	 printf("\n");
	 printf("--------------------------- TEST 2 ---------------------------\n");
	char text2[] = "It was the best of times, it was the worst of times, it was the age of wisdom, it was the age of foolishness, it was the epoch of belief, it was the epoch of incredulity, it was the season of light, it was the season of darkness, it was the spring of hope, it was the winter of despair, we had everything before us, we had nothing before us, we were all going direct to heaven, we were all going direct the other way–in short, the period was so far like the present period, that some of its noisiest authorities insisted on its being received, for good or for evil, in the superlative degree of comparison only.";
	length = strlen(text2);
	buffer = malloc(length+1);

	store_data(table, store, text2, 0, length);
	read_data(table, store, buffer, 0, length);
	printf("%s\n", buffer);

	free(buffer);
	/**------------------------------- TEST 3 -------------------------------
	 * Offset Test : 
	 * Store data of length 94 in with offset 10
	 * Read data from offset 20
	 */
	 printf("\n");
	 printf("--------------------------- TEST 3 ---------------------------\n");
	char text3[] = "There is starter code on studres for simulating paging on the 16-bit architecture from Part 1.";
	length = strlen(text3);
	buffer = malloc(length+1-10);

	// Virtual address [0000001110001010] = 906. Page number 7 [000000111] and offset 10 [0001010]
	store_data(table, store, text3, 906, length);
	// Virtual address [0000001110010100] = 1022. Page number 7 [000000111] and offset 20 [0010100]
	read_data(table, store, buffer, 916, length - 10);
	printf("%s\n", buffer);
 
	free(buffer);
	
	/**------------------------------- TEST 4 -------------------------------
	 * Large Data Test 
	 */
	printf("\n");
	printf("--------------------------- TEST 4 ---------------------------\n");
	char text4[] = "Make sure you describe your implementation in the report and justify main design decisions. There is very little actual code needed, but you will need to think carefully about how to implement the page table from Part 1, and how to use pointers appropriately to make sure your implementation is correct. You should provide evidence of testing that demonstrates that your solution works for a range of non-trivial tasks.";
	length = strlen(text4);
	buffer = malloc(length+1);

	store_data(table, store, text4, 2432, length);
	read_data(table, store, buffer, 2432, length);
	printf("%s\n", buffer);
 
	free(buffer);
	
	// Cleanup
	free(store);
	free(table);
	
}
