#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fmpz_poly_q.h"

/**
 * \ingroup  StringConversions
 * 
 * Returns the pretty string representation of \c op.
 * 
 * Returns the pretty string representation of the rational function \c op, 
 * using the string \c x as the variable name.
 */
char * fmpz_poly_q_get_str_pretty(const fmpz_poly_q_t op, const char *x)
{
    int i, j;
    char * str;
    char * numstr;
    char * denstr;
    
    if (fmpz_poly_is_one(op->den))
    {
        return fmpz_poly_get_str_pretty(op->num, x);
    }
    
    numstr = fmpz_poly_get_str_pretty(op->num, x);
    denstr = fmpz_poly_get_str_pretty(op->den, x);
    
    str = malloc(strlen(numstr) + strlen(denstr) + 6);
    if (!str)
    {
        printf("ERROR (fmpz_poly_q_get_str_pretty).  Memory allocation failed.\n");
        abort();
    }
    
    i = 0;
    if (fmpz_poly_degree(op->num) > 0)
    {
        str[i++] = '(';
        for (j = 0; j < strlen(numstr); j++)
            str[i++] = numstr[j];
        str[i++] = ')';
    }
    else
    {
        for (j = 0; j < strlen(numstr); j++)
            str[i++] = numstr[j];
    }
    str[i++] = '/';
    if (fmpz_poly_degree(op->den) > 0)
    {
        str[i++] = '(';
        for (j = 0; j < strlen(denstr); j++)
            str[i++] = denstr[j];
        str[i++] = ')';
    }
    else
    {
        for (j = 0; j < strlen(denstr); j++)
            str[i++] = denstr[j];
    }
    str[i] = '\0';
    
    free(numstr);
    free(denstr);
    
    return str;
}