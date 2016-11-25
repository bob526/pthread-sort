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

typedef struct {
    int *data;
    int element_num;
    int element_size;
} qsort_struct;

void mergesort(int *source, int *temp, int number);
void real_merge(int *source, int *temp, int left, int mid, int right);
void* real_merge_thread(void *argument);
void swap(int *a, int *b);
void copy_arr(int *source, int *dest, int number);
void disp_arr(int *data, int number);
int compare_func(const void *a, const void*b);
void* thread_qsort(void *argument);


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


    //mergesort(my_array,backup,data_size);
    //qsort(my_array,data_size,sizeof(int),compare_func);
    pthread_t mytest_thread_qsort[4];
    qsort_struct thread_qsort_passing[4];

    for (i=0;i<4;i++) {
        printf("i=%d\n",i);
        if (i==0) {
            thread_qsort_passing[i].data = my_array;
            thread_qsort_passing[i].element_num = data_size/4;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
        else if (i==3) {
            thread_qsort_passing[i].data = thread_qsort_passing[i-1].data+thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_num = data_size-3*thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
        else {
            thread_qsort_passing[i].data = thread_qsort_passing[i-1].data+thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_num = data_size/4;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
    }
    

    for (i=0;i<4;i++) {
        pthread_create(&mytest_thread_qsort[i],NULL,thread_qsort,(void*)&thread_qsort_passing[i]);
    }

    
    for (i=0;i<4 /*&& pthread_equal(origin,mytest_thread_qsort[i])*/;i++)
        pthread_join(mytest_thread_qsort[i],NULL);
    //pthread_join(another_thread,NULL);

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
    int width,i/*,thread_count=0,join_idx*/;
    //merge_struct test;
    //pthread_t thread_id[number], main_thread;
    //main_thread = pthread_self();
    for (width=1; width<number; width*=2) {
        //thread_count=0;
        for (i=0; i<number; i+=2*width) {
            /*test.source = source;
            test.temp = temp;
            test.left = i;
            test.mid = MIN(i+width,number);
            test.right = MIN(i+2*width,number);
            if(pthread_create(&thread_id[thread_count++],NULL,&real_merge_thread,&test)) {
                perror("Error in thread creation");
                exit(-1);
            }*/
            //real_merge_thread(&test);
            real_merge(source,temp,i,MIN(i+width,number),MIN(i+2*width,number));
        }
        //pthread_exit(NULL);
        //printf("Current thread: %lu\n",pthread_self());
       /* for (join_idx=0; join_idx<thread_count && pthread_equal(main_thread,pthread_self()); join_idx++) {

            if (pthread_equal(pthread_self(),thread_id[0])) {
                continue;
            }
            else {
                pthread_join(thread_id[0],NULL);
            }

            if (pthread_join(thread_id[join_idx],NULL)) {
                printf("join idx=%d\n",join_idx);
                perror("Pthread join error");
                exit(-1);
            }
            printf("Index = %d is OK.\n",join_idx);
        }*/
        /*printf("thread count=%d\n",thread_count);
        puts("Temp:");
        disp_arr(temp,number);
        puts("Source:");
        disp_arr(source,number);
        if (pthread_equal(main_thread,pthread_self()))*/
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
    printf("New thread: %lu\n",pthread_self());
    pthread_mutex_t mylock;
    pthread_mutex_lock(&mylock);
    merge_struct *a = (merge_struct*)argument;
    int i,j,k;

    i=a->left, j=a->mid;
    for (k=a->left; k<a->right; k++) {
        //printf("i=%d k=%d\n",i,k);
        if (i<a->mid && (j>=a->right || a->source[i]<=a->source[j])) {
            a->temp[k] = a->source[i];
            i++;
        }
        else {
            a->temp[k] = a->source[j];
            j++;
        }
    }
    pthread_mutex_unlock(&mylock);
    pthread_exit(NULL);
    return NULL;
}

void disp_arr(int *data, int number)
{
    int i;
    for (i=0;i<number;i++) {
        printf("%d ",data[i]);
    }
    printf("\n");
}

int compare_func(const void *a, const void *b)
{
    if (*(int*)a < *(int*)b)
        return -1;
    else if (*(int*)a > *(int*)b)
        return 1;
    else 
        return 0;
}

void* thread_qsort(void *argument)
{
    qsort_struct *a = (qsort_struct*)argument;
    qsort(a->data,a->element_num,a->element_size,compare_func);
    return NULL;
}