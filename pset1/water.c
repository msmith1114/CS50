#include <stdio.h>
#include <cs50.h>

int main(void)
{
  int minutes=0;
  int bottles=0;
  
  printf( "Minutes: ");
  minutes = get_int();
  bottles = (((1.5*minutes)*(128))/16);
  printf( "Bottles: ");
  printf("%d\n",bottles);
 
}