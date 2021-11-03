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

// gray *converter(int row_of_img, int column_of_img, float *values, int pgmraw, int img_maxval, int threshold, char *name)
// {
//     gray *result_image;
//     for (int i = 0; i < row_of_img; i++)
//     {
//         for (int j = 0; j < column_of_img; j++)
//         {
//             *(result_image + (i * column_of_img + j)) = (gray)(*(values + (i * column_of_img + j)));
//         }
//     }
//     displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, threshold, name);

//     return result_image;
// }

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

void Scharr_gradient(gray *img, int row_of_img, int column_of_img, int normalization_value, int pgmraw, int img_maxval, int threshold)
{
    gray *result_image;
    float square = 0;
    float *horizontal, *vertical;
    int filter_x[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    int filter_y[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    result_image = (gray *)malloc(row_of_img * column_of_img * sizeof(gray));

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
        }
    }
    displayImg(row_of_img, column_of_img, pgmraw, img_maxval, result_image, threshold, "Gradient_Magnitude.pgm");

    free(horizontal);
    free(vertical);
    free(result_image);
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
    norm_val = 4;
    img_column = pm_getint(PGM_FILE);
    img_row = pm_getint(PGM_FILE);
    img_maxval = pm_getint(PGM_FILE);
    /* Storage for convolution result */
    result_array = (gray *)malloc(img_column * img_row * sizeof(gray));

    /*Array(Pixel) values for Image*/
    img_ptr = (gray *)malloc(img_column * img_row * sizeof(gray));

    /*Reading Image into Array*/
    readImage(img_row, img_column, pgmraw, img_ptr);
    //result_array = bilinear_filter(img_ptr, img_row, img_column, norm_val, pgmraw, img_maxval, 16);
    Scharr_gradient(img_ptr, img_row, img_column, norm_val, pgmraw, img_maxval, threshold);

    // displayImg(img_row, img_column, pgmraw, img_maxval, result_array,threshold);

    close(img_ptr, result_array);
    return 0;
}
