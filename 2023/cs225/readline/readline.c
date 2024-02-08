/**************************
     Author: andrew kitzan (andrew.kitzan@digipen.edu)
last edited: 8/30/23
       desc: reads the first line of the passed in file.
**************************/

/* libraries */

#include <stdio.h>  /* Null, file, feof, fgets */
#include <stdlib.h> /* calloc, realloc         */
#include <string.h> /* strlen, strcpy          */

#define TRUE 1



char * readline( FILE * stream )
{
    /* set up varables */
    char * str;
    char * tempStr;
    int capacity = 2;
    int size = 0;


    /* check if file was opened */
    if ( !stream )
    {
        return NULL;
    }

    /* allocate the string for the first time */
    str = (char *)calloc( capacity, sizeof(char) );

    while ( TRUE )
    {
        /* collect the string from the stream up to capacity-1 char */
        fgets( str + size, capacity - size, stream );

        if ( str == NULL )
        {
            break;
        }

        size = strlen(str);

        /* if at end of file or line then stop looping */
        if( str[size - 1] == '\n' || feof(stream) || str[size - 1] == '\0')
        {
            break;
        }
        
        /* realloc old string*/
        capacity *= 2;
        tempStr = (char *)calloc( capacity, sizeof(char) );
        strcpy( tempStr, str );
        free( str );
        str = tempStr;
    }

    /* if new line charecter was found then replace with */
    if( str[size - 1] == '\n' )
    {
        str[size - 1] = '\0';
    }

    return str;
}