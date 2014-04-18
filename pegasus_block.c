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

#include "pegasus_block.h"

pgs_block_t* pgb_create_blocks(unsigned long long _count, unsigned long long _alignment)
{
	pgs_block_t* ret = malloc(sizeof(pgs_block_t) * _count);
	if (unlikely(ret == NULL))
		return NULL;
	for (unsigned long long i = 0; i < _count; i++)
	{
		ret[i].chunk = malloc(sizeof(pgs_bit_t) * _alignment);
		if (unlikely(ret[i].chunk == NULL))
			pgp_malloc();
		for (unsigned long long j = 0; j < _alignment; j++)
			pgb_set_bit(&ret[i], j, 0);
		ret[i].chunk_size = _alignment;
		ret[i].used = 0;
	}
	return ret;
}

pgs_block_t* pgb_create_block(unsigned long long _alignment)
{
	return pgb_create_blocks(1, _alignment);
}

void pgb_destroy_blocks(pgs_block_t* _blocks, unsigned long long _amount)
{
	if (unlikely(_blocks == NULL))
		pgp_null();
	for (unsigned long long i = 0; i < _amount; i++)
	{
		if (unlikely(_blocks[i].chunk == NULL))
			pgp_null();
		free(_blocks[i].chunk);
	}
	free(_blocks);
}

void pgb_destroy_block(pgs_block_t* _block)
{
	pgb_destroy_blocks(_block, 1);
}

inline unsigned int pgb_get_bit(pgs_block_t* _block, unsigned long long _index)
{
	if (unlikely(_block == NULL))
		pgp_null();

	return _block->chunk[_index].bit;
}

inline void pgb_set_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit)
{
	if (unlikely(_block == NULL))
		pgp_null();

	_block->chunk[_index].bit = _bit;
}

inline void pgb_xor_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit)
{
	if (unlikely(_block == NULL))
		pgp_null();

	_block->chunk[_index].bit ^= _bit;
}

inline void pgb_copyxor_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index)

{
	if (unlikely(_target == NULL) || unlikely(_source == NULL))
		pgp_null();

	_target->chunk[_target_index].bit ^= _source->chunk[_source_index].bit;
}

inline void pgb_copy_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index)
{
	if (unlikely(_target == NULL) || unlikely(_source == NULL))
		pgp_null();

	_target->chunk[_target_index].bit = _source->chunk[_source_index].bit;
}

inline void pgb_flip_bit(pgs_block_t* _block, unsigned long long _index)
{
	if (unlikely(_block == NULL))
		pgp_null();

	_block->chunk[_index].bit = ~_block->chunk[_index].bit;
}

inline unsigned int pgb_cmp_bit(pgs_block_t* _block1, unsigned long long _index1,
		pgs_block_t* _block2, unsigned long long _index2)
{
	if (unlikely(_block1 == NULL) || unlikely(_block2 == NULL))
		pgp_null();

	return _block1->chunk[_index1].bit == _block2->chunk[_index2].bit;
}


unsigned long long pgb_block_to_ull(pgs_block_t* _block)
{
	if (unlikely(_block == NULL))
		pgp_null();

	unsigned long long ret = 0;

	for (unsigned long long i = 0; i < _block->chunk_size; i++)
		if (pgb_get_bit(_block, i))
			ret += (1ULL << (_block->chunk_size - i - 1));

	return ret;
}

void pgb_ull_to_block(pgs_block_t* _block,
		unsigned long long _value)
{
	if (unlikely(_block == NULL))
		pgp_null();
	if (unlikely(_block->chunk == NULL))
		pgp_null();

	for (unsigned long long i = 0; i < _block->chunk_size; i++)
		pgb_set_bit(_block, _block->chunk_size - i - 1, (_value & ( 1ULL << i )) >> i);
}

void pgb_binary_string_to_block(pgs_block_t* _block,
		const char* _binary_string)
{
	if (unlikely(_binary_string == NULL))
		pgp_null();

	for (unsigned long long i = 0; i < strlen(_binary_string); i++)
		pgb_set_bit(_block, i, _binary_string[i] == '1' ? 1 : 0);
}

void pgb_divmod2(pgs_block_t** _quotient, pgs_block_t* _dividend, pgs_block_t* _divisor)
{
	pgs_block_t* tmp = pgb_create_block(_dividend->chunk_size);
	if (unlikely(tmp == NULL))
		pgp_null();
	pgb_copy(tmp, 0, _dividend, 0, _dividend->chunk_size);

	for (unsigned long long i = 0; i < _dividend->chunk_size - _divisor->chunk_size + 1; i++)
		if (pgb_get_bit(tmp, i))
			for (unsigned long long j = 0; j < _divisor->chunk_size; j++)
				pgb_copyxor_bit(tmp, i + j, _divisor, j);

	*_quotient = pgb_create_block(_divisor->chunk_size - 1);
	if (unlikely(*_quotient == NULL))
		pgp_null();
	pgb_copy(((pgs_block_t*)(*_quotient)), 0, tmp, _dividend->chunk_size - _divisor->chunk_size + 1, _divisor->chunk_size - 1);
	pgb_destroy_block(tmp);
}

void pgb_xor(pgs_block_t* _target, pgs_block_t* _pattern)
{
	for (unsigned long long i = 0; i < _target->chunk_size; i++)
		pgb_copyxor_bit(_target, i, _pattern, i);
}

unsigned int pgb_multixor(pgs_block_t* _block, unsigned long long* _bits, unsigned long long _length)
{
	if (unlikely(_block == NULL))
		pgp_null();

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
	if (unlikely(_destination == NULL || _source == NULL))
		pgp_null();
	if (unlikely(_destination_index + _amount > _destination->chunk_size ||
			_source_index + _amount > _source->chunk_size))
		pgp_range();

	for (unsigned long long i = _destination_index; i < _destination_index + _amount; i++)
		pgb_copy_bit(_destination, i, _source, _source_index + i - _destination_index);
}

void pgb_show_blocks(pgs_block_t* _block, unsigned long long _count)
{
	if (unlikely(_block == NULL))
		pgp_null();

	for (unsigned long long i = 0; i < _count; i++)
	{
		for (unsigned long long j = 0; j < _block[i].chunk_size; j++)
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

