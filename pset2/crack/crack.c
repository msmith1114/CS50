#define _XOPEN_SOURCE
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int crackCheck(char *word, char *cipher);
int main(int argc, string argv[])
{
   if (argc != 2){
    printf("Usage: ./crack hash");
    return 1;
   }
    
   char *cipher = argv[1]; //cipher 
   int trials = 0;
    
   //Open File
   FILE *fp;
   char buff[255];
   fp = fopen("/usr/share/dict/words", "r"); //The working version that cracks all 10 uses a 1.1gb Dictionary file which I did not include. This will crack about 2-3 I believe.
   
   if(fp == NULL)
   {
       printf("Failed to Open File...");
       
   }
   
   bool success = false;
   while (fgets(buff, sizeof(buff), fp) != NULL) 
    {  
        trials++;
        buff[strlen(buff) - 1] = '\0';
        success = crackCheck(buff,cipher);
        if(success)
        {
        printf("Word was:%s\n", buff);
        }
        
    } 
    printf("Trials: %i", trials);
    return 0;
}

int crackCheck(char *word, char *cipher)
{

    char *result;
    char salt[3];
    strncpy(salt, cipher, 2);
    salt[2] = '\0';
        result = crypt(word,salt);
        if(strcmp(result, cipher) == 0)
            {
            for(int a = 0;a < strlen(result); a++){
                printf("%c",result[a]);
            }
            return 1;
            }
        else
        {
            return 0;
        }
    return 0;
}
