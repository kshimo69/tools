/* queue by array */

#include <stdio.h>
#define MAX 10

int q[MAX], top, rear;

int queue_is_empty()
{
  if(rear == top)
    return(1);
  return(0);
}

void initqueue()
{
  top = rear = 0;
}

void enqueue(int k)
{
  q[rear] = k;
  rear++;
}

int dequeue()
{
  int key;

  key = q[top];
  top++;
  return(key);
}

int main(int argc, char *argv[])
{
  int n, c;

  initqueue();
  printf("enqueue: 1, dequeue: 2\n");
  while(scanf("%d", &c) != EOF) {
    switch(c) {
    case 1:
      printf("Input key\n");
      scanf("%d", &n);
      enqueue(n);
      break;
    case 2:
      if(queue_is_empty() == 1) {
        printf("Queue is empty!!\n");
        break;
      }
      printf("key: %d\n", dequeue());
      break;
    }
    printf("enqueue: 1, dequeue 2\n");
  }
}
