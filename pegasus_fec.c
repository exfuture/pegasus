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
 * pegasus — digital channel simulator, FEC file
 */

#include <stdlib.h>

#include "pegasus_common_strings.h"
#include "pegasus_common_types.h"

#include "pegasus_block.h"
#include "pegasus_panic.h"
#include "pegasus_tools.h"

#include "pegasus_fec.h"

void pgf_init_polynomes()
{
#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_polynome_cyclic85 = pgb_create_block(PGF_CYCLIC85_FEC_SUFFIX + 1);
			if (unlikely(pgf_polynome_cyclic85 == NULL))
				pgp_null();
			pgb_binary_string_to_block(pgf_polynome_cyclic85, PGF_CYCLIC85_POLYNOME);
		}

#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_polynome_bch1557 = pgb_create_block(PGF_BCH1557_FEC_SUFFIX + 1);
			if (unlikely(pgf_polynome_bch1557 == NULL))
				pgp_null();
			pgb_binary_string_to_block(pgf_polynome_bch1557, PGF_BCH1557_POLYNOME);
		}

#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_polynome_bch1575 = pgb_create_block(PGF_BCH1575_FEC_SUFFIX + 1);
			if (unlikely(pgf_polynome_bch1575 == NULL))
				pgp_null();
			pgb_binary_string_to_block(pgf_polynome_bch1575, PGF_BCH1575_POLYNOME);
		}
	}
}

void pgf_done_polynomes()
{
#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_block(pgf_polynome_cyclic85);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_block(pgf_polynome_bch1557);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_block(pgf_polynome_bch1575);
		}
	}
}

static void pgf_examine_error_vectors(pgs_block_t** _syndromes_table,
		unsigned long long _block_size,
		unsigned long long _fec_suffix,
		unsigned long long _errors_count,
		pgs_block_t* _fec_polynome,
		const char* _test_sequence)
{
	*_syndromes_table = pgb_create_blocks(_fec_suffix, _block_size);
	pgs_block_t* test_message = pgb_create_block(_block_size);
	pgs_block_t* test_error = pgb_create_block(_block_size);
	pgs_block_t* test_syndrome;
	pgb_binary_string_to_block(test_message, _test_sequence);
	for (unsigned long long i = (1ULL << _block_size) - 1; i > 0; i--)
		if (unlikely(pgt_popcount(i) <= _errors_count))
		{
			pgb_ull_to_block(test_error, i);
			pgb_xor(test_message, test_error);

			pgb_divmod2(&test_syndrome, test_message, _fec_polynome);
			unsigned long long index = pgb_block_to_ull(test_syndrome);
			(*_syndromes_table)[index].used = 1;
			pgb_copy(&(*_syndromes_table)[index], 0, test_error, 0, _block_size);
			pgb_destroy_block(test_syndrome);

			pgb_xor(test_message, test_error);
		}
	pgb_destroy_block(test_error);
	pgb_destroy_block(test_message);
}

void pgf_init_syndromes()
{
#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_examine_error_vectors(&pgf_syndromes_cyclic85,
					PGF_CYCLIC85_OUTPUT_BLOCK_SIZE,
					PGF_CYCLIC85_VOLUME,
					PGF_CYCLIC85_FEC_POWER,
					pgf_polynome_cyclic85,
					PGF_CYCLIC85_TEST);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_examine_error_vectors(&pgf_syndromes_bch1557,
					PGF_BCH1557_OUTPUT_BLOCK_SIZE,
					PGF_BCH1557_VOLUME,
					PGF_BCH1557_FEC_POWER,
					pgf_polynome_bch1557,
					PGF_BCH1557_TEST);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgf_examine_error_vectors(&pgf_syndromes_bch1575,
					PGF_BCH1575_OUTPUT_BLOCK_SIZE,
					PGF_BCH1575_VOLUME,
					PGF_BCH1575_FEC_POWER,
					pgf_polynome_bch1575,
					PGF_BCH1575_TEST);
		}
	}
}

void pgf_done_syndromes()
{
#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgf_syndromes_cyclic85, PGF_CYCLIC85_VOLUME);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgf_syndromes_bch1557, PGF_BCH1557_VOLUME);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgf_syndromes_bch1575, PGF_BCH1575_VOLUME);
		}
	}
}

char* pgf_to_string(unsigned int _fec)
{
	switch (_fec)
	{
		case PGF_NONE:
			return PGF_NONE_STRING;
			break;
		case PGF_HAMMING74:
			return PGF_HAMMING74_STRING;
			break;
		case PGF_CYCLIC85:
			return PGF_CYCLIC85_STRING;
			break;
		case PGF_BCH1557:
			return PGF_BCH1557_STRING;
			break;
		case PGF_BCH1575:
			return PGF_BCH1575_STRING;
			break;
		default:
			pgp_switch_default();
			return PGS_CS_UNKNOWN;
			break;
	}
}

unsigned long long pgf_get_input_block_size(unsigned int _fec)
{
	switch (_fec)
	{
		case PGF_NONE:
			return PGF_NONE_INPUT_BLOCK_SIZE;
			break;
		case PGF_HAMMING74:
			return PGF_HAMMING74_INPUT_BLOCK_SIZE;
			break;
		case PGF_CYCLIC85:
			return PGF_CYCLIC85_INPUT_BLOCK_SIZE;
			break;
		case PGF_BCH1557:
			return PGF_BCH1557_INPUT_BLOCK_SIZE;
			break;
		case PGF_BCH1575:
			return PGF_BCH1575_INPUT_BLOCK_SIZE;
			break;
		default:
			pgp_switch_default();
			return PGF_UNKNOWN_INPUT_BLOCK_SIZE;
			break;
	}
}

unsigned long long pgf_get_output_block_size(unsigned int _fec)
{
	switch (_fec)
	{
		case PGF_NONE:
			return PGF_NONE_OUTPUT_BLOCK_SIZE;
			break;
		case PGF_HAMMING74:
			return PGF_HAMMING74_OUTPUT_BLOCK_SIZE;
			break;
		case PGF_CYCLIC85:
			return PGF_CYCLIC85_OUTPUT_BLOCK_SIZE;
			break;
		case PGF_BCH1557:
			return PGF_BCH1557_OUTPUT_BLOCK_SIZE;
			break;
		case PGF_BCH1575:
			return PGF_BCH1575_OUTPUT_BLOCK_SIZE;
			break;
		default:
			pgp_switch_default();
			return PGF_UNKNOWN_OUTPUT_BLOCK_SIZE;
			break;
	}
}

static void pgf_encode_block_hamming74(pgs_block_t* _encoded_block,
		pgs_block_t* _source_block)
{
	pgb_copy(_encoded_block, 0, _source_block, 0, PGF_HAMMING74_INPUT_BLOCK_SIZE);
	for (unsigned long long i = 0; i < PGF_HAMMING74_FEC_SUFFIX; i++)
		pgb_set_bit(_encoded_block, PGF_HAMMING74_INPUT_BLOCK_SIZE + i,
			pgb_multixor(_source_block,
					PGF_HAMMING74_FEC_BITS[i],
					PGF_HAMMING74_FEC_SUFFIX));
}

static void pgf_encode_block_crc(pgs_block_t* _encoded_block,
		pgs_block_t* _source_block,
		pgs_block_t* _polynome,
		unsigned long long _input_block_size,
		unsigned long long _fec_suffix)
{
	pgs_block_t* syndrome;
	pgb_copy(_encoded_block, 0, _source_block, 0, _input_block_size);
	pgb_divmod2(&syndrome, _encoded_block, _polynome);
	pgb_copy(_encoded_block, _input_block_size, syndrome, 0, _fec_suffix);
	pgb_destroy_block(syndrome);
}

static void pgf_encode_block(pgs_block_t* _encoded_block,
		pgs_block_t* _source_block,
		unsigned long long _fec)
{
	if (unlikely(_encoded_block == NULL || _source_block == NULL))
		pgp_null();

	switch (_fec)
	{
		case PGF_NONE:
			pgb_copy(_encoded_block, 0, _source_block, 0, _source_block->bits_count);
			break;
		case PGF_HAMMING74:
			pgf_encode_block_hamming74(_encoded_block, _source_block);
			break;
		case PGF_CYCLIC85:
			pgf_encode_block_crc(_encoded_block,
					_source_block,
					pgf_polynome_cyclic85,
					PGF_CYCLIC85_INPUT_BLOCK_SIZE,
					PGF_CYCLIC85_FEC_SUFFIX);
			break;
		case PGF_BCH1557:
			pgf_encode_block_crc(_encoded_block,
					_source_block,
					pgf_polynome_bch1557,
					PGF_BCH1557_INPUT_BLOCK_SIZE,
					PGF_BCH1557_FEC_SUFFIX);
			break;
		case PGF_BCH1575:
			pgf_encode_block_crc(_encoded_block,
					_source_block,
					pgf_polynome_bch1575,
					PGF_BCH1575_INPUT_BLOCK_SIZE,
					PGF_BCH1575_FEC_SUFFIX);
			break;
		default:
			pgp_switch_default();
			break;
	}
}

unsigned long long pgf_encode_blocks(pgs_block_t** _encoded_blocks,
		pgs_block_t* _source_blocks,
		unsigned long long _source_blocks_count,
		unsigned int _fec)
{
	if (unlikely(_source_blocks == NULL))
		return 0;

	*_encoded_blocks = pgb_create_blocks(_source_blocks_count, _fec == PGF_NONE ? _source_blocks[0].bits_count : pgf_get_output_block_size(_fec));
	if (unlikely(*_encoded_blocks == NULL))
		pgp_null();

#if defined (_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _source_blocks_count; i++)
		pgf_encode_block(&((pgs_block_t*)(*_encoded_blocks))[i],
				&_source_blocks[i], _fec);

	return _source_blocks_count;
}

static void pgf_decode_block_hamming74(pgs_block_t* _decoded_block,
		pgs_block_t* _encoded_block)
{
	unsigned long long wrong_bit = 0;
	pgs_block_t* syndrome_hamming74 = pgb_create_block(PGF_HAMMING74_FEC_SUFFIX);
	if (unlikely(syndrome_hamming74 == NULL))
		pgp_null();
	for (unsigned long long i = 0; i < PGF_HAMMING74_FEC_SUFFIX; i++)
		pgb_set_bit(syndrome_hamming74, i,
			pgb_multixor(_encoded_block,
					PGF_HAMMING74_SYNDROME_BITS[i],
					PGF_HAMMING74_INPUT_BLOCK_SIZE));
	wrong_bit = pgb_block_to_ull(syndrome_hamming74);
	pgb_copy(_decoded_block, 0, _encoded_block, 0, PGF_HAMMING74_INPUT_BLOCK_SIZE);
	if (unlikely(wrong_bit > 0 && wrong_bit <= PGF_HAMMING74_INPUT_BLOCK_SIZE))
		pgb_flip_bit(_decoded_block, wrong_bit - 1);
	pgb_destroy_block(syndrome_hamming74);
}

static void pgf_decode_block_crc(pgs_block_t* _decoded_block,
		pgs_block_t* _encoded_block,
		pgs_block_t* _syndromes_table,
		pgs_block_t* _polynome,
		unsigned long long _input_block_size)
{
	pgs_block_t* syndrome;
	pgb_copy(_decoded_block, 0, _encoded_block, 0, _input_block_size);
	pgb_divmod2(&syndrome, _encoded_block, _polynome);
	unsigned long long syndrome_ull = pgb_block_to_ull(syndrome);
	if (likely(syndrome_ull > 0))
		if (unlikely(_syndromes_table[syndrome_ull].used))
			pgb_xor(_decoded_block, &_syndromes_table[syndrome_ull]);
	pgb_destroy_block(syndrome);
}

static void pgf_decode_block(pgs_block_t* _decoded_block,
		pgs_block_t* _encoded_block,
		unsigned long long _fec)
{
	if (unlikely(_decoded_block == NULL || _encoded_block == NULL))
		pgp_null();

	switch (_fec)
	{
		case PGF_NONE:
			pgb_copy(_decoded_block, 0, _encoded_block, 0, _encoded_block->bits_count);
			break;
		case PGF_HAMMING74:
			pgf_decode_block_hamming74(_decoded_block, _encoded_block);
			break;
		case PGF_CYCLIC85:
			pgf_decode_block_crc(_decoded_block,
					_encoded_block,
					pgf_syndromes_cyclic85,
					pgf_polynome_cyclic85,
					PGF_CYCLIC85_INPUT_BLOCK_SIZE);
			break;
		case PGF_BCH1557:
			pgf_decode_block_crc(_decoded_block,
					_encoded_block,
					pgf_syndromes_bch1557,
					pgf_polynome_bch1557,
					PGF_BCH1557_INPUT_BLOCK_SIZE);
			break;
		case PGF_BCH1575:
			pgf_decode_block_crc(_decoded_block,
					_encoded_block,
					pgf_syndromes_bch1575,
					pgf_polynome_bch1575,
					PGF_BCH1575_INPUT_BLOCK_SIZE);
			break;
		default:
			pgp_switch_default();
			break;
	}
}

unsigned long long pgf_decode_blocks(pgs_block_t** _decoded_blocks,
		pgs_block_t* _encoded_blocks,
		unsigned long long _encoded_blocks_count,
		unsigned int _fec)
{
	if (unlikely(_encoded_blocks == NULL))
		return 0;

	*_decoded_blocks = pgb_create_blocks(_encoded_blocks_count, _fec == PGF_NONE ? _encoded_blocks[0].bits_count : pgf_get_input_block_size(_fec));
	if (unlikely(*_decoded_blocks == NULL))
		pgp_null();

#if defined (_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _encoded_blocks_count; i++)
		pgf_decode_block(&((pgs_block_t*)(*_decoded_blocks))[i],
				&_encoded_blocks[i], _fec);

	return _encoded_blocks_count;

}

