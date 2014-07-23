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
 * pegasus — digital channel simulator, modulation file
 */

#include <math.h>
#include <stdlib.h>

#include "pegasus_common_strings.h"
#include "pegasus_common_types.h"

#include "pegasus_block.h"
#include "pegasus_panic.h"
#include "pegasus_tools.h"

#include "pegasus_modulation.h"

static void pgm_fill_xsk(pgs_signal_t* _table,
		unsigned long long _volume,
		double _angle_step,
		double _phase_shift,
		unsigned int _gray_coded,
		unsigned int _self_multiply)
{
	unsigned long long index;
	double multiplier, argument;

	for (unsigned long long i = 0; i < _volume; i++)
	{
		index = _gray_coded == 1 ? pgt_value_to_gray(i) : i;
		multiplier = _self_multiply == 1 ? (double)i : 1.0;
		argument = _phase_shift + (double)i * _angle_step * 2.0;

		_table[index].i = multiplier * cos(argument);
		_table[index].q = multiplier * sin(argument);
	}
}

static void pgm_fill_rectangle_qam(pgs_signal_t* _table,
		double _normalization_factor,
		long long _bound)
{
	unsigned long long index = 0;
	int up = 1;
	for (long long i = -_bound; i <= _bound; i += 2)
	{
		long long
			q = -_bound * up,
			q_end = _bound * up,
			q_step = 2 * up;
		while (1)
		{
			_table[pgt_value_to_gray(index)].i = i * _normalization_factor;
			_table[pgt_value_to_gray(index)].q = q * _normalization_factor;
			index++;
			q += q_step;
			if (unlikely(abs(q) > abs(q_end)))
				break;
		}
		up *= -1;
	}
}

static void pgm_fill_nonrectangle_qam(pgs_signal_t* _table,
		double _normalization_factor,
		long long _bound)
{
	unsigned long long index = 0;
	int up = 1;
	for (long long i = -_bound; i <= _bound; i += 2)
	{
		long long
			q = -_bound * up,
			q_end = _bound * up,
			q_step = 2 * up;
		while (1)
		{
			if (likely(!((i == -_bound && q == -_bound) ||
					(i == -_bound && q == _bound) ||
					(i == _bound && q == -_bound) ||
					(i == _bound && q == _bound))))
			{
				_table[pgt_value_to_gray(index)].i = i * _normalization_factor;
				_table[pgt_value_to_gray(index)].q = q * _normalization_factor;
				index++;
			}
			q += q_step;
			if (unlikely(abs(q) > abs(q_end)))
				break;
		}
		up *= -1;
	}
}

void pgm_init_tables()
{
#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_ask_table, PGM_ASK_VOLUME, PGM_ASK_ANGLE_STEP, PGM_ASK_PHASE_SHIFT, 0, 1);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_fsk_table, PGM_FSK_VOLUME, PGM_FSK_ANGLE_STEP, PGM_FSK_PHASE_SHIFT, 0, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_bpsk_table, PGM_BPSK_VOLUME, PGM_BPSK_ANGLE_STEP, PGM_BPSK_PHASE_SHIFT, 0, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_qpsk_table, PGM_QPSK_VOLUME, PGM_QPSK_ANGLE_STEP, PGM_QPSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_8psk_table, PGM_8PSK_VOLUME, PGM_8PSK_ANGLE_STEP, PGM_8PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_16psk_table, PGM_16PSK_VOLUME, PGM_16PSK_ANGLE_STEP, PGM_16PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_32psk_table, PGM_32PSK_VOLUME, PGM_32PSK_ANGLE_STEP, PGM_32PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_64psk_table, PGM_64PSK_VOLUME, PGM_64PSK_ANGLE_STEP, PGM_64PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_256psk_table, PGM_256PSK_VOLUME, PGM_256PSK_ANGLE_STEP, PGM_256PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_1024psk_table, PGM_1024PSK_VOLUME, PGM_1024PSK_ANGLE_STEP, PGM_1024PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_xsk(pgm_4096psk_table, PGM_4096PSK_VOLUME, PGM_4096PSK_ANGLE_STEP, PGM_4096PSK_PHASE_SHIFT, 1, 0);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_rectangle_qam(pgm_16qam_table, PGM_16QAM_NORMALIZATION, PGM_16QAM_BOUND);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_nonrectangle_qam(pgm_32qam_table, PGM_32QAM_NORMALIZATION, PGM_32QAM_BOUND);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_rectangle_qam(pgm_64qam_table, PGM_64QAM_NORMALIZATION, PGM_64QAM_BOUND);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_rectangle_qam(pgm_256qam_table, PGM_256QAM_NORMALIZATION, PGM_256QAM_BOUND);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_rectangle_qam(pgm_1024qam_table, PGM_1024QAM_NORMALIZATION, PGM_1024QAM_BOUND);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgm_fill_rectangle_qam(pgm_4096qam_table, PGM_4096QAM_NORMALIZATION, PGM_4096QAM_BOUND);
		}
	}
}

char* pgm_to_string(unsigned int _modulation)
{
	switch (_modulation)
	{
		case PGM_ASK:
			return PGM_ASK_STRING;
			break;
		case PGM_FSK:
			return PGM_FSK_STRING;
			break;
		case PGM_BPSK:
			return PGM_BPSK_STRING;
			break;
		case PGM_QPSK:
			return PGM_QPSK_STRING;
			break;
		case PGM_8PSK:
			return PGM_8PSK_STRING;
			break;
		case PGM_16PSK:
			return PGM_16PSK_STRING;
			break;
		case PGM_32PSK:
			return PGM_32PSK_STRING;
			break;
		case PGM_64PSK:
			return PGM_64PSK_STRING;
			break;
		case PGM_256PSK:
			return PGM_256PSK_STRING;
			break;
		case PGM_1024PSK:
			return PGM_1024PSK_STRING;
			break;
		case PGM_4096PSK:
			return PGM_4096PSK_STRING;
			break;
		case PGM_16QAM:
			return PGM_16QAM_STRING;
			break;
		case PGM_32QAM:
			return PGM_32QAM_STRING;
			break;
		case PGM_64QAM:
			return PGM_64QAM_STRING;
			break;
		case PGM_256QAM:
			return PGM_256QAM_STRING;
			break;
		case PGM_1024QAM:
			return PGM_1024QAM_STRING;
			break;
		case PGM_4096QAM:
			return PGM_4096QAM_STRING;
			break;
		default:
			pgp_switch_default();
			return PGS_CS_UNKNOWN;
			break;
	}
}

unsigned long long pgm_get_volume(unsigned int _modulation)
{
	switch (_modulation)
	{
		case PGM_ASK:
			return PGM_ASK_VOLUME;
			break;
		case PGM_FSK:
			return PGM_FSK_VOLUME;
			break;
		case PGM_BPSK:
			return PGM_BPSK_VOLUME;
			break;
		case PGM_QPSK:
			return PGM_QPSK_VOLUME;
			break;
		case PGM_8PSK:
			return PGM_8PSK_VOLUME;
			break;
		case PGM_16PSK:
			return PGM_16PSK_VOLUME;
			break;
		case PGM_32PSK:
			return PGM_32PSK_VOLUME;
			break;
		case PGM_64PSK:
			return PGM_64PSK_VOLUME;
			break;
		case PGM_256PSK:
			return PGM_256PSK_VOLUME;
			break;
		case PGM_1024PSK:
			return PGM_1024PSK_VOLUME;
			break;
		case PGM_4096PSK:
			return PGM_4096PSK_VOLUME;
			break;
		case PGM_16QAM:
			return PGM_16QAM_VOLUME;
			break;
		case PGM_32QAM:
			return PGM_32QAM_VOLUME;
			break;
		case PGM_64QAM:
			return PGM_64QAM_VOLUME;
			break;
		case PGM_256QAM:
			return PGM_256QAM_VOLUME;
			break;
		case PGM_1024QAM:
			return PGM_1024QAM_VOLUME;
			break;
		case PGM_4096QAM:
			return PGM_4096QAM_VOLUME;
			break;
		default:
			pgp_switch_default();
			return PGM_UNKNOWN_VOLUME;
			break;
	}
}

unsigned long long pgm_get_block_size(unsigned int _modulation)
{
	switch (_modulation)
	{
		case PGM_ASK:
			return PGM_ASK_BLOCK_SIZE;
			break;
		case PGM_FSK:
			return PGM_FSK_BLOCK_SIZE;
			break;
		case PGM_BPSK:
			return PGM_BPSK_BLOCK_SIZE;
			break;
		case PGM_QPSK:
			return PGM_QPSK_BLOCK_SIZE;
			break;
		case PGM_8PSK:
			return PGM_8PSK_BLOCK_SIZE;
			break;
		case PGM_16PSK:
			return PGM_16PSK_BLOCK_SIZE;
			break;
		case PGM_32PSK:
			return PGM_32PSK_BLOCK_SIZE;
			break;
		case PGM_64PSK:
			return PGM_64PSK_BLOCK_SIZE;
			break;
		case PGM_256PSK:
			return PGM_256PSK_BLOCK_SIZE;
			break;
		case PGM_1024PSK:
			return PGM_1024PSK_BLOCK_SIZE;
			break;
		case PGM_4096PSK:
			return PGM_4096PSK_BLOCK_SIZE;
			break;
		case PGM_16QAM:
			return PGM_16QAM_BLOCK_SIZE;
			break;
		case PGM_32QAM:
			return PGM_32QAM_BLOCK_SIZE;
			break;
		case PGM_64QAM:
			return PGM_64QAM_BLOCK_SIZE;
			break;
		case PGM_256QAM:
			return PGM_256QAM_BLOCK_SIZE;
			break;
		case PGM_1024QAM:
			return PGM_1024QAM_BLOCK_SIZE;
			break;
		case PGM_4096QAM:
			return PGM_4096QAM_BLOCK_SIZE;
			break;
		default:
			pgp_switch_default();
			return PGM_UNKNOWN_BLOCK_SIZE;
			break;
	}
}

static void pgm_modulate_block(pgs_signal_t* _signal, pgs_block_t* _block, unsigned int _modulation)
{
	pgs_signal_t* current_signal = NULL;
	switch (_modulation)
	{
		case PGM_ASK:
			current_signal = &pgm_ask_table[pgb_block_to_ull(_block)];
			break;
		case PGM_FSK:
			current_signal = &pgm_fsk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_BPSK:
			current_signal = &pgm_bpsk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_QPSK:
			current_signal = &pgm_qpsk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_8PSK:
			current_signal = &pgm_8psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_16PSK:
			current_signal = &pgm_16psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_32PSK:
			current_signal = &pgm_32psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_64PSK:
			current_signal = &pgm_64psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_256PSK:
			current_signal = &pgm_256psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_1024PSK:
			current_signal = &pgm_1024psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_4096PSK:
			current_signal = &pgm_4096psk_table[pgb_block_to_ull(_block)];
			break;
		case PGM_16QAM:
			current_signal = &pgm_16qam_table[pgb_block_to_ull(_block)];
			break;
		case PGM_32QAM:
			current_signal = &pgm_32qam_table[pgb_block_to_ull(_block)];
			break;
		case PGM_64QAM:
			current_signal = &pgm_64qam_table[pgb_block_to_ull(_block)];
			break;
		case PGM_256QAM:
			current_signal = &pgm_256qam_table[pgb_block_to_ull(_block)];
			break;
		case PGM_1024QAM:
			current_signal = &pgm_1024qam_table[pgb_block_to_ull(_block)];
			break;
		case PGM_4096QAM:
			current_signal = &pgm_4096qam_table[pgb_block_to_ull(_block)];
			break;
		default:
			pgp_switch_default();
			break;
	}
	_signal->i = current_signal->i;
	_signal->q = current_signal->q;
}


unsigned long long pgm_modulate(pgs_signal_t** _modulated_signals,
		pgs_block_t* _premodulated_blocks,
		unsigned long long _premodulated_blocks_count,
		unsigned int _modulation)
{
	if (unlikely(_premodulated_blocks == NULL))
		return 0;

	*_modulated_signals = pgt_alloc(_premodulated_blocks_count, sizeof(pgs_signal_t));

#if defined (_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _premodulated_blocks_count; i++)
		pgm_modulate_block(&((pgs_signal_t*)(*_modulated_signals))[i], &_premodulated_blocks[i], _modulation);

	return _premodulated_blocks_count;
}

static void pgm_find_block(pgs_block_t* _block,
		double _real_i,
		double _real_q,
		pgs_signal_t* _table,
		unsigned long long _table_size,
		double _minimum)
{
	unsigned long long min_index = 0;
	double min_distance = pgt_distance(_real_i, _real_q, _table[0].i, _table[0].q);
	for (unsigned long long i = 1; i < _table_size; i++)
	{
		double current_distance = pgt_distance(_real_i, _real_q, _table[i].i, _table[i].q);
		if (unlikely(current_distance < _minimum))
		{
			min_index = i;
			break;
		}
		if (unlikely(current_distance < min_distance))
		{
			min_index = i;
			min_distance = current_distance;
		}
	}
	pgb_ull_to_block(_block, min_index);
}

static void pgm_demodulate_block(pgs_block_t* _demodulated_block, pgs_signal_t* _modulated_signal, unsigned int _modulation)
{
	switch (_modulation)
	{
		case PGM_ASK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_ask_table,
					PGM_ASK_VOLUME,
					PGM_ASK_MINIMUM);
			break;
		case PGM_FSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_fsk_table,
					PGM_FSK_VOLUME,
					PGM_FSK_MINIMUM);
			break;
		case PGM_BPSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_bpsk_table,
					PGM_BPSK_VOLUME,
					PGM_BPSK_MINIMUM);
			break;
		case PGM_QPSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_qpsk_table,
					PGM_QPSK_VOLUME,
					PGM_QPSK_MINIMUM);
			break;
		case PGM_8PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_8psk_table,
					PGM_8PSK_VOLUME,
					PGM_8PSK_MINIMUM);
			break;
		case PGM_16PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_16psk_table,
					PGM_16PSK_VOLUME,
					PGM_16PSK_MINIMUM);
			break;
		case PGM_32PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_32psk_table,
					PGM_32PSK_VOLUME,
					PGM_32PSK_MINIMUM);
			break;
		case PGM_64PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_64psk_table,
					PGM_64PSK_VOLUME,
					PGM_64PSK_MINIMUM);
			break;
		case PGM_256PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_256psk_table,
					PGM_256PSK_VOLUME,
					PGM_256PSK_MINIMUM);
			break;
		case PGM_1024PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_1024psk_table,
					PGM_1024PSK_VOLUME,
					PGM_1024PSK_MINIMUM);
			break;
		case PGM_4096PSK:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_4096psk_table,
					PGM_4096PSK_VOLUME,
					PGM_4096PSK_MINIMUM);
			break;
		case PGM_16QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_16qam_table,
					PGM_16QAM_VOLUME,
					PGM_16QAM_MINIMUM);
			break;
		case PGM_32QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_32qam_table,
					PGM_32QAM_VOLUME,
					PGM_32QAM_MINIMUM);
			break;
		case PGM_64QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_64qam_table,
					PGM_64QAM_VOLUME,
					PGM_64QAM_MINIMUM);
			break;
		case PGM_256QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_256qam_table,
					PGM_256QAM_VOLUME,
					PGM_256QAM_MINIMUM);
			break;
		case PGM_1024QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_1024qam_table,
					PGM_1024QAM_VOLUME,
					PGM_1024QAM_MINIMUM);
			break;
		case PGM_4096QAM:
			pgm_find_block(_demodulated_block,
					_modulated_signal->i,
					_modulated_signal->q,
					pgm_4096qam_table,
					PGM_4096QAM_VOLUME,
					PGM_4096QAM_MINIMUM);
			break;
		default:
			pgp_switch_default();
			break;
	}
}

unsigned long long pgm_demodulate(pgs_block_t** _demodulated_blocks,
		pgs_signal_t* _modulated_signals,
		unsigned long long _modulated_signals_count,
		unsigned int _modulation)
{
	if (unlikely(_modulated_signals == NULL))
		return 0;

	*_demodulated_blocks = pgb_create_blocks(_modulated_signals_count, pgm_get_block_size(_modulation));

#if defined (_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _modulated_signals_count; i++)
		pgm_demodulate_block(&((pgs_block_t*)(*_demodulated_blocks))[i], &_modulated_signals[i], _modulation);

	return _modulated_signals_count;
}

