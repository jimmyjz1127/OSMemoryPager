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
    char *buffer = malloc(20);

    for (int i =0; i<20; i++){
        strcpy(buffer + i,"a");
    }

    printf("%s", buffer);

    // char text[] = "HELLOWORLD";

    // void* buffer = text;

    // char *s = buffer;

    // char x = s[0];
    // char y = s[1];


    // printf("%c", y);
    // printf("%c", x);
}
