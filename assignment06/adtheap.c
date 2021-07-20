#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	void **heapArr;
	int	last;
	int	capacity;
	int (*compare) (void *arg1, void *arg2);
} HEAP;

HEAP *heapCreate( int capacity, int (*compare) (void *arg1, void *arg2));
void heapDestroy( HEAP *heap);
int heapInsert( HEAP *heap, void *dataPtr);
static void _reheapUp( HEAP *heap, int index);
int heapDelete( HEAP *heap, void **dataOutPtr);
static void _reheapDown( HEAP *heap, int index);
int compare(void *arg1, void *arg2);
void heapPrint( HEAP *heap);

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate( int capacity, int (*compare) (void *arg1, void *arg2))
{ 	
	HEAP *heap = (HEAP*)malloc(sizeof(HEAP));
	if(!heap)
		return NULL;

	heap -> last = -1;
	heap -> capacity = MAX_ELEM;
	heap -> compare = compare;	
	
	heap -> heapArr = (void**)malloc(sizeof(void**)*(heap->capacity));
	if(!(heap->heapArr)) 
		printf("MEMORY WARNING : HEAP ARRAY");
	
	return heap;
}
	
/* Free memory for heap
*/
void heapDestroy( HEAP *heap)
{
	int i;
	if(!heap) return;
	if(heap -> heapArr) 
	{
		for(i = 0; i <= heap -> last ; i++)
		{
			if(heap -> heapArr[i]) free(heap->heapArr[i]);
		}
		free(heap -> heapArr);
	}	
	free(heap);
}		

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert( HEAP *heap, void *dataPtr)
{
	if( heap -> last == MAX_ELEM - 1)
		return 0;
	
	++(heap -> last);
	--(heap -> capacity);
	heap->heapArr[heap -> last] = dataPtr;
	
	if(heap->last > 0)	
		_reheapUp(heap, heap->last);

	return 1;
}	
	
/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index)
{
	int parent;
	void *hold;
	int cmpParent; 
	
	if(index)
	{
		parent = (index - 1)/2;
		cmpParent = heap->compare(heap->heapArr[index], heap->heapArr[parent]);
		
		if(cmpParent > 0)
		{
			hold = heap->heapArr[parent];
			heap->heapArr[parent] = heap->heapArr[index];
			heap->heapArr[index] = hold;
			_reheapUp(heap, parent);
		}
	}
	return;
}	
/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete( HEAP *heap, void **dataOutPtr)
{
	if(heap->last== -1)
		return 0;
	
	*dataOutPtr = heap -> heapArr[0];
	heap -> heapArr[0] = heap -> heapArr[heap->last];
	
	--(heap->last);
	++(heap->capacity);
	_reheapDown(heap, 0);
	return 1;
}	

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index)
{
	void *hold;
	void *leftData;
	void *rightData;
	
	int largeLoc;
	int leftLoc = index * 2 + 1;
	int rightLoc = index * 2 + 2;
	int cmpLar;
	
	if(leftLoc <= heap->last)
	{
		leftData = heap->heapArr[leftLoc];
		if(rightLoc <= heap->last)
		{
			rightData = heap->heapArr[rightLoc];
		}
		else rightData = NULL;
		
		if(!rightData) largeLoc = leftLoc;
		else 
		{
			if(heap->compare(leftData, rightData)> 0)
			{
				largeLoc = leftLoc;
			}
			else largeLoc = rightLoc;
		}
		
		cmpLar = heap->compare(heap->heapArr[index], heap->heapArr[largeLoc]);
		if(cmpLar<0)
		{
			hold = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[largeLoc];
			heap->heapArr[largeLoc] = hold;
			_reheapDown(heap, largeLoc);
		}
	}
	return;
}

/* user-defined compare function */
int compare(void *arg1, void *arg2)
{
	int *a1 = (int *)arg1;
	int *a2 = (int *)arg2;
	
	return *a1 - *a2;
}

/* Print heap array */
void heapPrint( HEAP *heap)
{
	int i;
	int last = heap->last;
	
	for( i = 0; i <= last; i++)
	{
		int *value = (int *)(heap->heapArr[i]);
		printf("%6d", *value);
	}
	printf( "\n");
}

int main(void)
{
	HEAP *heap;
	int data;
	int *dataPtr;
	int i;
	
	heap = heapCreate(MAX_ELEM, compare);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		int *newdata = (int *)malloc( sizeof(int));
		*newdata = data;
		
		// insert function call
		heapInsert( heap, newdata);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, (void **)&dataPtr);

		printf( "Deleting %d: ", *(int *)dataPtr);

		free(dataPtr);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}

