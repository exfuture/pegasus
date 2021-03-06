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
 * pegasus — digital channel simulator, block file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pegasus_common_types.h"

#include "pegasus_panic.h"
#include "pegasus_tools.h"

#include "pegasus_block.h"

pgs_block_t* pgb_create_blocks(unsigned long long _count, unsigned long long _alignment)
{
	pgs_block_t* ret = pgt_alloc(_count, sizeof(pgs_block_t));
	for (unsigned long long i = 0; i < _count; i++)
	{
		ret[i].chunk = pgt_alloc(_alignment, sizeof(pgs_bit_t));
		ret[i].bits_count = _alignment;
	}
	return ret;
}

pgs_block_t* pgb_create_block(unsigned long long _alignment)
{
	return pgb_create_blocks(1, _alignment);
}

void pgb_destroy_blocks(pgs_block_t* _blocks, unsigned long long _amount)
{
	for (unsigned long long i = 0; i < _amount; i++)
		free(_blocks[i].chunk);
	free(_blocks);
}

void pgb_destroy_block(pgs_block_t* _block)
{
	pgb_destroy_blocks(_block, 1);
}

inline unsigned int pgb_get_bit(pgs_block_t* _block, unsigned long long _index)
{
	return _block->chunk[_index].bit;
}

inline void pgb_set_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit)
{
	_block->chunk[_index].bit = _bit;
}

inline void pgb_xor_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit)
{
	_block->chunk[_index].bit ^= _bit;
}

inline void pgb_copyxor_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index)

{
	_target->chunk[_target_index].bit ^= _source->chunk[_source_index].bit;
}

inline void pgb_copy_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index)
{
	_target->chunk[_target_index].bit = _source->chunk[_source_index].bit;
}

inline void pgb_flip_bit(pgs_block_t* _block, unsigned long long _index)
{
	_block->chunk[_index].bit = ~_block->chunk[_index].bit;
}

inline unsigned int pgb_cmp_bit(pgs_block_t* _block1, unsigned long long _index1,
		pgs_block_t* _block2, unsigned long long _index2)
{
	return _block1->chunk[_index1].bit == _block2->chunk[_index2].bit;
}


unsigned long long pgb_block_to_ull(pgs_block_t* _block)
{
	unsigned long long ret = 0;

	for (unsigned long long i = 0; i < _block->bits_count; i++)
		if (pgb_get_bit(_block, i))
			ret += (1ULL << (_block->bits_count - i - 1));

	return ret;
}

void pgb_ull_to_block(pgs_block_t* _block,
		unsigned long long _value)
{
	for (unsigned long long i = 0; i < _block->bits_count; i++)
		pgb_set_bit(_block, _block->bits_count - i - 1, (_value & ( 1ULL << i )) >> i);
}

void pgb_binary_string_to_block(pgs_block_t* _block,
		const char* _binary_string)
{
	for (unsigned long long i = 0; i < strlen(_binary_string); i++)
		pgb_set_bit(_block, i, _binary_string[i] == '1' ? 1 : 0);
}

void pgb_divmod2(pgs_block_t** _quotient, pgs_block_t* _dividend, pgs_block_t* _divisor)
{
	pgs_block_t* tmp = pgb_create_block(_dividend->bits_count);
	pgb_copy(tmp, 0, _dividend, 0, _dividend->bits_count);

	for (unsigned long long i = 0; i < _dividend->bits_count - _divisor->bits_count + 1; i++)
		if (pgb_get_bit(tmp, i))
			for (unsigned long long j = 0; j < _divisor->bits_count; j++)
				pgb_copyxor_bit(tmp, i + j, _divisor, j);

	*_quotient = pgb_create_block(_divisor->bits_count - 1);
	pgb_copy(((pgs_block_t*)(*_quotient)), 0, tmp, _dividend->bits_count - _divisor->bits_count + 1, _divisor->bits_count - 1);
	pgb_destroy_block(tmp);
}

void pgb_xor(pgs_block_t* _target, pgs_block_t* _pattern)
{
	for (unsigned long long i = 0; i < _target->bits_count; i++)
		pgb_copyxor_bit(_target, i, _pattern, i);
}

unsigned int pgb_multixor(pgs_block_t* _block, unsigned long long* _bits, unsigned long long _length)
{
	unsigned long long ret = 0;
	for (unsigned long long i = 0; i < _length; i++)
		ret ^= pgb_get_bit(_block, _bits[i]);
	return ret;
}

void pgb_copy(pgs_block_t* _destination,
		unsigned long long _destination_index,
		pgs_block_t* _source,
		unsigned long long _source_index,
		unsigned long long _amount)
{
	if (unlikely(_destination_index + _amount > _destination->bits_count ||
			_source_index + _amount > _source->bits_count))
		pgp_range();

	for (unsigned long long i = _destination_index; i < _destination_index + _amount; i++)
		pgb_copy_bit(_destination, i, _source, _source_index + i - _destination_index);
}

void pgb_show_blocks(pgs_block_t* _block, unsigned long long _count)
{
	for (unsigned long long i = 0; i < _count; i++)
	{
		for (unsigned long long j = 0; j < _block[i].bits_count; j++)
			printf("%u", pgb_get_bit(&_block[i], j));
		if (likely(_count > 1 && i < _count - 1))
			printf(" ");
		else
			printf("\n");
	}
}

void pgb_show_block(pgs_block_t* _block)
{
	pgb_show_blocks(_block, 1);
}

