/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    if(n < 1 || n > 100)
    {
        fprintf(stderr, "Scale must be a value of 1 through 100\n");
    }
    
    

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Save old Width and old padding and scale them to n
    int oldWidth = bi.biWidth;
    int oldPadding =  (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biHeight *= n;
    bi.biWidth *= n;
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = (bi.biWidth * abs(bi.biHeight) * 3) + padding*abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + bf.bfOffBits;

    
    //Write new headers
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < oldWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile n times horizontally
            for(int a = 0; a < n; a++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }
        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
            fputc(0x00, outptr);

        int new_line = oldWidth*sizeof(RGBTRIPLE) + oldPadding;
        //
        if(((i+1) % n) != 0)
            fseek(inptr, -new_line, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
