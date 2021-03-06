/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011, 2012 Sebastian Pancratz
    Copyright (C) 2014 William Hart
 
******************************************************************************/

#include "padic.h"

int padic_sub_exact(padic_t rop, const padic_t op1, const padic_t op2, 
               const padic_ctx_t ctx)
{
    if (padic_is_zero(op1))
    {
        if (!padic_is_zero(op2))
           return 0;

        padic_zero(rop);
    }
    else if (padic_is_zero(op2))
    {
        fmpz_set(padic_unit(rop), padic_unit(op1));
        padic_val(rop) = padic_val(op1);
    }
    else
    {
        if (padic_val(op1) == padic_val(op2))
        {

            fmpz_sub(padic_unit(rop), padic_unit(op1), padic_unit(op2));
            
            if (fmpz_sgn(padic_unit(rop)) < 0)
               return 0;

            padic_val(rop) = padic_val(op1);

            _padic_canonicalise(rop, ctx);
        }
        else if (padic_val(op1) < padic_val(op2))
        {
            fmpz_t f;

            fmpz_init(f);
            padic_ctx_pow_ui(f, padic_val(op2) - padic_val(op1), ctx);
            if (rop != op2)
            {
                fmpz_set(padic_unit(rop), padic_unit(op1));
                fmpz_submul(padic_unit(rop), f, padic_unit(op2));
 
            }
            else
            {
                fmpz_mul(padic_unit(rop), f, padic_unit(op2));
                fmpz_sub(padic_unit(rop), padic_unit(op1), padic_unit(rop));
            }
            fmpz_clear(f);

            if (fmpz_sgn(padic_unit(rop)) < 0)
                   return 0;

            padic_val(rop) = padic_val(op1);
        }
        else  /* padic_val(op1) > padic_val(op2) */
        {
            fmpz_t f;

            fmpz_init(f);
            padic_ctx_pow_ui(f, padic_val(op1) - padic_val(op2), ctx);
            if (rop != op1)
            {
                fmpz_neg(padic_unit(rop), padic_unit(op2));
                fmpz_addmul(padic_unit(rop), f, padic_unit(op1));
            }
            else
            {
                fmpz_mul(padic_unit(rop), f, padic_unit(op1));
                fmpz_sub(padic_unit(rop), padic_unit(rop), padic_unit(op2));
            }
            fmpz_clear(f);

            if (fmpz_sgn(padic_unit(rop)) < 0)
                   return 0;

            padic_val(rop) = padic_val(op2);
        }
    }

    return 1;
}

