#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "paging.h"

int main(int argc, char **argv){
    // void* table = pt_init();

    // map_page_to_frame(table, 0, 4, false, false);
	// map_page_to_frame(table, 1, 3, false, false);
	// map_page_to_frame(table, 2, 0, false, false);
	// map_page_to_frame(table, 3, 2, false, false);
	// map_page_to_frame(table, 4, 1, false, false);



    //-------------------------------------------
    //store_data(table, table, table, 4369, 10);

    // printf("%d\n", 1<<15);
    // char x[] = "HELLOWORLD";
    // void *arr = malloc(100);
    // void *list = malloc(100);
    // list = x;

    // int* row = (int*)arr;
    // int* b = (int*)list;

    // for (int i = 0; i<10; i++){
    //     *(row+i) = *(b+i);
    // }
    // printf("%s\n", row);

    // for (int i = 10; i<20; i++){
    //     *(row+i) = 1;
    // }

    // for (int i = 0; i < 10; i++){
    //     printf("%c", *(row+i));
    // }

    // for (int i = 10; i<20; i++){
    //     printf("%d", *(row+i));
    // }

    int x[10] ={0};

    for (int i = 0; i < 10; i++){
        printf("%d", x[i]);
    }

}