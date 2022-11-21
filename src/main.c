#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "paging.h"

int main(int argc, char **argv){
    // void* table = pt_init();

    int frame_number = 5;

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

    for (int i = 0; i< 9; i++){
        printf("%d ", address[i]);
    }
}