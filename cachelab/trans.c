/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void transpose_32x32(int M, int N, int A[N][M], int B[M][N]);
void transpose_64x64(int M, int N, int A[N][M], int B[M][N]);
void transpose_61x67(int M, int N, int A[N][M], int B[M][N]);
void transpose_block8(int M, int N, int A[N][M], int B[M][N]);
void transpose_block4(int M, int N, int A[N][M], int B[M][N]);
void trans(int M, int N, int A[N][M], int B[M][N]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32) {
        return transpose_32x32(M, N, A, B);
    }
    if (M == 64 && N == 64) {
        return transpose_64x64(M, N, A, B);
    }
    if (M == 61 && N == 67) {
        return transpose_61x67(M, N, A, B);
    }
    return trans(M, N, A, B);
}

// optimized for 32x32: 259 misses
void transpose_32x32(int M, int N, int A[N][M], int B[M][N]) {
    int a, b, i, j;
    int v0, v1, v2, v3, v4, v5, v6, v7;
    for (a = 0; a < N; a += 8) {
        for (b = 0; b < M; b += 8) {
            if (a == b) {
                // row 0
                v0 = A[a][b+0]; v1 = A[a][b+1]; v2 = A[a][b+2]; v3 = A[a][b+3];
                v4 = A[a][b+4]; v5 = A[a][b+5]; v6 = A[a][b+6]; v7 = A[a][b+7];

                B[a][b+0] = v0; B[a][b+1] = v1; B[a][b+2] = v2; B[a][b+3] = v3;
                B[a][b+4] = v4; B[a][b+5] = v5; B[a][b+6] = v6; B[a][b+7] = v7;

                // row 1
                v0 = A[a+1][b+0]; v1 = A[a+1][b+1]; v2 = A[a+1][b+2]; v3 = A[a+1][b+3];
                v4 = A[a+1][b+4]; v5 = A[a+1][b+5]; v6 = A[a+1][b+6]; v7 = A[a+1][b+7];

                B[a+1][b+0] = B[a+0][b+1];
                B[a+0][b+1] = v0; B[a+1][b+1] = v1; B[a+1][b+2] = v2; B[a+1][b+3] = v3;
                B[a+1][b+4] = v4; B[a+1][b+5] = v5; B[a+1][b+6] = v6; B[a+1][b+7] = v7;

                // row2
                v0 = A[a+2][b+0]; v1 = A[a+2][b+1]; v2 = A[a+2][b+2]; v3 = A[a+2][b+3];
                v4 = A[a+2][b+4]; v5 = A[a+2][b+5]; v6 = A[a+2][b+6]; v7 = A[a+2][b+7];

                B[a+2][b+0] = B[a+0][b+2]; B[a+2][b+1] = B[a+1][b+2];
                B[a+0][b+2] = v0; B[a+1][b+2] = v1; B[a+2][b+2] = v2; B[a+2][b+3] = v3;
                B[a+2][b+4] = v4; B[a+2][b+5] = v5; B[a+2][b+6] = v6; B[a+2][b+7] = v7;

                // row 3
                v0 = A[a+3][b+0]; v1 = A[a+3][b+1]; v2 = A[a+3][b+2]; v3 = A[a+3][b+3];
                v4 = A[a+3][b+4]; v5 = A[a+3][b+5]; v6 = A[a+3][b+6]; v7 = A[a+3][b+7];

                B[a+3][b+0] = B[a+0][b+3]; B[a+3][b+1] = B[a+1][b+3]; B[a+3][b+2] = B[a+2][b+3];
                B[a+0][b+3] = v0; B[a+1][b+3] = v1; B[a+2][b+3] = v2; B[a+3][b+3] = v3;
                B[a+3][b+4] = v4; B[a+3][b+5] = v5; B[a+3][b+6] = v6; B[a+3][b+7] = v7;

                // row 4
                v0 = A[a+4][b+0]; v1 = A[a+4][b+1]; v2 = A[a+4][b+2]; v3 = A[a+4][b+3];
                v4 = A[a+4][b+4]; v5 = A[a+4][b+5]; v6 = A[a+4][b+6]; v7 = A[a+4][b+7];

                B[a+4][b+0] = B[a+0][b+4]; B[a+4][b+1] = B[a+1][b+4]; B[a+4][b+2] = B[a+2][b+4];
                B[a+4][b+3] = B[a+3][b+4];
                B[a+0][b+4] = v0; B[a+1][b+4] = v1; B[a+2][b+4] = v2; B[a+3][b+4] = v3;
                B[a+4][b+4] = v4; B[a+4][b+5] = v5; B[a+4][b+6] = v6; B[a+4][b+7] = v7;

                // row 5
                v0 = A[a+5][b+0]; v1 = A[a+5][b+1]; v2 = A[a+5][b+2]; v3 = A[a+5][b+3];
                v4 = A[a+5][b+4]; v5 = A[a+5][b+5]; v6 = A[a+5][b+6]; v7 = A[a+5][b+7];

                B[a+5][b+0] = B[a+0][b+5]; B[a+5][b+1] = B[a+1][b+5]; B[a+5][b+2] = B[a+2][b+5];
                B[a+5][b+3] = B[a+3][b+5]; B[a+5][b+4] = B[a+4][b+5];
                B[a+0][b+5] = v0; B[a+1][b+5] = v1; B[a+2][b+5] = v2; B[a+3][b+5] = v3;
                B[a+4][b+5] = v4; B[a+5][b+5] = v5; B[a+5][b+6] = v6; B[a+5][b+7] = v7;

                // row 6
                v0 = A[a+6][b+0]; v1 = A[a+6][b+1]; v2 = A[a+6][b+2]; v3 = A[a+6][b+3];
                v4 = A[a+6][b+4]; v5 = A[a+6][b+5]; v6 = A[a+6][b+6]; v7 = A[a+6][b+7];

                B[a+6][b+0] = B[a+0][b+6]; B[a+6][b+1] = B[a+1][b+6]; B[a+6][b+2] = B[a+2][b+6];
                B[a+6][b+3] = B[a+3][b+6]; B[a+6][b+4] = B[a+4][b+6]; B[a+6][b+5] = B[a+5][b+6];
                B[a+0][b+6] = v0; B[a+1][b+6] = v1; B[a+2][b+6] = v2; B[a+3][b+6] = v3;
                B[a+4][b+6] = v4; B[a+5][b+6] = v5; B[a+6][b+6] = v6; B[a+6][b+7] = v7;

                // row 7
                v0 = A[a+7][b+0]; v1 = A[a+7][b+1]; v2 = A[a+7][b+2]; v3 = A[a+7][b+3];
                v4 = A[a+7][b+4]; v5 = A[a+7][b+5]; v6 = A[a+7][b+6]; v7 = A[a+7][b+7];

                B[a+7][b+0] = B[a+0][b+7]; B[a+7][b+1] = B[a+1][b+7]; B[a+7][b+2] = B[a+2][b+7];
                B[a+7][b+3] = B[a+3][b+7]; B[a+7][b+4] = B[a+4][b+7]; B[a+7][b+5] = B[a+5][b+7];
                B[a+7][b+6] = B[a+6][b+7];
                B[a+0][b+7] = v0; B[a+1][b+7] = v1; B[a+2][b+7] = v2; B[a+3][b+7] = v3;
                B[a+4][b+7] = v4; B[a+5][b+7] = v5; B[a+6][b+7] = v6;B[a+7][b+7] = v7;
            } else {
                for (i = a; i < a + 8; ++i) {
                    for (j = b; j < b + 8; ++j) {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
}

// optimized for 64x64: 1179 misses
void transpose_64x64(int M, int N, int A[N][M], int B[M][N]) {
    int a, b, i, j;
    int v0, v1, v2, v3, v4, v5, v6, v7;
    for (a = 0; a < N; a += 8) {
        for (b = 0; b < M; b += 8) {
            for (i = a; i < a + 4; ++i) {
                v0 = A[i][b+0]; v1 = A[i][b+1]; v2 = A[i][b+2]; v3 = A[i][b+3];
                v4 = A[i][b+4]; v5 = A[i][b+5]; v6 = A[i][b+6]; v7 = A[i][b+7];
                j = i + 4;
                B[b+0][i] = v0; B[b+1][i] = v1; B[b+2][i] = v2; B[b+3][i] = v3;
                B[b+0][j] = v4; B[b+1][j] = v5; B[b+2][j] = v6; B[b+3][j] = v7;
            }
            for (j = b; j < b + 4; ++j) {
                v4 = A[a+4][j]; v5 = A[a+5][j]; v6 = A[a+6][j]; v7 = A[a+7][j];
                v0 = B[j][a+4]; v1 = B[j][a+5]; v2 = B[j][a+6]; v3 = B[j][a+7];
                i = j + 4;
                B[j][a+4] = v4; B[j][a+5] = v5; B[j][a+6] = v6; B[j][a+7] = v7;
                B[i][a+0] = v0; B[i][a+1] = v1; B[i][a+2] = v2; B[i][a+3] = v3;
            }
            for (i = a + 4; i < a + 8; ++i) {
                v4 = A[i][b+4]; v5 = A[i][b+5]; v6 = A[i][b+6]; v7 = A[i][b+7];
                B[b+4][i] = v4; B[b+5][i] = v5; B[b+6][i] = v6; B[b+7][i] = v7;
            }
        }
    }
}

// optimized for 61x67
void transpose_61x67(int M, int N, int A[N][M], int B[M][N]) {
    int b, i, m = M / 8 * 8;
    int v0, v1, v2, v3, v4, v5, v6, v7;
    for (b = 0; b < m; b += 8) {
        for (i = 0; i < N; ++i) {
            v0 = A[i][b+0]; v1 = A[i][b+1]; v2 = A[i][b+2]; v3 = A[i][b+3];
            v4 = A[i][b+4]; v5 = A[i][b+5]; v6 = A[i][b+6]; v7 = A[i][b+7];

            B[b+0][i] = v0; B[b+1][i] = v1; B[b+2][i] = v2; B[b+3][i] = v3;
            B[b+4][i] = v4; B[b+5][i] = v5; B[b+6][i] = v6; B[b+7][i] = v7;
        }
    }
    for (i = 0; i < N; ++i) {
        v0 = A[i][b]; v1 = A[i][b+1]; v2 = A[i][b+2]; v3 = A[i][b+3]; v4 = A[i][b+4];
        B[b][i] = v0; B[b+1][i] = v1; B[b+2][i] = v2; B[b+3][i] = v3; B[b+4][i] = v4;
    }
}

// for 32x32: 287 misses
void transpose_block8(int M, int N, int A[N][M], int B[M][N]) {
    int a, b, i, j, ns = 8;
    for (a = 0; a < N; a += ns) {
        for (b = 0; b < M; b += ns) {
            for (i = a; i < a + ns; ++i) {
                for (j = b; j < b + ns; ++j) {
                    if (i != j) B[j][i] = A[i][j];
                }
                if (a == b) B[i][i] = A[i][i];
            }
        }
    }
}

// for 64x64 (column traversal): 1651 misses
void transpose_block4(int M, int N, int A[N][M], int B[M][N]) {
    int a, b, i, ns = 4;
    int v0, v1, v2, v3;
    for (b = 0; b < M; b += ns) {
        for (a = 0; a < N; a += ns) {
            for (i = a; i < a + ns; ++i) {
                v0 = A[i][b+0];
                v1 = A[i][b+1];
                v2 = A[i][b+2];
                v3 = A[i][b+3];
                B[b+0][i] = v0;
                B[b+1][i] = v1;
                B[b+2][i] = v2;
                B[b+3][i] = v3;
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

