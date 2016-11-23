all:
	gcc -Wall -Wextra -g -o my_sort pthread_sort.c -pthread
