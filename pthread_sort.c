#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *data;
    int left;
    int right;
} sort_arg;

void* myquicksort(void *pass_para);
int partition(sort_arg *pass_para, int pivot_old_idx);
//Return pivot new idx
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

    sort_arg start_sort;
    start_sort.data = my_array;
    start_sort.left = 0;
    start_sort.right = data_size-1; //Or data_size???

    myquicksort(&start_sort);

/*
    puts("After sort:");
    for (i=0; i<data_size; i++)
        printf("%d ",my_array[i]);
    printf("\n");
*/


    free(my_array);
    return 0;
}

void* myquicksort(void *input_para)
{
    sort_arg *pass_para = (sort_arg*)input_para;
    pthread_t some_test,some_test2;
    printf("pthread id=%lu\n", pthread_self());
    int pivot_new_idx;
    sort_arg leftside=*pass_para, rightside=*pass_para;
    if (pass_para->right > pass_para->left) {
        //pivot idx always is "left"
        pivot_new_idx = partition(pass_para,pass_para->left);

        leftside.right = pivot_new_idx-1;
        rightside.left = pivot_new_idx+1;
        if(pthread_create(&some_test, NULL, &myquicksort, &leftside)) {
            perror("First pthread error");
            exit(-1);
        }
        if(pthread_create(&some_test2, NULL, &myquicksort, &rightside)) {
            perror("Second pthread error");
            exit(-1);
        }
        //myquicksort(&leftside);
        //myquicksort(&rightside);
    }
}

int partition(sort_arg *pass_para, int pivot_old_idx)
{
    int pivot_value, store_idx, i, current_left, current_right;
    current_left = (*pass_para).left;
    current_right = (*pass_para).right;
    pivot_value = (*pass_para).data[pivot_old_idx];

    swap(&pass_para->data[pivot_old_idx], &pass_para->data[current_right]);
    store_idx = current_left;
    for (i= current_left; i<=current_right-1; i++) {
        if ((*pass_para).data[i] <= pivot_value) {
            //printf("here i =%d current_right=%d\n",i,current_right);
            swap(&pass_para->data[store_idx], &pass_para->data[i]);
            store_idx++;
        }
    }
    swap(&pass_para->data[current_right], &pass_para->data[store_idx]);
    return store_idx;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
