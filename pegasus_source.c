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
 * pegasus — digital channel simulator, source file
 */

#if defined(_OPENMP)
#include <omp.h>
#endif
#include <stdlib.h>

#include "pegasus_common_strings.h"
#include "pegasus_common_types.h"

#include "pegasus_block.h"
#include "pegasus_modulation.h"
#include "pegasus_panic.h"
#include "pegasus_random.h"
#include "pegasus_tools.h"

#include "pegasus_source.h"

char* pgs_source_to_string(unsigned int _source)
{

	switch (_source)
	{
		case PGS_RANDOM:
			return PGS_RANDOM_STRING;
			break;
		case PGS_PREDEFINED:
			return PGS_PREDEFINED_STRING;
			break;
		default:
			pgp_switch_default();
			return PGS_CS_UNKNOWN;
			break;
	}
}

static void pgs_generate_random(pgs_block_t* _data_buffer,
		unsigned long long _amount)
{
	unsigned long long extra_bits = _amount % PGS_UINT64_SIZE;
	unsigned long long bits_count = _amount - extra_bits;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < bits_count; i += PGS_UINT64_SIZE)
	{
		pgs_block_t* current_block = pgb_create_block(PGS_UINT64_SIZE);
		unsigned long long current_ull = pgr_get_u64();
		pgb_ull_to_block(current_block, current_ull);
		pgb_copy(&_data_buffer[0], i, current_block, 0, PGS_UINT64_SIZE);
		pgb_destroy_block(current_block);
	}
	if (extra_bits > 0)
	{
		pgs_block_t* current_block = pgb_create_block(PGS_UINT64_SIZE);
		unsigned long long current_ull = pgr_get_u64();
		pgb_ull_to_block(current_block, current_ull);
		pgb_copy(&_data_buffer[0], bits_count, current_block, 0, extra_bits);
		pgb_destroy_block(current_block);
	}
}

static void pgs_generate_predefined(pgs_block_t* _data_buffer,
		unsigned int _modulation,
		unsigned long long _amount)
{
	unsigned long long modulation_block_size = pgm_get_block_size(_modulation);
	unsigned long long extra_bits = _amount % modulation_block_size;
	unsigned long long current_index = 0;
	pgs_block_t* current_block = pgb_create_block(modulation_block_size);
	for (unsigned long long i = 0; i < _amount - extra_bits; i += modulation_block_size)
	{
		pgb_ull_to_block(current_block, current_index);
		pgb_copy(&_data_buffer[0], i, current_block, 0, modulation_block_size);

		current_index++;
		if (unlikely(current_index >= pgm_get_volume(_modulation)))
			current_index = 0;
	}
	if (extra_bits > 0)
	{
		pgb_ull_to_block(current_block, current_index);
		pgb_copy(&_data_buffer[0], _amount - extra_bits, current_block, 0, extra_bits);
	}
	pgb_destroy_block(current_block);
}

unsigned long long pgs_generate(pgs_block_t** _data_buffer,
		unsigned int _source,
		unsigned int _modulation,
		unsigned long long _amount)
{
	*_data_buffer = pgb_create_block(_amount);

	switch (_source)
	{
		case PGS_RANDOM:
			pgs_generate_random(*_data_buffer, _amount);
			break;
		case PGS_PREDEFINED:
			pgs_generate_predefined(*_data_buffer, _modulation, _amount);
			break;
		default:
			break;
	}

	return _amount;
}

