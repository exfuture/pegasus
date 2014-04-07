#pragma once

#ifndef PEGASUS_FEC_H
#define PEGASUS_FEC_H

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
 * pegasus — digital channel simulator, FEC file header
 */

#include "pegasus_common_types.h"

#define PGF_NONE	1
#define	PGF_HAMMING74	2
#define	PGF_CYCLIC85	3
#define PGF_BCH1557	4
#define PGF_BCH1575	5

#define PGF_NONE_STRING		"No FEC"
#define PGF_HAMMING74_STRING	"Hamming code (7, 4)"
#define PGF_CYCLIC85_STRING	"Cyclic code (8, 5)"
#define PGF_BCH1557_STRING	"BCH code (15, 5, 7)"
#define PGF_BCH1575_STRING	"BCH code (15, 7, 5)"

#define PGF_CYCLIC85_POLYNOME	"1011"
#define PGF_BCH1557_POLYNOME	"10100110111"
#define PGF_BCH1575_POLYNOME	"111010001"

#define PGF_CYCLIC85_TEST	"10101101"
#define PGF_BCH1557_TEST	"101011001000111"
#define PGF_BCH1575_TEST	"101010111100101"

#define PGF_NONE_INPUT_BLOCK_SIZE	0
#define PGF_UNKNOWN_INPUT_BLOCK_SIZE	0
#define PGF_HAMMING74_INPUT_BLOCK_SIZE	4
#define PGF_CYCLIC85_INPUT_BLOCK_SIZE	5
#define PGF_BCH1557_INPUT_BLOCK_SIZE	5
#define PGF_BCH1575_INPUT_BLOCK_SIZE	7

#define PGF_NONE_OUTPUT_BLOCK_SIZE	0
#define PGF_UNKNOWN_OUTPUT_BLOCK_SIZE	0
#define PGF_HAMMING74_OUTPUT_BLOCK_SIZE	7
#define PGF_CYCLIC85_OUTPUT_BLOCK_SIZE	8
#define PGF_BCH1557_OUTPUT_BLOCK_SIZE	15
#define PGF_BCH1575_OUTPUT_BLOCK_SIZE	15

#define PGF_HAMMING74_FEC_SUFFIX	(PGF_HAMMING74_OUTPUT_BLOCK_SIZE - PGF_HAMMING74_INPUT_BLOCK_SIZE)
#define PGF_CYCLIC85_FEC_SUFFIX		(PGF_CYCLIC85_OUTPUT_BLOCK_SIZE - PGF_CYCLIC85_INPUT_BLOCK_SIZE)
#define PGF_BCH1557_FEC_SUFFIX		(PGF_BCH1557_OUTPUT_BLOCK_SIZE - PGF_BCH1557_INPUT_BLOCK_SIZE)
#define PGF_BCH1575_FEC_SUFFIX		(PGF_BCH1575_OUTPUT_BLOCK_SIZE - PGF_BCH1575_INPUT_BLOCK_SIZE)

#define PGF_HAMMING74_FEC_POWER		1
#define PGF_CYCLIC85_FEC_POWER		1
#define PGF_BCH1557_FEC_POWER		3
#define PGF_BCH1575_FEC_POWER		2

#define PGF_CYCLIC85_VOLUME	(1ULL << PGF_CYCLIC85_FEC_SUFFIX)
#define PGF_BCH1557_VOLUME	(1ULL << PGF_BCH1557_FEC_SUFFIX)
#define PGF_BCH1575_VOLUME	(1ULL << PGF_BCH1575_FEC_SUFFIX)

pgs_block_t* pgf_syndromes_cyclic85;
pgs_block_t* pgf_syndromes_bch1557;
pgs_block_t* pgf_syndromes_bch1575;
pgs_block_t* pgf_polynome_cyclic85;
pgs_block_t* pgf_polynome_bch1557;
pgs_block_t* pgf_polynome_bch1575;

void pgf_init_polynomes();
void pgf_done_polynomes();
void pgf_init_syndromes();
void pgf_done_syndromes();
char* pgf_to_string(unsigned int _fec);
unsigned long long pgf_get_input_block_size(unsigned int _fec);
unsigned long long pgf_get_output_block_size(unsigned int _fec);
unsigned long long pgf_encode_blocks(pgs_block_t** _encoded_blocks,
		pgs_block_t* _source_blocks,
		unsigned long long _source_blocks_count,
		unsigned int _fec);
unsigned long long pgf_decode_blocks(pgs_block_t** _decoded_blocks,
		pgs_block_t* _encoded_blocks,
		unsigned long long _encoded_blocks_count,
		unsigned int _fec);

#endif /* PEGASUS_FEC_H */

