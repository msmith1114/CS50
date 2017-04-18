#include <stdio.h>
#include <cs50.h>

int main(void)
{
  int height=0;
  do {
  printf("Height: "); 
  height = get_int();
 
  } while ((height < 0) || (height > 23));
  
  for(int i=0; i < height; i++){
    
    for(int a=0; a < height-i-1; a++){ //Print Spaces
      printf("%s", " ");
      }
      
    for(int b=0; b <= i; b++){ //Print #
      printf("#");
      }
      
    printf("  "); // Print Gap
      
    for(int c=0; c< i+1; c++){ //Print Right-side #
      printf("#");
      
      }
      printf("\n"); //Newline for after #
    
  }
  return 0;
}