#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "paging.h"

int main(int argc, char **argv){
    void* table = pt_init();

    char text1[]  = "It was the best of times, it was the worst of times";

    printf("%d\n", strlen(text1));

    char text2[] = "It was the best of times, it was the worst of times, it was the age of wisdom, it was the age of foolishness, it was the epoch of belief, it was the epoch of incredulity, it was the season of light, it was the season of darkness, it was the spring of hope, it was the winter of despair, we had everything before us, we had nothing before us, we were all going direct to heaven, we were all going direct the other way–in short, the period was so far like the present period, that some of its noisiest authorities insisted on its being received, for good or for evil, in the superlative degree of comparison only.";

    printf("%d\n", strlen(text2));
}