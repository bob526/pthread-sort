#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MIN(x,y) (((x)<(y))?(x):(y))

typedef struct {
    int *source;
    int *temp;
    int left;
    int mid;
    int right;
} merge_struct;

void mergesort(int *source, int *temp, int number);
void real_merge(int *source, int *temp, int left, int mid, int right);
void* real_merge_thread(void *argument);
void swap(int *a, int *b);
void copy_arr(int *source, int *dest, int number);


int main(int argc, char *argv[])
{
    if (argc!=3) {
        printf("Input error. Please enter rand_seed(number) & data_size(number).\n");
        return -1;
    }
    int rand_seed, data_size, *my_array,*backup, i;
    rand_seed = atoi(argv[1]);
    data_size = atoi(argv[2]);
    srand(rand_seed);
    my_array = calloc(data_size,sizeof(int));
    backup = calloc(data_size,sizeof(int));
    for (i=0; i<data_size; i++)
        my_array[i] = rand()%20;
/*
    puts("Before sort:");
    for (i=0; i<data_size; i++)
        printf("%d ",my_array[i]);
    printf("\n");
*/
   

    mergesort(my_array,backup,data_size);



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

void mergesort(int *source, int *temp, int number)
{
    int width,i;
    merge_struct test;
    for (width=1; width<number; width*=2) {
        for (i=0; i<number; i+=2*width) {
            //merge_struct test;
            test.source = source;
            test.temp = temp;
            test.left = i;
            test.mid = MIN(i+width,number);
            test.right = MIN(i+2*width,number);
            real_merge_thread(&test);
            //real_merge(source,temp,i,MIN(i+width,number),MIN(i+2*width,number));
        }
        copy_arr(temp,source,number);
    }

    
}

void copy_arr(int *source, int *dest, int number)
{
    int i;
    for (i=0;i<number;i++) 
        dest[i] = source[i];
}

void real_merge(int *source, int *temp, int left, int mid, int right)
{
    int i,j,k;

    i=left, j=mid;
    //printf("%d %d %d\n", left, mid, right);
    for (k=left; k<right; k++) {
        if (i<mid && (j>=right || source[i]<=source[j])) {
            temp[k] = source[i];
            i++;
        }
        else {
            temp[k] = source[j];
            j++;
        }
    }
}

void* real_merge_thread(void *argument)
{
    merge_struct *a = (merge_struct*)argument;
    int i,j,k;

    i=a->left, j=a->mid;
    for (k=a->left; k<a->right; k++) {
        if (i<a->mid && (j>=a->right || a->source[i]<=a->source[j])) {
            a->temp[k] = a->source[i];
            i++;
        }
        else {
            a->temp[k] = a->source[j];
            j++;
        }
    }

    return NULL;
}