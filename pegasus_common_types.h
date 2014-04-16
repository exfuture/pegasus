#pragma once

#ifndef PEGASUS_COMMON_TYPES_H
#define PEGASUS_COMMON_TYPES_H

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
 * pegasus — digital channel simulator, common types file header
 */

#include <limits.h>
#include <math.h>
#include <stdint.h>

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else /* __GNUC__ */
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif /* __GNUC__ */

#define PGS_UINT64_SIZE	(sizeof(uint64_t) * CHAR_BIT)
#define PGS_UINT64_MAX 	(pow(2.0, PGS_UINT64_SIZE) - 1)

typedef struct pgs_bit
{
	unsigned int bit:1;
} __attribute__((packed)) pgs_bit_t;

typedef struct pgs_block
{
	pgs_bit_t* chunk;
	unsigned long long chunk_size;
	unsigned int used:1;
} pgs_block_t;

typedef struct pgs_signal
{
	double i, q;
} pgs_signal_t;

#endif /* PEGASUS_COMMON_TYPES_H */

