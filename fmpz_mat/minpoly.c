#include "flint.h"
#include "fmpz_mat.h"
#include "fmpz.h"
#include "fmpz_poly.h"

/*
   Assumes that \code{mat} is an $n \times n$ matrix and sets \code{cp}
   to its minimal polynomial (which has at most $n+1$ terms).
   */

void fmpz_mat_minpoly(fmpz_poly_t cp, const fmpz_mat_t mat)
{
	if (mat->r != mat->c)
	{
		flint_printf("Exception (fmpz_mat_minpoly). Non-square matrix.\n");
		abort();
	}

	fmpz_poly_fit_length(cp, mat->r + 1);
	_fmpz_poly_set_length(cp, mat->r + 1);
	
	const slong n = mat->c;
	
	slong n_square = n * n;

	if (n == 0)
	{
		fmpz_one(cp);
	}
	else
	{
		slong i;
		slong idx = 1;

		fmpz_mat_t tmp;
		fmpz_mat_init(tmp, n + 1, n_square);
		fmpz_mat_zero(tmp);

		fmpz_mat_t power_matrix;
		fmpz_mat_init(power_matrix, n, n);
		fmpz_mat_one(power_matrix);

		for (i = 0; i < n; i++)
		{
			fmpz_set_si(fmpz_mat_entry(tmp, 0, i * n + i), 1);
		}

		fmpz_mat_t qu;
		fmpz_mat_init(qu, n + 1, n + 1);
		fmpz_mat_zero(qu);
		
		fmpz_t gcd;
		fmpz_init(gcd);
		fmpz_set_si(gcd, 0);

		int terminate = 0;

		while (!terminate)
		{
			fmpz_mat_mul(power_matrix, power_matrix, mat);
			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					fmpz * val = fmpz_mat_entry(power_matrix, i, j);
					fmpz_set_si(fmpz_mat_entry(tmp, idx, i * n + j), *val); 
				}
			}
			
			idx++;

			fmpz_mat_t transpose;
			fmpz_mat_init(transpose, n_square, n + 1);
			fmpz_mat_transpose(transpose, tmp);

			slong nullity = fmpz_mat_nullspace(qu, transpose);
			
			fmpz_mat_clear(transpose);

			for (i = 0; i < n; i++)
			{
				fmpz * tmp = fmpz_mat_entry(qu, i, 0);

				fmpz_t s;
				fmpz_init(s);
				fmpz_set_si(s, *tmp);

				fmpz_gcd(gcd, gcd, s);
			}

			if (idx == n + 1)
			{
				terminate = 1;
			}
		}

		if (!fmpz_is_zero(gcd))
		{
			for (i = 0; i < n + 1; i++)
			{
				fmpz * tmp = fmpz_mat_entry(qu, i, 0);

				fmpz_t s;
				fmpz_init(s);
				fmpz_set_si(s, *tmp);
				fmpz_divexact(s, s, gcd);
				fmpz_set_si(fmpz_mat_entry(qu, i, 0), *s);
			}
		}

		for (i = 0; i < qu->r; i++)
		{
			fmpz * tmp = fmpz_mat_entry(qu, i, 0);
			fmpz_set_si((cp->coeffs) + i, *tmp);
		}
		
		fmpz_mat_clear(tmp);
		fmpz_mat_clear(power_matrix);
		fmpz_mat_clear(qu);
	}
}
