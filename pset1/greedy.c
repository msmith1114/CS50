#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
 float amount=0.0;
 int cents=0;
 int quarters=0;
 int dimes=0;
 int nickels=0;
 int leftover=0;
 do{
   printf("O hai! How much change is owed? ");
   amount = get_float();
 } while(amount <= 0 || amount == 0);
 cents = (int)round(amount*100.0);
 
 //check quarters
 quarters = cents / 25;
 leftover = cents % 25;
 
 //check dimes
 dimes = leftover / 10;
 leftover = leftover % 10;
 
 //check nickels
 nickels = leftover / 5;
 leftover = leftover % 5;

 
 printf("%d\n",quarters+dimes+nickels+leftover);
 
 




  return 0;
}