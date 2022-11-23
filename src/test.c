#include "paging.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	// Allocate 32 KiB of memory to serve as our store
	void *store = malloc(1<<15);

	// Initialise our mapping table
	void *table = pt_init();

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

	// Virtual Address [0000100110000000] : page number [000010011] = 19, offset [0000000]= 0
	store_data(table, store, text4, 2432, length);
	read_data(table, store, buffer, 2432, length);
	printf("%s\n", buffer);
 
	free(buffer);

	/**------------------------------- TEST 5 -------------------------------
	 * Accessing previously stored data (from test 2) to check whether paging implementation correctly allocated physical frames 
	 */
	printf("\n");
	printf("--------------------------- TEST 5 ---------------------------\n");
	buffer = malloc(strlen(text2) + 1);
	read_data(table, store, buffer, 0, strlen(text2));
	printf("%s\n", buffer);

	free(buffer);


	
	/**------------------------------- TEST 6 -------------------------------
	 * Storing a large multi-frame piece of data in page 0. This is to test that my implementation can allocate physical frames non-contiguously to a contiguous set of virtual pages 
	 */
	printf("\n");
	printf("--------------------------- TEST 6 ---------------------------\n");
	//unmap data stored in test 3 which occupied frame 5. 
	unmap_page(table, 906);

	//currently frames 0,1,2,3,4,6,7,8,9,10 are occupied
	//so writing text6, which will require 4 frames, should be allocated to frame 5,11,12,13 (hence being allocated in non-contiguous frames)
	char text6[] = "A submission which completes Parts 1-4 to an excellent standard. It should show evidence of extensive background reading and understanding of strengths and weaknesses of paging on ARM. Part 4 should demonstrate good understanding of advanced paging concept. All work and the report must be excellent for this grade band, which includes well-tested and documented code with an insightful write-up demonstrating deep understanding of challenges and trade-offs.";
	length = strlen(text6);
	buffer = malloc(length+1);

	store_data(table, store, text6, 906, length);
	read_data(table, store, buffer, 906, length);
	printf("%s\n", buffer);

	free(buffer);

	/**------------------------------- TEST 7 -------------------------------
	 * Test to check that other frames were not overwritten by test 6
	 */
	printf("\n");
	printf("--------------------------- TEST 7 ---------------------------\n");
	//check that data written in test 7 did not get overwritten
	length = strlen(text4);
	buffer = malloc(length+1);
	read_data(table, store, buffer, 2432, length);
	printf("%s\n", buffer);

	free(buffer);
	
	

	// Cleanup
	free(store);
	free(table);

	
	
}
