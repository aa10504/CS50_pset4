/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file 
    FILE *inptr = fopen(infile, "r"); //fopen - opens a file; FILE* fopen(string filename, string mode); "r" means reading; "w" means writing; "a" means appending; return a pointer
    if (inptr == NULL) //using fopen, you must check if the file you openned exist
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w"); //"w" function will create an empty file even if one of the same name already exists; Appending with a will append data to the end of an already present file, or create an empty file if filename doesn’t exist.
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf; //給BITMAPFILEHEADER這個data struct一個變數bf
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
//fread - read from a file; size_t fread(void* ptr, size_t size, size_t blocks, FILE* fp); read "size" bytes of "fp" file in blocks times, On each fread, it will store the bytes in a buffer pointer to by ptr.
//fread returns the total number of "chunks" successfully read in; this should correspond to the blocks parameter.

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi; //給BITMAPINFOHEADER這個data struct一個變數bi
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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++) //abs(); 絕對值
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            //chang the color of pixels
            if ((triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00 && triple.rgbtRed == 0xff) || (triple.rgbtBlue == 0xff && triple.rgbtGreen == 0xff && triple.rgbtRed == 0xff))
            {
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0xff;
            }
            else
            {
                triple.rgbtBlue = 0x00;
                triple.rgbtGreen = 0x00;
                triple.rgbtRed = 0xff;
            }
            
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR); 
//int fseek(FILE* fp, long int offset, int from_where);
//Use fseek when you want to change the offset of the file pointer fp. This is an extremely useful tool.
//Normally, when reading in from a file, the pointer continues in one direction, from the start of the file to the end. 
//fseek, however, allows you to change the location of the file pointer.

//offset refers to how many bytes you want to change the current position of the file pointer fp. You can go both forward with positive numbers, or backward with negative numbers.
//If you want to change the location of the pointer fp from its current location, set from_where to SEEK_CUR.
//To instead set the file pointer fp to a location in relation to the end of the file, set from_where to SEEK_END.
//Similarly, if you want to alter the pointer in relation to the beginning of the file, set from_where to SEEK_SET.

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
