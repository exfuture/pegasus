#pragma once

#ifndef PEGASUS_RANDOM_H
#define PEGASUS_RANDOM_H

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
 * pegasus — digital channel simulator, random file header
 */

#if defined(_OPENMP)
#include <omp.h>
#endif
#include <stdint.h>

#include "pegasus_common_types.h"

#if defined(_OPENMP)
#define pgr_get_u64() \
	__pgr_get_u64(omp_get_thread_num())
#define pgr_get_lf() \
	__pgr_get_lf(omp_get_thread_num())
#define pgr_get_gauss() \
	__pgr_get_gauss(omp_get_thread_num())
#else
#define pgr_get_u64() \
	__pgr_get_u64(0)
#define pgr_get_lf() \
	__pgr_get_lf(0)
#define pgr_get_gauss() \
	__pgr_get_gauss(0)
#endif

uint64_t* pgr_seed;

void pgr_init_seed();
uint64_t __pgr_get_u64(unsigned long long _thread);
double __pgr_get_lf(unsigned long long _thread);
double __pgr_get_gauss(unsigned long long _thread);

#endif /* PEGASUS_RANDOM_H */

