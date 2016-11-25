#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MIN(x,y) (((x)<(y))?(x):(y))
#define THREAD_LIMIT 4

//Global var
int sum_diff;
pthread_mutex_t mutex_sum_diff;

typedef struct {
    int *data;
    int element_num;
    int element_size;
} qsort_struct;

typedef struct {
    int *data;
    int start;
    int end;
    //"Not" include data[end]
} all_sub_struct;

void disp_arr(int *data, int number);
int compare_func(const void *a, const void*b);
void* thread_qsort(void *argument);
void all_sub(int *data, int start, int end);
                                    //"not" include data[end]
void* thread_all_sub(void *argument);


int main(int argc, char *argv[])
{
    if (argc!=3) {
        printf("Input error. Please enter rand_seed(number) & data_size(number).\n");
        return -1;
    }
    int rand_seed, data_size, *my_array, i;
    rand_seed = atoi(argv[1]);
    data_size = atoi(argv[2]);
    sum_diff=0;
    pthread_mutex_init(&mutex_sum_diff,NULL);
    srand(rand_seed);
    my_array = calloc(data_size,sizeof(int));
    for (i=0; i<data_size; i++)
        my_array[i] = rand();


    //qsort(my_array,data_size,sizeof(int),compare_func);
    pthread_t thread_for_qsort[THREAD_LIMIT];
    qsort_struct thread_qsort_passing[THREAD_LIMIT];

    for (i=0;i<THREAD_LIMIT;i++) {
        if (i==0) {
            thread_qsort_passing[i].data = my_array;
            thread_qsort_passing[i].element_num = data_size/THREAD_LIMIT;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
        else if (i==THREAD_LIMIT-1) {
            thread_qsort_passing[i].data = thread_qsort_passing[i-1].data+thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_num = data_size-(THREAD_LIMIT-1)*thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
        else {
            thread_qsort_passing[i].data = thread_qsort_passing[i-1].data+thread_qsort_passing[i-1].element_num;
            thread_qsort_passing[i].element_num = data_size/THREAD_LIMIT;
            thread_qsort_passing[i].element_size = sizeof(int);
        }
    }


    for (i=0;i<THREAD_LIMIT;i++) {
        pthread_create(&thread_for_qsort[i],NULL,thread_qsort,(void*)&thread_qsort_passing[i]);
    }


    for (i=0;i<THREAD_LIMIT ;i++)
        pthread_join(thread_for_qsort[i],NULL);

    qsort(my_array,data_size,sizeof(int),compare_func);

    //puts("After sort:");
    //disp_arr(my_array,data_size);

    //all_sub(my_array,0,data_size);
    all_sub_struct all_sub_passing[THREAD_LIMIT];
    pthread_t thread_id_all_sub[THREAD_LIMIT];
    for (i=0;i<THREAD_LIMIT&&data_size>=8;i++) {
        if (i==0) {
            all_sub_passing[i].data = my_array;
            all_sub_passing[i].start = 0;
            all_sub_passing[i].end = data_size/THREAD_LIMIT;
        }
        else if (i==THREAD_LIMIT-1) {
            all_sub_passing[i].data = my_array;
            all_sub_passing[i].start = all_sub_passing[i-1].end;
            all_sub_passing[i].end = data_size;
        }
        else {
            all_sub_passing[i].data = my_array;
            all_sub_passing[i].start = all_sub_passing[i-1].end;
            all_sub_passing[i].end = all_sub_passing[i-1].end+data_size/THREAD_LIMIT;
        }
    }


    for (i=0;i<THREAD_LIMIT;i++)
        pthread_create(&thread_id_all_sub[i],NULL,thread_all_sub,(void*)&all_sub_passing[i]);

    for (i=0;i<THREAD_LIMIT;i++)
        pthread_join(thread_id_all_sub[i],NULL);

    //thread_all_sub((void*)&all_sub_passing);

    printf("%d\n",sum_diff);

    pthread_mutex_destroy(&mutex_sum_diff);
    free(my_array);
    return 0;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}



void copy_arr(int *source, int *dest, int number)
{
    int i;
    for (i=0;i<number;i++)
        dest[i] = source[i];
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

void all_sub(int *data, int start, int end)
{
    int i,temp_sum=0;
    for (i=start+1; i<end; i++) {
        temp_sum+=data[i]-data[i-1];
    }
    //pthread_mutex_lock(&mutex_sum_diff);
    sum_diff+=temp_sum;
    //pthread_mutex_unlock(&mutex_sum_diff);
}

void* thread_all_sub(void *argument)
{
    all_sub_struct *a = (all_sub_struct*)argument;
    int i,temp_sum=0;
    for (i=(a->start)+1; i<a->end; i++) {
        //printf("i=%d\n",i);
        if (i!=0 && i==((a->start)+1)) {
            temp_sum+=a->data[i-1]-a->data[i-2];
        }
        temp_sum+=a->data[i]-a->data[i-1];
        //printf("tempsum=%d\n",temp_sum);
    }
    //printf("Out =%d\n",temp_sum);
    pthread_mutex_lock(&mutex_sum_diff);
    sum_diff+=temp_sum;
    pthread_mutex_unlock(&mutex_sum_diff);
    return NULL;
}
