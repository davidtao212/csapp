/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Tao Cheng",              /* Team name */

    "Tao Cheng",     /* First member full name */
    "chengtao06@gmail.com",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * Simply switch i,j, because cache write miss is costly than read miss
 */
char rotate_switch_ij_descr[] = "rotate: Simply switch i j";
void rotate_switch_ij(int dim, pixel *src, pixel *dst)
{
    int i, j, k;

    for (j = 0; j < dim; j++) {
        k = dim-1-j;
        for (i = 0; i < dim; i++)
            dst[RIDX(k, i, dim)] = src[RIDX(i, j, dim)];
    }
}

char rotate_block8_descr[] = "rotate: Block 8";
void rotate_block8(int dim, pixel *src, pixel *dst)
{
    int a, j;
    pixel *s, *d;
    for (a = 0; a < dim; a += 8) {
        for (j = 0; j < dim; ++j) {
            // i: a+0 ... a+7; j: j
            s = src + a * dim + j;
            d = dst + (dim - 1 - j) * dim + a;

            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *d = *s; // d -= dim;
        }
    }
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int a, j;
    pixel *s, *d;
    for (a = 0; a < dim; a += 16) {
        for (j = 0; j < dim; ++j) {
            // i: a+0 ... a+7; j: j
            s = src + a * dim + j;
            d = dst + (dim - 1 - j) * dim + a;

            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;

            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *(d++) = *s; s += dim;
            *d = *s; // d -= dim;
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
    add_rotate_function(&rotate_switch_ij, rotate_switch_ij_descr);
    add_rotate_function(&rotate_block8, rotate_block8_descr);
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

#define AVG_9(v0, v1, v2, v3, v4, v5, v6, v7, v8, c) ((v0)->c + (v1)->c + (v2)->c + (v3)->c + (v4)->c + (v5)->c + (v6)->c + (v7)->c + (v8)->c) / 9u
#define AVG_6(v0, v1, v2, v3, v4, v5, c) ((v0)->c + (v1)->c + (v2)->c + (v3)->c + (v4)->c + (v5)->c) / 6u
#define AVG_4(v0, v1, v2, v3, c) ((v0)->c + (v1)->c + (v2)->c + (v3)->c) / 4u

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    pixel *v0 = src, *v1 = src, *v2 = src + dim;

    dst->red   = AVG_4(v1, v1+1, v2, v2+1, red);
    dst->green = AVG_4(v1, v1+1, v2, v2+1, green);
    dst->blue  = AVG_4(v1, v1+1, v2, v2+1, blue);
    v1++; v2++; dst++;
    for (j = 1; j < dim - 1; ++j) {
        dst->red   = AVG_6(v1-1, v1, v1+1, v2-1, v2, v2+1, red);
        dst->green = AVG_6(v1-1, v1, v1+1, v2-1, v2, v2+1, green);
        dst->blue  = AVG_6(v1-1, v1, v1+1, v2-1, v2, v2+1, blue);
        v1++; v2++; dst++;
    }
    dst->red   = AVG_4(v1-1, v1, v2-1, v2, red);
    dst->green = AVG_4(v1-1, v1, v2-1, v2, green);
    dst->blue  = AVG_4(v1-1, v1, v2-1, v2, blue);
    v1++; v2++; dst++;

    for (i = 1; i < dim - 1; ++i) {
        dst->red   = AVG_6(v0, v0+1, v1, v1+1, v2, v2+1, red);
        dst->green = AVG_6(v0, v0+1, v1, v1+1, v2, v2+1, green);
        dst->blue  = AVG_6(v0, v0+1, v1, v1+1, v2, v2+1, blue);
        v0++; v1++; v2++; dst++;
        for (j = 1; j < dim - 1; ++j) {
            dst->red   = AVG_9(v0-1, v0, v0+1, v1-1, v1, v1+1, v2-1, v2, v2+1, red);
            dst->green = AVG_9(v0-1, v0, v0+1, v1-1, v1, v1+1, v2-1, v2, v2+1, green);
            dst->blue  = AVG_9(v0-1, v0, v0+1, v1-1, v1, v1+1, v2-1, v2, v2+1, blue);
            v0++; v1++; v2++; dst++;
        }
        dst->red   = AVG_6(v0-1, v0, v1-1, v1, v2-1, v2, red);
        dst->green = AVG_6(v0-1, v0, v1-1, v1, v2-1, v2, green);
        dst->blue  = AVG_6(v0-1, v0, v1-1, v1, v2-1, v2, blue);
        v0++; v1++; v2++; dst++;
    }

    dst->red   = AVG_4(v0, v0+1, v1, v1+1, red);
    dst->green = AVG_4(v0, v0+1, v1, v1+1, green);
    dst->blue  = AVG_4(v0, v0+1, v1, v1+1, blue);
    v0++; v1++; dst++;
    for (j = 1; j < dim - 1; ++j) {
        dst->red   = AVG_6(v0-1, v0, v0+1, v1-1, v1, v1+1, red);
        dst->green = AVG_6(v0-1, v0, v0+1, v1-1, v1, v1+1, green);
        dst->blue  = AVG_6(v0-1, v0, v0+1, v1-1, v1, v1+1, blue);
        v0++; v1++; dst++;
    }
    dst->red   = AVG_4(v0-1, v0, v1-1, v1, red);
    dst->green = AVG_4(v0-1, v0, v1-1, v1, green);
    dst->blue  = AVG_4(v0-1, v0, v1-1, v1, blue);
}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

