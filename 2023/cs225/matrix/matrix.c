#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void  matrix_print( Matrix m, int num_rows, int num_columns ) {
    int i, j;
    for ( i=0; i<num_rows; ++i ) {
        for ( j=0; j<num_columns; ++j ) {
            printf( "%4i ", m[i][j] );
        }
        printf( "\n" );
    }
}


/* allocate 2-dimentional array with the specified dimensions */
Matrix matrix_create(int num_rows, int num_columns)
{
    Matrix m = (Matrix)calloc(num_rows, sizeof(int*));
    int i = 0;
    for (i = 0; i < num_rows; i++)
    {
        m[i] = (int*)calloc(num_columns, sizeof(int));
    }
    return m;
}
/* matrix addition, assume result matrix is already allocated */
void matrix_add(Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns)
{
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }
}
/* matrix transposition, returns a new matrix, does not delete the original */
Matrix matrix_transpose(Matrix m, int num_rows, int num_columns)
{
    Matrix ret = matrix_create(num_columns, num_rows);
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            ret[j][i] = m[i][j];
        }
    }
    return ret;
}
/* deallocate matrix */
void   matrix_delete(Matrix m, int num_rows)
{
    int i;
    for (i = 0; i < num_rows; i++)
    {
        free(m[i]);
    }
    free(m);
}

/* delete one row (index r) from matrix */
void   matrix_delete_row(Matrix m, int r, int num_rows)
{
    if (r < num_rows)
    {
        int i;
        free(m[r]);
        for (i = r+1; i < num_rows; i++)
        {
            m[i - 1] = m[i];
        }
    }
    
}
/* delete one column (index c) from matrix */
void   matrix_delete_column(Matrix m, int c, int num_rows, int num_columns)
{
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        for (j = c + 1; j < num_columns; j++)
        {
            m[i][j - 1] = m[i][j];
        }
    }

}
