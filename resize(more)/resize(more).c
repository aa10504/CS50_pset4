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
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    int r = 1 / f;
    
    if (f > 100 || f <= 0)
    {
        fprintf(stderr, "f must be (0.0, 100.0]\n");
        return 5;
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
    
    BITMAPFILEHEADER new_bf = bf;
    BITMAPINFOHEADER new_bi = bi;
    
    new_bi.biWidth = bi.biWidth * f;
    new_bi.biHeight = bi.biHeight * f;
    
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    new_bi.biSizeImage = (new_bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(new_bi.biHeight);
    new_bf.bfSize = new_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    if (f >= 1)
    {
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            for (int a = 0; a < f; a++)
            {
                // iterate over pixels in scanline
                for (int j = 0; j < bi.biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr); //從inptr裡，讀取1次sizeof(RGBTRIPLE)大小的資料，存到triple裡
                
                    //原圖同一列的每一個pixel重復畫n次到新的圖上
                    for (int b = 0; b < f; b++)
                    {
                        // write RGB triple to outfile
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); //從triple裡，讀取1次sizeof(RGBTRIPLE)大小的資料，存到outptr裡
                    }
                }

                // then add it back (to demonstrate how)
                for (int k = 0; k < new_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            
                //將原圖指標返回到此列的第一個
                fseek(inptr, -(bi.biWidth * 3), SEEK_CUR);
            }
        
            //將原圖指標換列
            fseek(inptr, bi.biWidth * sizeof(RGBTRIPLE) + padding, SEEK_CUR);
        }
    }
    else if (f < 1)
    {
        for (int i = 0, biHeight = abs(bi.biHeight) * f; i < biHeight; i++)
        {
            for (int j = 0, biWidth = bi.biWidth * f; j < biWidth; j++)
            {
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                fseek(inptr, (r - 1) * 3, SEEK_CUR);
            }
            
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }
            fseek(inptr, padding + (bi.biWidth * sizeof(RGBTRIPLE) + padding), SEEK_CUR);
        }
    }
    
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
