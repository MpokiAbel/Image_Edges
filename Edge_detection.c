#include <stdio.h>
#include <stdlib.h>
#include "Util.c"
#include <math.h>

FILE *PGM_FILE, *OUTPUT_IMG;

void *convolution(int *filter, int row_of_filter, int column_of_filter, gray *img, int row_of_img, int column_of_img, int normalization_value)
{
    float *result_array;
    result_array = (float *)malloc(row_of_img * column_of_img * sizeof(float));

    float sum = 0;

    for (int bigrow = 0; bigrow < row_of_img - 2; bigrow++)
    {
        for (int bigcolumn = 0; bigcolumn < column_of_img - 2; bigcolumn++)
        {
            for (int i = 0; i < row_of_filter; i++)
            {
                for (int j = 0; j < column_of_filter; j++)
                {
                    sum += *(img + ((bigrow + i) * column_of_img + (bigcolumn + j))) * (*(filter + (i * column_of_filter + j)));
                }
            }
            *(result_array + (bigrow + 1) * column_of_img + (bigcolumn + 1)) = (sum / (float)normalization_value);
            sum = 0;
        }
    }
    return result_array;
}
void displayImg(int img_row, int img_column, int pgmraw, int maxval, gray *graymap, int threashold, char *name)
{
    OUTPUT_IMG = fopen(name, "w");
    if (OUTPUT_IMG == NULL)
    {
        printf("Error Could not read file");
        exit(-1);
    }
    else
    {
        printf("Creating file Success \n");
    }

    /*Writing Image Headers*/
    fprintf(OUTPUT_IMG, "P5\n");
    fprintf(OUTPUT_IMG, "%d %d \n", img_column, img_row);
    fprintf(OUTPUT_IMG, "%d\n", maxval);
    for (int i = 0; i < img_row; i++)
        for (int j = 0; j < img_column; j++)
        {
            if (threashold == 0)
            {
                // printf("%d ", graymap[i * img_column + j]);
                putc(graymap[i * img_column + j], OUTPUT_IMG);
            }
            else
            {
                if (threashold < (int)graymap[i * img_column + j])
                {
                    // printf("%d ", graymap[i * img_column + j]);
                    putc(graymap[i * img_column + j], OUTPUT_IMG);
                }
                else
                {
                    // printf("%d ", 0);
                    putc(0, OUTPUT_IMG);
                }
            }
        }
    fclose(OUTPUT_IMG);
}

// gray *bilinear_filter(gray *img, int row_of_img, int column_of_img, int normalization_value, int pgmraw, int img_maxval, int threshold)
// {
//     gray *result_image;
//     float *result_array;
//     int filter[3][3] = {
//         {1, 2, 1},
//         {2, 4, 2},
//         {1, 2, 1}};

//     result_image = (gray *)malloc(row_of_img * column_of_img * sizeof(gray));

//     result_array = convolution(&filter[0][0], 3, 3, img, row_of_img, column_of_img, normalization_value);

//     for (int i = 0; i < row_of_img; i++)
//     {
//         for (int j = 0; j < column_of_img; j++)
//         {
//             *(result_image + (i * column_of_img + j)) = (gray)(*(result_array + (i * column_of_img + j)));
//         }
//     }
//     displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, threshold, "Bilinear_Filtered.pgm");

//     free(result_array);
//     return result_image;
// }

void non_maximum_suppression(gray *gradients, int *direction, int rows, int columns)
{

    int direc_value, magn_value;

    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < columns; y++)
        {

            direc_value = *(direction + (x * columns + y));
            switch (direc_value)
            {
            case 0:
                if ((*(gradients + (x * columns + y))) < (*(gradients + (x * columns + y - 1))) || (*(gradients + (x * columns + y))) < (*(gradients + (x * columns + y + 1))))
                {
                    (*(gradients + (x * columns + y))) = 0;
                }

                break;

            case 1:
                if ((*(gradients + (x * columns + y))) < (*(gradients + ((x - 1) * columns + y - 1))) || (*(gradients + (x * columns + y))) < (*(gradients + ((x + 1) * columns + y + 1))))
                {
                    (*(gradients + (x * columns + y))) = 0;
                }

                break;

            case 2:
                if ((*(gradients + (x * columns + y))) < (*(gradients + ((x - 1) * columns + y - 1))) || (*(gradients + (x * columns + y))) < (*(gradients + ((x + 1) * columns + y + 1))))
                {
                    (*(gradients + (x * columns + y))) = 0;
                }

                break;

            case 3:
                if ((*(gradients + (x * columns + y))) < (*(gradients + ((x - 1) * columns + y + 1))) || (*(gradients + (x * columns + y))) < (*(gradients + ((x + 1) * columns + y - 1))))
                {
                    (*(gradients + (x * columns + y))) = 0;
                }

                break;

            default:
                break;
            }
        }
    }
}

void Scharr_gradient(gray *img, int row_of_img, int column_of_img, int normalization_value, int pgmraw, int img_maxval, int threshold)
{
    gray *result_image;
    float square = 0, arctan = 0;
    float *horizontal, *vertical;
    int *direction_image;
    int filter_x[3][3] = {
        {-3, -10, -3},
        {0, 0, 0},
        {3, 10, 3}};

    int filter_y[3][3] = {
        {-3, 0, 3},
        {-10, 0, 10},
        {-3, 0, 3}};

    result_image = (gray *)malloc(row_of_img * column_of_img * sizeof(gray));
    direction_image = (int *)malloc(row_of_img * column_of_img * sizeof(int));

    horizontal = convolution(&filter_x[0][0], 3, 3, img, row_of_img, column_of_img, normalization_value);
    // converter(row_of_img, column_of_img, horizontal, pgmraw, img_maxval, threshold, "Horizontal_Image.pgm");
    for (int i = 0; i < row_of_img; i++)
    {
        for (int j = 0; j < column_of_img; j++)
        {
            *(result_image + (i * column_of_img + j)) = (gray)(*(horizontal + (i * column_of_img + j)));
        }
    }
    displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, 0, "Horizontal_Image.pgm");
    vertical = convolution(&filter_y[0][0], 3, 3, img, row_of_img, column_of_img, normalization_value);
    // converter(row_of_img, column_of_img, vertical, pgmraw, img_maxval, threshold, "vertical_Image.pgm");
    for (int i = 0; i < row_of_img; i++)
    {
        for (int j = 0; j < column_of_img; j++)
        {
            *(result_image + (i * column_of_img + j)) = (gray)(*(vertical + (i * column_of_img + j)));
        }
    }
    displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, 0, "Vertical_Image.pgm");

    for (int i = 0; i < row_of_img; i++)
    {
        for (int j = 0; j < column_of_img; j++)
        {
            square = pow((*(horizontal + (i * column_of_img + j))), 2) + pow((*(vertical + (i * column_of_img + j))), 2);
            *(result_image + (i * column_of_img + j)) = (gray)(round(sqrt(square)));

            arctan = atanf((*(vertical + (i * column_of_img + j))) / (*(horizontal + (i * column_of_img + j))));
            arctan = arctan * 180 / 3.1415926;
            

            if (arctan <= 22.5 || arctan > 157.5)
            {
                *(direction_image + (i * column_of_img + j)) = 0; // 0 degree
            }
            else if (arctan <= 67.5)
            {
                *(direction_image + (i * column_of_img + j)) = 1; // 45 degree
            }
            else if (arctan <= 112.5)
            {
                *(direction_image + (i * column_of_img + j)) = 2; // 90 degree
            }
            else if (arctan <= 157.5)
            {
                *(direction_image + (i * column_of_img + j)) = 3; // 135 degree
            }
        }
    }
    displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, threshold, "Gradient_Magnitude.pgm");

    non_maximum_suppression(result_image, direction_image, row_of_img, column_of_img);

    displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, threshold, "Non_Maximum_Suppression.pgm");

    free(horizontal);
    free(vertical);
    free(result_image);
    free(direction_image);
}

int openImg()
{
    int ich1, ich2, pgmraw;
    PGM_FILE = fopen("boat_filtered.pgm", "r");
    if (PGM_FILE == NULL)
    {
        printf("Error Could not read file");
        exit(-1);
    }
    else
    {
        printf("Opening file Success \n");
    }

    /*  Magic number reading */
    ich1 = getc(PGM_FILE);
    if (ich1 == EOF)
        printf("EOF / read error / magic number");
    ich2 = getc(PGM_FILE);
    if (ich2 == EOF)
        printf("EOF /read error / magic number");
    if (ich2 != '2' && ich2 != '5')
        printf(" wrong file type ");
    else if (ich2 == '2')
        pgmraw = 0;
    else
        pgmraw = 1;
    return pgmraw;
}

void readImage(int img_row, int img_column, int pgmraw, gray *img_ptr)
{
    for (int i = 0; i < img_row; i++)
        for (int j = 0; j < img_column; j++)
            if (pgmraw)
                img_ptr[i * img_column + j] = pm_getrawbyte(PGM_FILE);
            else
                img_ptr[i * img_column + j] = pm_getint(PGM_FILE);
}

void close(gray *img_ptr, gray *result_array)
{
    fclose(PGM_FILE);
    free(result_array);
    free(img_ptr);
}

int main(int argc, char **argv)
{
    int img_row, img_column, img_maxval, pgmraw, norm_val, threshold;
    gray *img_ptr;
    gray *result_array;

    if (argc != 2)
    {
        fprintf(stderr, "usage : %s threshold \n", argv[0]);
        exit(-1);
    }

    threshold = atoi(argv[1]);
    pgmraw = openImg();
    norm_val = 16;
    img_column = pm_getint(PGM_FILE);
    img_row = pm_getint(PGM_FILE);
    img_maxval = pm_getint(PGM_FILE);
    /* Storage for convolution result */
    result_array = (gray *)malloc(img_column * img_row * sizeof(gray));

    /*Array(Pixel) values for Image*/
    img_ptr = (gray *)malloc(img_column * img_row * sizeof(gray));

    /*Reading Image into Array*/
    readImage(img_row, img_column, pgmraw, img_ptr);
    // result_array = bilinear_filter(img_ptr, img_row, img_column, norm_val, pgmraw, img_maxval, 16);
    Scharr_gradient(img_ptr, img_row, img_column, norm_val, pgmraw, img_maxval, threshold);

    // displayImg(img_row, img_column, pgmraw, img_maxval, result_array,threshold);

    close(img_ptr, result_array);
    return 0;
}
