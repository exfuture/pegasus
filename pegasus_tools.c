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
 * pegasus — digital channel simulator, tools file
 */

#include <math.h>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <stdlib.h>

#include "pegasus_common_types.h"

#include "pegasus_block.h"
#include "pegasus_panic.h"

#include "pegasus_tools.h"

void pgt_init_threads()
{
#if defined(_OPENMP)
	pgt_threads = omp_get_max_threads();
#else
	pgt_threads = 1;
#endif
}

void* pgt_alloc(size_t _items, size_t _item_size)
{
	void* pointer = calloc(_items, _item_size);
	if (unlikely(pointer == NULL))
		pgp_malloc();

	return pointer;
}

unsigned long long pgt_blocks_to_blocks(pgs_block_t** _target_blocks,
		pgs_block_t* _source_blocks,
		unsigned long long _source_blocks_count,
		unsigned long long _target_block_size)
{
	if (unlikely(_source_blocks == NULL))
		return 0;

	unsigned long long source_block_size = _source_blocks[0].bits_count;

	if (unlikely(_target_block_size == 0))
		_target_block_size = source_block_size;

	unsigned long long target_blocks_count = _source_blocks_count * source_block_size / _target_block_size;
	if ((_source_blocks_count * source_block_size) % _target_block_size != 0)
		target_blocks_count++;

	*_target_blocks = pgb_create_blocks(target_blocks_count, _target_block_size);

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _source_blocks_count; i++)
	{
		unsigned long long source_bits_passed = i * source_block_size;
		for (unsigned long long j = 0; j < source_block_size; j++)
		{
			unsigned long long source_bit_serial_index = source_bits_passed + j;
			unsigned long long target_block_index = source_bit_serial_index / _target_block_size;
			unsigned long long target_bit_index = source_bit_serial_index % _target_block_size;
			pgb_copy_bit(&((pgs_block_t*)(*_target_blocks))[target_block_index], target_bit_index, &_source_blocks[i], j);
		}
	}

	return target_blocks_count;
}

unsigned long long pgt_value_to_gray(unsigned long long _value)
{
	return (_value >> 1ULL) ^ _value;
}

double pgt_get_ber(pgs_block_t* _original,
		pgs_block_t* _distorted,
		unsigned long long _length)
{
	if (unlikely(_original == NULL || _distorted == NULL))
		return 0;

	unsigned long long wrong_bits = 0;

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _length; i++)
		if (!pgb_cmp_bit(&_original[0], i, &_distorted[0], i))
#if defined(_OPENMP)
#pragma omp atomic
#endif
			wrong_bits++;

	return (double)wrong_bits / (double)_length;
}

double pgt_get_ser(pgs_block_t* _original,
		pgs_block_t* _distorted,
		unsigned long long _blocks_count)
{
	if (unlikely(_original == NULL || _distorted == NULL))
		return 0;

	unsigned long long wrong_blocks = 0;

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _blocks_count; i++)
	{
		unsigned long long wrong_bits = 0;
		for (unsigned long long j = 0; j < _original[i].bits_count; j++)
			if (!pgb_cmp_bit(&_original[i], j, &_distorted[i], j))
				wrong_bits++;
		if (wrong_bits > 0)
#if defined(_OPENMP)
#pragma omp atomic
#endif
			wrong_blocks++;
	}

	return (double)wrong_blocks / (double)_blocks_count;
}

double pgt_dbs_to_times(double _dbs)
{
	return pow(10.0, _dbs / 10.0);
}

unsigned long long pgt_popcount(unsigned long long _value)
{
#ifdef __GNUC__
	return __builtin_popcount(_value);
#else /* __GNUC__ */
	unsigned long long ret = 0;
	for (unsigned long long i = 0; i < sizeof(unsigned long long) * CHAR_BIT; i++)
		if ((_value >> i) & 1ULL)
			ret++;
	return ret;
#endif /* __GNUC__ */
}

inline double pgt_distance(double _x1, double _y1, double _x2, double _y2)
{
	return sqrt(pow(_x1 - _x2, 2.0) + pow(_y1 - _y2, 2.0));
}

