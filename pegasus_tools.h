#pragma once

#ifndef PEGASUS_TOOLS_H
#define PEGASUS_TOOLS_H

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
 * pegasus — digital channel simulator, tools file header
 */

#include "pegasus_common_types.h"

unsigned long long pgt_threads;

void pgt_init_threads();
void* pgt_alloc(size_t _items, size_t _item_size);
unsigned long long pgt_blocks_to_blocks(pgs_block_t** _target_blocks,
		pgs_block_t* _source_blocks,
		unsigned long long _source_blocks_count,
		unsigned long long _target_block_size);
unsigned long long pgt_value_to_gray(unsigned long long _value);
double pgt_get_ber(pgs_block_t* _original,
		pgs_block_t* _distorted,
		unsigned long long _length);
double pgt_get_ser(pgs_block_t* _original,
		pgs_block_t* _distorted,
		unsigned long long _blocks_count);
double pgt_dbs_to_times(double _dbs);
unsigned long long pgt_popcount(unsigned long long _value);
double pgt_distance(double _x1, double _y1, double _x2, double _y2);

#endif /* PEGASUS_TOOLS_H */

