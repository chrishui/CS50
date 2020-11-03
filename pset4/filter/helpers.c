#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Integer for average of colors
    int rgbtAVG;

    //Creating loop to convert to grayscale, by pixel in height/width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgbtAVG = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);

            //Apply filter
            image[i][j].rgbtBlue = rgbtAVG;
            image[i][j].rgbtGreen = rgbtAVG;
            image[i][j].rgbtRed = rgbtAVG;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //Integer for Sepia colors
    float sepiaRed;
    float sepiaGreen;
    float sepiaBlue;

    //Creating loop to convert to Sepia, by pixel in height/width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaRed = .393 * (float)image[i][j].rgbtRed + .769 * (float)image[i][j].rgbtGreen + .189 * (float)image[i][j].rgbtBlue;
            sepiaGreen = .349 * (float)image[i][j].rgbtRed + .686 * (float)image[i][j].rgbtGreen + .168 * (float)image[i][j].rgbtBlue;
            sepiaBlue = .272 * (float)image[i][j].rgbtRed + .534 * (float)image[i][j].rgbtGreen + .131 * (float)image[i][j].rgbtBlue;

            // Adjust for values over 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // Apply filter
            image[i][j].rgbtBlue = round(sepiaBlue);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtRed = round(sepiaRed);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Creating temp storage
    typedef struct
    {
        BYTE tempBlue;
        BYTE tempGreen;
        BYTE tempRed;
    }
    TEMP;

    TEMP img[height][width];

    //converting image to temp storage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img[i][j].tempRed = image[i][width - 1 - j].rgbtRed;
            img[i][j].tempGreen = image[i][width - 1 - j].rgbtGreen;
            img[i][j].tempBlue = image[i][width - 1- j].rgbtBlue;
        }
    }

    //saving temp storage to new file
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = img[i][j].tempRed;
            image[i][j].rgbtGreen = img[i][j].tempGreen;
            image[i][j].rgbtBlue = img[i][j].tempBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Average of colors storage
    typedef struct
    {
        float tempBlue;
        float tempGreen;
        float tempRed;
    }
    blurTEMP;

    blurTEMP img[height][width];

    //conversion to temp storage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Top Bit, excluding corners
            if (i == 0 && j > 0 && j < width)
            {
                img[i][j].tempBlue = roundf(((float)image[i][j-1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue + (float)image[i+1][j-1].rgbtBlue + (float)image[i+1][j].rgbtBlue + (float)image[i+1][j+1].rgbtBlue) / 6);
                img[i][j].tempGreen = roundf(((float)image[i][j-1].rgbtGreen + image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen + (float)image[i+1][j-1].rgbtGreen + (float)image[i+1][j].rgbtGreen + (float)image[i+1][j+1].rgbtGreen) / 6);
                img[i][j].tempRed = roundf(((float)image[i][j-1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed + (float)image[i+1][j-1].rgbtRed + (float)image[i+1][j].rgbtRed + (float)image[i+1][j+1].rgbtRed) / 6);
            }

            //Bottom bit, excluding corners
            if (i == height - 1 && j > 0 && j < width)
            {
                img[i][j].tempBlue = roundf(((float)image[i-1][j-1].rgbtBlue + image[i-1][j].rgbtBlue + (float)image[i-1][j+1].rgbtBlue + (float)image[i][j-1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue) / 6);
                img[i][j].tempGreen = roundf(((float)image[i-1][j-1].rgbtGreen + (float)image[i-1][j].rgbtGreen + (float)image[i-1][j+1].rgbtGreen + (float)image[i][j-1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen) / 6);
                img[i][j].tempRed = roundf(((float)image[i-1][j-1].rgbtRed + (float)image[i-1][j].rgbtRed + (float)image[i-1][j+1].rgbtRed + (float)image[i][j-1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed) / 6);
            }

            //Left bit, excluding corners
            if (i > 0 && i < height - 1 && j == 0)
            {
                img[i][j].tempBlue = round(( (float)image[i-1][j].rgbtBlue + (float)image[i-1][j+1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue + (float)image[i+1][j].rgbtBlue + (float)image[i+1][j+1].rgbtBlue) / 6);
                img[i][j].tempGreen = round(((float)image[i-1][j].rgbtGreen + (float)image[i-1][j+1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen + (float)image[i+1][j].rgbtGreen + (float)image[i+1][j+1].rgbtGreen) / 6);
                img[i][j].tempRed = round(((float)image[i-1][j].rgbtRed + (float)image[i-1][j+1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed + (float)image[i+1][j].rgbtRed + (float)image[i+1][j+1].rgbtRed) / 6);
            }

            //Right bit, excluding corners
            if (i > 0 && i < height - 1 && j == width - 1)
            {
                img[i][j].tempBlue = roundf(((float)image[i-1][j].rgbtBlue + (float)image[i-1][j-1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j-1].rgbtBlue + (float)image[i+1][j].rgbtBlue + (float)image[i+1][j-1].rgbtBlue) / 6);
                img[i][j].tempGreen = roundf(((float)image[i-1][j].rgbtGreen + (float)image[i-1][j-1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i][j-1].rgbtGreen + (float)image[i+1][j].rgbtGreen + (float)image[i+1][j-1].rgbtGreen) / 6);
                img[i][j].tempRed = roundf(((float)image[i-1][j].rgbtRed + (float)image[i-1][j-1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j-1].rgbtRed + (float)image[i+1][j].rgbtRed + (float)image[i+1][j-1].rgbtRed) / 6);
            }

            //Top left corner
            if (i == 0 && j == 0)
            {
                img[i][j].tempBlue = roundf(((float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue + (float)image[i+1][j].rgbtBlue + (float)image[i+1][j+1].rgbtBlue) / 4);
                img[i][j].tempGreen = roundf(((float)image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen + (float)image[i+1][j].rgbtGreen + (float)image[i+1][j+1].rgbtGreen) / 4);
                img[i][j].tempRed = roundf(((float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed + (float)image[i+1][j].rgbtRed + (float)image[i+1][j+1].rgbtRed) / 4);
            }

            //Bottom left corner
            if (i == height - 1 && j == 0)
            {
                img[i][j].tempBlue = roundf(((float)image[i-1][j].rgbtBlue + (float)image[i-1][j+1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue) / 4);
                img[i][j].tempGreen = roundf(((float)image[i-1][j].rgbtGreen + (float)image[i-1][j+1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen) / 4);
                img[i][j].tempRed = roundf(((float)image[i-1][j].rgbtRed + (float)image[i-1][j+1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed) / 4);
            }

            //Top right corner
            if (i == 0 && j == width - 1)
            {
                img[i][j].tempBlue = roundf(((float)image[i][j-1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i+1][j-1].rgbtBlue + (float)image[i+1][j].rgbtBlue) / 4);
                img[i][j].tempGreen = roundf(((float)image[i][j-1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i+1][j-1].rgbtGreen + (float)image[i+1][j].rgbtGreen) / 4);
                img[i][j].tempRed = roundf(((float)image[i][j-1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i+1][j-1].rgbtRed + (float)image[i+1][j].rgbtRed) / 4);
            }

            //Bottom right corner
            if (i == height - 1 && j == width - 1)
            {
                img[i][j].tempBlue = roundf(((float)image[i-1][j-1].rgbtBlue + (float)image[i-1][j].rgbtBlue + (float)image[i][j-1].rgbtBlue + (float)image[i][j].rgbtBlue) / 4);
                img[i][j].tempGreen = roundf(((float)image[i-1][j-1].rgbtGreen + (float)image[i-1][j].rgbtGreen + (float)image[i][j-1].rgbtGreen + (float)image[i][j].rgbtGreen) / 4);
                img[i][j].tempRed = roundf(((float)image[i-1][j-1].rgbtRed + (float)image[i-1][j].rgbtRed + (float)image[i][j-1].rgbtRed + (float)image[i][j].rgbtRed) / 4);
            }

            //Middle Bit, excluding edge
            else if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
            {
                img[i][j].tempBlue = roundf(( (float)image[i-1][j-1].rgbtBlue + (float)image[i-1][j].rgbtBlue + (float)image[i-1][j+1].rgbtBlue + (float)image[i][j-1].rgbtBlue + (float)image[i][j].rgbtBlue + (float)image[i][j+1].rgbtBlue + (float)image[i+1][j-1].rgbtBlue + (float)image[i+1][j].rgbtBlue + (float)image[i+1][j+1].rgbtBlue ) / 9);
                img[i][j].tempGreen = roundf(( (float)image[i-1][j-1].rgbtGreen + (float)image[i-1][j].rgbtGreen + (float)image[i-1][j+1].rgbtGreen + (float)image[i][j-1].rgbtGreen + (float)image[i][j].rgbtGreen + (float)image[i][j+1].rgbtGreen + (float)image[i+1][j-1].rgbtGreen + (float)image[i+1][j].rgbtGreen + (float)image[i+1][j+1].rgbtGreen ) / 9);
                img[i][j].tempRed = roundf(( (float)image[i-1][j-1].rgbtRed + (float)image[i-1][j].rgbtRed + (float)image[i-1][j+1].rgbtRed + (float)image[i][j-1].rgbtRed + (float)image[i][j].rgbtRed + (float)image[i][j+1].rgbtRed + (float)image[i+1][j-1].rgbtRed + (float)image[i+1][j].rgbtRed + (float)image[i+1][j+1].rgbtRed ) / 9);
            }
        }
    }

    //saving temp storage to new file
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = img[i][j].tempRed;
            image[i][j].rgbtGreen = img[i][j].tempGreen;
            image[i][j].rgbtBlue = img[i][j].tempBlue;
        }
    }


    return;
}
