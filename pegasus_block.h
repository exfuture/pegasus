#pragma once

#ifndef PEGASUS_BLOCK_H
#define PEGASUS_BLOCK_H

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
 * pegasus — digital channel simulator, block file header
 */

#include "pegasus_common_types.h"

pgs_block_t* pgb_create_blocks(unsigned long long _count, unsigned long long _alignment);
pgs_block_t* pgb_create_block(unsigned long long _alignment);
void pgb_destroy_block(pgs_block_t* _block);
void pgb_destroy_blocks(pgs_block_t* _blocks, unsigned long long _amount);
unsigned int pgb_get_bit(pgs_block_t* _block, unsigned long long _index);
void pgb_set_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit);
void pgb_xor_bit(pgs_block_t* _block, unsigned long long _index, unsigned int _bit);
void pgb_copyxor_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index);
void pgb_copy_bit(pgs_block_t* _target, unsigned long long _target_index,
		pgs_block_t* _source, unsigned long long _source_index);
void pgb_flip_bit(pgs_block_t* _block, unsigned long long _index);
unsigned int pgb_cmp_bit(pgs_block_t* _block1, unsigned long long _index1,
		pgs_block_t* _block2, unsigned long long _index2);
unsigned long long pgb_block_to_ull(pgs_block_t* _block);
void pgb_ull_to_block(pgs_block_t* _block,
		unsigned long long _value);
void pgb_binary_string_to_block(pgs_block_t* _block,
		const char* _binary_string);
void pgb_divmod2(pgs_block_t** _quotient, pgs_block_t* _dividend, pgs_block_t* _divisor);
void pgb_xor(pgs_block_t* _target, pgs_block_t* _pattern);
unsigned int pgb_multixor(pgs_block_t* _block, unsigned long long* _bits, unsigned long long _length);
void pgb_copy(pgs_block_t* _destination,
		unsigned long long _destination_index,
		pgs_block_t* _source,
		unsigned long long _source_index,
		unsigned long long _amount);
void pgb_show_blocks(pgs_block_t* _block, unsigned long long _count);
void pgb_show_block(pgs_block_t* _block);

#endif /* PEGASUS_BLOCK_H */

