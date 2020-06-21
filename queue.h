#include <stdio.h>
#include <stdlib.h>

typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;

Queue * createQueue(int maxElements)
{
        /* Create a Queue */
        Queue *Q;
        Q = (Queue *)malloc(sizeof(Queue));
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        /* Return the pointer */
        return Q;
}
int Dequeue(Queue *Q)
{
        int temp;
        /* If Queue size is zero then it is empty. So we cannot pop */
        if(Q->size==0)
                return -1;
        /* Removing an element is equivalent to incrementing index of front by one */
        else
        {
                temp = Q->elements[Q->front];
                Q->size--;
                Q->front++;
                /* As we fill elements in circular fashion */
                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        printf("\nProcess %d is dequeued\n",temp);
        return temp;
}

void Enqueue(Queue *Q,int element)
{
        /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
        if(Q->size == Q->capacity)
        {
                printf("\nQueue is Full\n");
        }
        else
        {
                Q->size++;
                Q->rear = Q->rear + 1;
                /* As we fill the queue in circular fashion */
                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }
                /* Insert the element in its rear side */ 
                Q->elements[Q->rear] = element;
        }
        printf("\nProcess %d is enqueued\n",Q->elements[Q->rear]);
        return;
}
