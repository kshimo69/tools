/* make list */

#include <stdio.h>
#include <stdlib.h>

struct vertex {
  int name, key;
  struct vertex *next;
} *top;

int find_key(int k)
{
  struct vertex *w;

  w = top;
  while(w != NULL) {
    if(k == w->key)
      return(1);
    w = w->next;
  }
  return(0);
}

void delete_key(int k)
{
  struct vertex *w, *prev, *tmp;

  if(k == top->key) {
    tmp = top;
    top = top->next;
    free(tmp);
  } else {
    prev = top;
    w = prev->next;
    while(w != NULL) {
      if(k == w->key) {
        prev->next = w->next;
        free(w);
      }
      prev = w;
      w = w->next;
    }
  }
}

int main()
{
  struct vertex *new;
  int n, k, ans;

  printf("\n");
  printf("#### input data to list ####\n");
  printf("\n");

  printf("Please input\n");
  top = NULL;
  while(1){
    printf(" input value for name: ");
    scanf("%d", &n);
    if(n < 0) break;
    printf(" input value for key: ");
    scanf("%d", &k);
    if(find_key(k) == 1) {
      printf("    key: %d is already inputed\n", k);
      printf("    Do you want to delete data(key: %d)?\n", k);
      printf("                      YES: 1 / No: 0 == ");
      scanf("%d", &ans);
      if(ans == 1) delete_key(k);
      else printf(" input value for name again\n");
    } else {
      new = (struct vertex *)malloc(sizeof(struct vertex));
      new->name = n;
      new->key  = k;
      new->next = top;
      top = new;
    }
  }

  printf("\n");
  printf("-----------------------------------\n");
  printf("print list [name, key]\n");
  printf("print list from near by HEAD\n");
  while(top != NULL){
    printf("[%d, %d] ", top->name, top->key);
    top = top->next;
  }
  printf("\n");
}
