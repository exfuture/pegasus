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
 * pegasus — digital channel simulator, selftests file
 */

#include <stdio.h>

#include "pegasus_random.h"

#include "pegasus_selftests.h"

static void pgst_show_result_lf(const char* _test_name,
		double _real_value,
		double _desired_value)
{
	printf("[%s] got: %lf, should be: %lf\n", _test_name, _real_value, _desired_value);
}

void pgst_run_tests()
{
	printf("Performing RNG normalization test…\n");
	double sum;
	sum = 0;
	for (unsigned long long i = 0; i < PGST_RNG_ITERATIONS; i++)
		sum += pgr_get_lf() / (double)PGST_RNG_ITERATIONS;
	pgst_show_result_lf("Uniform average", sum, 0.5);
	sum = 0;
	for (unsigned long long i = 0; i < PGST_RNG_ITERATIONS; i++)
		sum += pgr_get_gauss() / (double)PGST_RNG_ITERATIONS;
	pgst_show_result_lf("Gaussian average", sum, 0.0);
}

