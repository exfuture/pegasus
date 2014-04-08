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
 * pegasus — digital channel simulator, selftests file
 */

#include <stdio.h>

#include "pegasus_common_types.h"

#include "pegasus_block.h"
#include "pegasus_fec.h"
#include "pegasus_modulation.h"
#include "pegasus_panic.h"
#include "pegasus_random.h"
#include "pegasus_source.h"
#include "pegasus_tools.h"

#include "pegasus_selftests.h"

static void pgst_show_result_lf(const char* _test_name,
		double _real_value,
		double _desired_value)
{
	printf("\t[%s] got: %lf, should be: %lf\n", _test_name, _real_value, _desired_value);
}

static double pgst_fec_test(unsigned int _fec)
{
	pgs_block_t* source_bits;
	pgs_block_t* source_blocks;
	pgs_block_t* encoded_blocks;
	pgs_block_t* decoded_blocks;
	pgs_block_t* target_blocks;
	printf("\t%s\n", pgf_to_string(_fec));
	unsigned long long source_bits_count =
		pgs_generate(&source_bits,
				PGS_RANDOM,
				PGM_QPSK,
				PGST_SOURCE_LENGTH);
	if (unlikely(source_bits_count == 0))
		pgp_nodata();
	printf("\t\tGenerated sequence: ");
	pgb_show_block(source_bits);
	unsigned long long source_blocks_count =
		pgt_blocks_to_blocks(&source_blocks,
				source_bits,
				1,
				pgf_get_input_block_size(_fec));
	if (unlikely(source_blocks_count == 0))
		pgp_nodata();
	printf("\t\tBroken-out sequence: ");
	pgb_show_blocks(source_blocks, source_blocks_count);
	unsigned long long encoded_blocks_count =
		pgf_encode_blocks(&encoded_blocks,
				source_blocks,
				source_blocks_count,
				_fec);
	if (unlikely(encoded_blocks_count == 0))
		pgp_nodata();
	printf("\t\tEncoded sequence: ");
	pgb_show_blocks(encoded_blocks, encoded_blocks_count);
	unsigned long long decoded_blocks_count =
		pgf_decode_blocks(&decoded_blocks,
				encoded_blocks,
				encoded_blocks_count,
				_fec);
	if (unlikely(decoded_blocks_count == 0))
		pgp_nodata();
	printf("\t\tDecoded sequence: ");
	pgb_show_blocks(decoded_blocks, decoded_blocks_count);
	unsigned long long target_blocks_count =
		pgt_blocks_to_blocks(&target_blocks,
				decoded_blocks,
				decoded_blocks_count,
				source_bits_count);
	if (unlikely(target_blocks_count == 0))
		pgp_nodata();
	printf("\t\tJoint sequence: ");
	pgb_show_blocks(target_blocks, target_blocks_count);

	double ber = pgt_get_ber(source_bits,
			target_blocks,
			source_bits_count);

	pgb_destroy_blocks(target_blocks, target_blocks_count);
	pgb_destroy_blocks(decoded_blocks, decoded_blocks_count);
	pgb_destroy_blocks(encoded_blocks, encoded_blocks_count);
	pgb_destroy_blocks(source_blocks, source_blocks_count);
	pgb_destroy_block(source_bits);

	return ber;
}

void pgst_run_tests()
{
	printf("Performing RNG normalization test…\n");
	double sum;
	sum = 0;
	for (unsigned long long i = 0; i < PGST_RNG_ITERATIONS; i++)
		sum += pgr_get_lf() / (double)PGST_RNG_ITERATIONS;
	pgst_show_result_lf("Uniform average", sum, 0.5);
	sum = 0;
	for (unsigned long long i = 0; i < PGST_RNG_ITERATIONS; i++)
		sum += pgr_get_gauss() / (double)PGST_RNG_ITERATIONS;
	pgst_show_result_lf("Gaussian average", sum, 0.0);

	printf("Performing codecs test…\n");
	pgst_show_result_lf(pgf_to_string(PGF_HAMMING74), pgst_fec_test(PGF_HAMMING74), 0.0);
	pgst_show_result_lf(pgf_to_string(PGF_CYCLIC85), pgst_fec_test(PGF_CYCLIC85), 0.0);
	pgst_show_result_lf(pgf_to_string(PGF_BCH1557), pgst_fec_test(PGF_BCH1557), 0.0);
	pgst_show_result_lf(pgf_to_string(PGF_BCH1575), pgst_fec_test(PGF_BCH1575), 0.0);
}

