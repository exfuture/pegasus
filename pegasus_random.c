/**
 * @author Oleksandr Natalenko aka post-factum <oleksandr@natalenko.name>
 *
 * @section LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * pegasus — digital channel simulator, random file
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "pegasus_common_types.h"

#include "pegasus_panic.h"
#include "pegasus_tools.h"

#include "pegasus_random.h"

void pgr_init_seed()
{
	struct timespec current_time;
	pgr_seed = malloc(sizeof(uint64_t) * pgt_threads);
	if (unlikely(pgr_seed == NULL))
		pgp_malloc();

	for (unsigned long long i = 0; i < pgt_threads; i++)
	{
		if (unlikely(clock_gettime(CLOCK_REALTIME, &current_time) != 0))
			pgp_clock_gettime();
		pgr_seed[i] = current_time.tv_sec * 1000000000L + current_time.tv_nsec;
	}
}

uint64_t __pgr_get_u64(unsigned long long _thread)
{
	pgr_seed[_thread] ^= (pgr_seed[_thread] << 21);
	pgr_seed[_thread] ^= (pgr_seed[_thread] >> 35);
	pgr_seed[_thread] ^= (pgr_seed[_thread] << 4);
	return pgr_seed[_thread];
}

double __pgr_get_lf(unsigned long long _thread)
{
	return fabs((double)__pgr_get_u64(_thread) / (double)PGS_UINT64_MAX);
}

double __pgr_get_gauss(unsigned long long _thread)
{
	double r, x, y;
	do
	{
		x = 2.0 * __pgr_get_lf(_thread) - 1.0;
		y = 2.0 * __pgr_get_lf(_thread) - 1.0;
		r = pow(x, 2.0) + pow(y, 2.0);
	} while (r > 1.0 || r == 0.0);
	return x * sqrt(-2.0 * log(r) / r);
}

