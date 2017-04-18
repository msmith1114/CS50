/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover 'filename' \n");
        return 1;
    }
    
    char *infile = argv[1];
    // open input file 
    // set output file
    FILE *inptr = fopen(infile, "r");
    FILE *outptr = NULL;
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    BYTE buffer[512];
    char filename[10];
    int counter = 0;

    while (!feof(inptr))
    {
       
        
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            if(outptr != NULL)
            {
                fclose(outptr);
            }
            sprintf( filename, "%03d.jpg", counter );
            outptr = fopen(filename, "w");
            fwrite(buffer, sizeof(buffer), 1, outptr);
            counter++;
            
        }
        else if(counter > 0)
        {
            
            fwrite(buffer, sizeof(buffer), 1, outptr);
        }
        
         fread(buffer, sizeof(buffer), 1, inptr);
        
    }
    fclose(inptr);
    fclose(outptr);

    

    // success
    return 0;
}
