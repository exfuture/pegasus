#pragma once

#ifndef PEGASUS_SOURCE_H
#define PEGASUS_SOURCE_H

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
 * pegasus — digital channel simulator, source file header
 */

#include "pegasus_common_types.h"

#define PGS_RANDOM	1
#define PGS_PREDEFINED	2

#define PGS_RANDOM_STRING	"Random bits sequence"
#define PGS_PREDEFINED_STRING	"Predefined bits sequence"

char* pgs_source_to_string(unsigned int _source);
unsigned long long pgs_generate(pgs_block_t** _data_buffer,
		unsigned int _source,
		unsigned int _modulation,
		unsigned long long _amount);

#endif /* PEGASUS_SOURCE_H */

