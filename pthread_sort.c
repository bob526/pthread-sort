#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *data;
    int left;
    int right;
} sort_arg;


void swap(int *a, int *b);

int main(int argc, char *argv[])
{
    if (argc!=3) {
        printf("Input error. Please enter rand_seed(number) & data_size(number).\n");
        return -1;
    }
    int rand_seed, data_size, *my_array, i;
    rand_seed = atoi(argv[1]);
    data_size = atoi(argv[2]);
    srand(rand_seed);
    my_array = calloc(data_size,sizeof(int));
    for (i=0; i<data_size; i++)
        my_array[i] = rand()%20;
/*
    puts("Before sort:");
    for (i=0; i<data_size; i++)
        printf("%d ",my_array[i]);
    printf("\n");
*/




    puts("After sort:");
    for (i=0; i<data_size; i++)
        printf("%d ",my_array[i]);
    printf("\n");



    free(my_array);
    return 0;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
