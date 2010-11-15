/* stack by array */

#include <stdio.h>
#define MAX 10

int s[MAX], top;

int stack_is_empty()
{
  if(top == 0)
    return(1);
  return(0);
}

void initstack()
{
  top = 0;
}

void push(int k)
{
  s[top] = k;
  top++;
}

int pop()
{
  int key;

  top--;
  key = s[top];
  return(key);
}

int main(int argc, char *argv[])
{
  int n, c;

  initstack();
  printf("push: 1, pop: 2\n");
  while(scanf("%d", &c) != EOF) {
    switch(c) {
    case 1:
      printf("Input key\n");
      scanf("%d", &n);
      push(n);
      break;
    case 2:
      if(stack_is_empty() == 1) {
        printf("Stack is empty!!\n");
        break;
      }
      printf("key: %d\n", pop());
      break;
    }
    printf("push: 1, pop: 2\n");
  }
}

