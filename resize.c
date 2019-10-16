// Copies a BMP file
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include"cs50.h"
#include"bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    if (!(n > 0 && n <= 100))
    {
        fprintf(stderr, "Usage: n should be positive and less than or equal to 100.\n");
        return 2;
    }
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inFilebf;
    fread(&inFilebf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inFilebi;
    fread(&inFilebi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inFilebf.bfType != 0x4d42 || inFilebf.bfOffBits != 54 || inFilebi.biSize != 40 ||
        inFilebi.biBitCount != 24 || inFilebi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    BITMAPFILEHEADER outFilebf = inFilebf;
    BITMAPINFOHEADER outFilebi = inFilebi;
    // write outfile's BITMAPFILEHEADER
    fwrite(&outFilebf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outFilebi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines before resizing
    int inFilePadding = (4 - (inFilebi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int inFileBiWidth = inFilebi.biWidth;
    int inFileBiHeight = abs(inFilebi.biHeight);

    inFilebi.biSizeImage = ((sizeof(RGBTRIPLE) * inFilebi.biWidth) + inFilePadding) * (inFilebi.biHeight);
    inFilebf.bfSize = inFilebi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    // ______________________AFTER RESIZIG_________________________

    outFilebi.biWidth = inFilebi.biWidth * n;
    outFilebi.biHeight = inFilebi.biHeight * n;

    int outFilePadding = (4 - (outFilebi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    outFilebi.biSizeImage = ((sizeof(RGBTRIPLE) * outFilebi.biWidth) + outFilePadding) * (outFilebi.biHeight);
    outFilebf.bfSize = outFilebi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	RGBTRIPLE array[outFilebi.biWidth];
	int count = 0;
    // iterate over infile's scanlines
    for (int i = 0; i < abs(inFilebi.biHeight); i++)
    {
       	// iterate over pixels in scanline
       	for (int j = 0; j < inFilebi.biWidth; j++)
       	{

            // temporary storage
       	   	RGBTRIPLE triple;
	        // read RGB triple from infile
       	   	fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
			for (int pixelC = 0; pixelC < n; pixelC++)
			{

	    		array[count++] = triple;
				fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

			}
        }
        // skip over padding, if any
        fseek(inptr, inFilePadding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < outFilePadding; k++)
        {
            fputc(0x00, outptr);
        }

        for (int lines = 1; lines < n; lines++)
        {
    	    fwrite(&array, (sizeof(RGBTRIPLE) * outFilebi.biWidth), 1, outptr);
        	// then add it back
        	for (int k = 0; k < outFilePadding; k++)
        	{
        	    fputc(0x00, outptr);
        	}
		}
        count = 0;

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
