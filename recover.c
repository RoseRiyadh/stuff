
#include <stdio.h>
#include <cs50.h>

bool isNewJPEG(unsigned char chunk[512])
{
    if (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: infile\n");
        return 1;
    }
    // assign the file to a pointer (string:<the file's name and type>).
    char *memoryCard = argv[1];
    FILE *inCardFile = fopen(memoryCard, "r");
    //Checking if the file is valid ;
    //If pointer returned NULL then it can't be opened (or maybe have not been found)
    if (inCardFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", memoryCard);
        return 2;
    }
    // Going to start with reading chunk , chunk to see if it is a new file or not.
    //Going to hold the bytes we will read into a chink to chack for the specifications requested..
    unsigned char chunk[512];
    // Putting this pointer to determine if the chunks that have been written are for the same JPEG or new one.
    FILE *pointer = NULL ;
    int count = 0;
    //if this value didn't change then there's no JPEGs or there's an error..
    int noJPEGS = 0;
    //Starting with recovery:
    while (fread(&chunk, sizeof(chunk), 1, inCardFile))
    {
        //if the first chunk is 512 byte or it's not an empty file .. then we will
        //continue reading into "chunk" .
        if (isNewJPEG(chunk))
        {
            // That means it found a new Jpeg .. so we need to close the last Jpeg if there's any..
            if (pointer != NULL)
            {
                fclose(pointer);
                return 0;
            }
            char jpegName[9];
            //To change the name everytime a new JPEG is found..
            sprintf(jpegName, "%03i.jpeg", count++);
            pointer = fopen(jpegName, "w");
            noJPEGS++;
            fwrite(&chunk, sizeof(chunk), 1, pointer);
        }
        // If the pointer doesn't equal to null then there's more chunk to read
        else if (pointer != NULL)
        {
            fwrite(&chunk, sizeof(chunk), 1, pointer);
        }
    }
    if (noJPEGS == 0)
    {
        fprintf(stderr, "photos not found.\n");
        return 3;
    }
    // Then it must have found at least 1 jpeg to recover so IT'S A WIN !!!!
    else
    {
        return 0;
    }

}
