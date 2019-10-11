


//TODO: WHOLE AND FULL CHECKING AND DEBUGGING !!!!


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include"cs50.h"
#include"bmp.h"

//TODO: bool isNewJpeg(char *yy)? (85% done)

bool isNewJPEG(char *chunk)
{
	//TODO: BITWISE AND (OR OR I CAN'T REMEMBER) (99% done , DEBUG)
	if (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: bool checking (char *yy)? if it is endoffile.( 99% done)

int checkingC(char *chunk)
{
	if ((sizeof(chunk) != 512) || (chunk != EOF))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main(int argc, char *argv[])
{
	//To check if the user wrote the file's name.
	if (argc != 2)
	{
		fprintf(stderr, "Usage: infile\n");
        return 1;
	}
	// assign the file to a pointer (string:<the file's name and type>).
	char *memoryCard = argv[1];
	// opening the file ..
	FILE *inCardFile = fopen(memoryCard, "r");
	/*
	Checking if the file is valid ;
	If pointer returned NULL then it can't be opened (or maybe have not been found)
	*/
	if (inCardFile == NULL)
	{
		fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
	}
	// Going to start with reading chunk , chunk to see if it is a new file or not.
	//Going to hold the bytes we will read into a chink to chack for the specifications
	//requested.
	char *chunk[512], *preChunk[512];
	//WARNING: check this (u didn't have internet ..)
	//(
	fread(&preChunk, (sizeof(char)*512), 1, inCardFile);
	//)
	int checking = 0;
	// ** number of JPEGS **
	int noJPEGs = 0;
	//Starting with recovery:
	while(checkingC(&preChunk) == 1)
	{
		//if the first chunk is 512 byte or it's not an empty file .. then we will
		//continue reading into "chunk" .
		fread(&chunk, (sizeof(char)*512), 1, inCardFile);
		if (isNewJPEG(&chunk))
		{
			//increasing the number of JPEGs to create a file's name with it's number.
			noJPEGs++;
			//creating new JPEG file, the name's depending on "noJPEGs".
			FILE *picFound = fopen(("%3i.jpg", noJPEGs), "w");
			while (!isNewJPEG(&chunk)){
				//While it's not a new JPEG , read the next chunk, check it if
				//it's new (again..) or it's the end of file, so you can close the file
				//and let it iterate again to take another file or finish its job.
				fread(&chunk, (sizeof(char)*512), 1, inCardFile);
				if ((isNewJPEG(&chunk)) || (checkingC(&chunk) == 0)) {
					//for the value in line (70).
					preChunk = chunk;
					//closing the file.
					fclose(picFound);
					break;
				}
				//if it's clean then now we can write it to the file "yaay :3".
				fwrite(&chunk, (sizeof(char) * 512), 1, picfound);
				
			}
			else
			{
				fread(&chunk, (sizeof(char)*512), 1, inCardFile);
				preChunk = chunk;
			}
			
		}
	}
	if (noJPEGS == 0)
	{
		fprintf(stderr, "photos not found.\n");
        return 3;
	}
	
	
	
	
}
