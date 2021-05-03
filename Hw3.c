/*Author: Utku POLAT 160302001*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				12					/*Element size of List*/
#define NUMBER_OF_THREADS	7					/*Number of threads*/

void *sorter(void *params);						/* thread that performs basic sorting algorithm */
void *merger(void *params);						/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8,13,1};
int result[SIZE];
int position=0;									/* (IN *merger Function)position being inserted into result list */

typedef struct 									//type definition
{
	int from_index;
	int to_index;
	int size;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	printf("Before Sorting: ");
	for(i=0 ; i < SIZE; i++){
		printf("%d ",list[i]);
	}

	/* establish the first sorting thread */
	parameters *data1 = (parameters *) malloc (sizeof(parameters));
	data1->from_index = 0;
	data1->to_index = (SIZE/4) - 1;
	data1->size = (data1->to_index - data1->from_index)+1;
	pthread_create(&workers[0], 0, sorter, data1);

	/* establish the second sorting thread */
	parameters *data2 = (parameters *) malloc (sizeof(parameters));
	data2 = (parameters *) malloc (sizeof(parameters));
	data2->from_index = (SIZE/4);
	data2->to_index = (SIZE/2) - 1;
	data2->size = (data2->to_index - data2->from_index)+1;
	pthread_create(&workers[1], 0, sorter, data2);

	/* establish the third sorting thread */
	parameters *data3 = (parameters *) malloc (sizeof(parameters));
	data3 = (parameters *) malloc (sizeof(parameters));
	data3->from_index = (SIZE/2);
	data3->to_index = (3*(SIZE/4)) - 1;
	data3->size = (data3->to_index - data3->from_index)+1;
	pthread_create(&workers[2], 0, sorter, data3);

	/* establish the fourth sorting thread */
	parameters *data4 = (parameters *) malloc (sizeof(parameters));
	data4 = (parameters *) malloc (sizeof(parameters));
	data4->from_index = (3*(SIZE/4));
	data4->to_index = SIZE-1;
	data4->size = (data4->to_index - data4->from_index)+1;
	pthread_create(&workers[3], 0, sorter, data4);

	/* now wait for the 4 sorting threads to finish */
	for (i = 0; i < 4; i++)
		pthread_join(workers[i], NULL);
		
	/* first thread list */
	printf("\nAfter 1. Sorting Thread Work: ");
	for(i = 0 ; i < SIZE/4 ;i++){
		printf("%d  ",list[i]);
	}
	
	/* second thread list */
	printf("\nAfter 2. Sorting Thread Work: ");	
	for(i = SIZE/4 ; i < (SIZE/2) ; i++){
		printf("%d  ",list[i]);
	}
	
	/* third thread list */
	printf("\nAfter 3. Sorting Thread Work: ");
	for(i = SIZE/2 ; i < (SIZE/4)*3 ;i++){
		printf("%d  ",list[i]);
	}
	
	/* fourth thread list */
	printf("\nAfter 4. Sorting Thread Work: ");	
	for(i = (SIZE/4)*3 ; i < SIZE ;i++){
		printf("%d  ",list[i]);
	}

	/* establish the first merge thread */
	parameters *data5 = (parameters *) malloc (sizeof(parameters));
	data5 = (parameters *) malloc(sizeof(parameters));
	data5->from_index = 0;
	data5->to_index = (SIZE/4);
	data5->size = 2 * (data5->to_index - data5->from_index);
	pthread_create(&workers[4], 0, merger, data5);
	pthread_join(workers[4], NULL);
	
	printf("\nAfter First Merging Thread Work: ");	
	for(i = 0 ; i < SIZE/2 ;i++){
		printf("%d  ",result[i]);
	}
		
	/* establish the second merge thread */
	parameters *data6 = (parameters *) malloc (sizeof(parameters));
	data6 = (parameters *) malloc(sizeof(parameters));
	data6->from_index = (SIZE/2);
	data6->to_index = ((SIZE/4)*3);
	data6->size = 4 * (data6->to_index - data6->from_index);
	pthread_create(&workers[5], 0, merger, data6);
	pthread_join(workers[5], NULL);
	
	printf("\nAfter Second Merging Thread Work: ");	
	for(i = SIZE/2; i < SIZE ;i++){
		printf("%d  ",result[i]);
	}
	for(i = 0; i < SIZE ;i++){	/****Because of the third merger function I equalize the result array to list array****/
		list[i]=result[i];
	}
	
	/* establish the third merge thread */
	parameters *data7 = (parameters *) malloc (sizeof(parameters));
	data7 = (parameters *) malloc(sizeof(parameters));
	data7->from_index = 0;
	data7->to_index = SIZE/2;
	data7->size = 2 * (data7->to_index - data7->from_index);
	pthread_create(&workers[6], 0, merger, data7);
	pthread_join(workers[6], NULL);
	
	printf("\nAfter Third Merging Thread Work: ");	
	for(i = 0 ; i < 12 ;i++){
		printf("%d  ",result[i]);
	}
	
    return 0;
}

/*Sorting thread.*/
void *sorter(void *params)
{
	int i;
	parameters* p = (parameters *)params;
	
	int begin = p->from_index;
	int end = p->to_index;
	
	int swapped = 1;
	int j = 0;
	int temp;
	
	while (swapped == 1) {
		swapped = 0;
		j++;
		
		for (i = begin; i <= end - j; i++) {
			if (list[i] > list[i+1]) {
				temp = list[i];
				list[i] = list[i+1];
				list[i+1] = temp;
				swapped = 1;
			}
		}
	}
	
	pthread_exit(0);
}

/* Merge thread*/
void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	int i,j,size;
	
	i = p->from_index; 
	j = p->to_index;   
	size = p->size;
	
	while ((i < p->to_index) && (j < size)) {   	
		if (list[i] <= list[j]) {			
			result[position++] = list[i];
			i++;
		}
		else {
			result[position++] = list[j];
			j++;
		}
	}
	
	/* copy the remainder */
	if (i < p->to_index) {
		while (i < p->to_index) {
			
			result[position] = list[i];
			position++;
			i++;
		}
	}
	else {
		while (j < size) {
			result[position] = list[j];
			position++;
			j++;
		}
	}
	if(position == 12){				//*** Because of the after second merger position must be equal to '0' ***//
		position = 0;
	}

	pthread_exit(0);
}

