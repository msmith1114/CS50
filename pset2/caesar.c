#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
   if(argc != 2){
       printf("Usage: ./caesar k");
       return 1;
   }
   else {
   int k = atoi(argv[1]);
   string plain;
   printf("%s","plaintext: ");
   plain = get_string();
   printf("%s","ciphertext: ");

   for (int i = 0, n = strlen(plain); i < n; i++)
    {

        if islower(plain[i])
        printf("%c", (((plain[i] + k) - 97) % 26) + 97);
        else if isupper(plain[i])
        printf("%c", (((plain[i] + k) - 65) % 26) + 65);
        else
        printf("%c", plain[i]);
    
   }
   printf("\n");
  }
   

   
}