#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cs50.h>


int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: Input file name to recover/n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");

    // Check if file exists
    if (!file)
    {
        printf("File does not exist!/n");
        return 1;
    }

    // Allocate buffer (array of bytes) for memory file to read to
    unsigned char buffer[512];

    // Integer to count amount of JPEGs created
    int JPEGcount = 0;

    // Creating file to write new JPEG into
    FILE *img = NULL;

    // Allocate memory for newJPG, for ###.jpg/0 filename format
    char newJPG[8];

    // Check for end of file
    while (fread(buffer, 512, 1, file) == 1)
    {
        // Check first four bytes for JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
           // If first JPEG
           if (JPEGcount == 0)
           {
                // Create string for new JPEG, string is stored into newJPG
                sprintf(newJPG, "%03d.jpg", JPEGcount);
                JPEGcount++;
                // Open file, to write 512 bytes into it
                img = fopen(newJPG, "w");

                // Write the jpeg to file img, writing 512 bytes into it
                fwrite(buffer, 512, 1, img);
           }

            // If new JPEG
            else if (JPEGcount > 0)
            {
                // Close previously opened JPEG
                fclose(img);

                // Create string for new JPEG, string is stored into newJPG
                sprintf(newJPG, "%03d.jpg", JPEGcount);
                JPEGcount++;
                // Open file, to write 512 bytes into it
                img = fopen(newJPG, "w");

                // Write the jpeg to file img, writing 512 bytes into it
                fwrite(buffer, 512, 1, img);
            }
        }

        else if (JPEGcount > 0)
        {
            // Write the jpeg to file img, writing 512 bytes into it
            fwrite(buffer, 512, 1, img);
        }

    }

    // Close output file
    fclose(img);

    // Close input file
    fclose(file);

    // Success
    return 0;
}
