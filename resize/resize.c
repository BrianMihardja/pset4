// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Arguments: resize(n) infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    int n = atoi(argv[1]);

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
    BITMAPFILEHEADER bf, bf_resize;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_resize = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_resize;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_resize = bi;

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
    // printf("bf.bfSize: %u\n", bf.bfSize);
    // printf("BITMAPFILEHEADER: %lu\n", sizeof(BITMAPFILEHEADER));
    // printf("BITMAPINFOHEADER: %lu\n", sizeof(BITMAPINFOHEADER));



    // printf("bf.bfSize: %u\n", bf.bfSize);


    // write outfile's BITMAPINFOHEADER
    printf("old bi.biWidth: %d\n", bi.biWidth);
    //change bi.biWidth by ratio of n
    bi_resize.biWidth = bi.biWidth * n;
    printf("new bi.biWidth: %d\n\n", bi_resize.biWidth);

    printf("old bi.biHeight: %d\n", bi.biHeight);
    //change bi.biHeight by ratio of n
    bi_resize.biHeight = bi.biHeight * n;
    bi_resize.biHeight = abs(bi_resize.biHeight);
    printf("new bi.biHeight: %d\n\n", bi_resize.biHeight);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_resize = (4 - (bi_resize.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi_resize.biSizeImage = (bi_resize.biWidth * sizeof(RGBTRIPLE) + padding_resize) * abs(bi_resize.biHeight);

    //change the bfSize
    bf_resize.bfSize = bf.bfSize + (bi_resize.biSizeImage - bi.biSizeImage);

    fwrite(&bf_resize, sizeof(BITMAPFILEHEADER), 1, outptr);

    fwrite(&bi_resize, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //to write the row a total of n times.
        for(int k = 0; k < n; k++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // printf("rgbtBlue: column %i row %i %x\n", j, i, triple.rgbtBlue);
                // printf("rgbtGreen: column %i row %i %x\n", j, i, triple.rgbtGreen);
                // printf("rgbtRed: column %i row %i %x\n", j, i, triple.rgbtRed);
                // printf("\n");

                // write RGB triple to outfile
                for (int counter = 0; counter < n; counter++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

                for(int paddingcounter = 0; paddingcounter < padding_resize; paddingcounter++)
                {
                   fputc(0x00, outptr);
                }

                if (k < n - 1)
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }




    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
