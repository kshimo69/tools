/* Greatest Common Divisor (Euclid's algorithm) */

#include <stdio.h>

int main(int argc, char *argv[])
{
  int x, y, z, gcd();

  printf("Please input x: ");
  scanf("%d", &x);
  printf("Please input y: ");
  scanf("%d", &y);

  z = gcd(x, y);

  printf("gcd(%d, %d) = %d\n", x, y, z);

  return 0;
}

int gcd(int a, int b)
{
  int c;
  if (b == 0) return(a);
  c = gcd(b, a%b);
  return(c);
}
