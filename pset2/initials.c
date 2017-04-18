#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
   string name = get_string();
   char temp;


   
   for(int i=0; i < strlen(name); i++){
       
    while(isspace(name[i]))
        {
            i++;
        }
    if(isalpha(name[i])){
        temp = toupper(name[i]);
        printf("%c",temp);
        while(isalpha(name[i])){
            i++;
        }
    }  
       
   }
   printf("\n");
   
}