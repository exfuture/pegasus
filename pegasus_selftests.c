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
#include <stdlib.h>

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
		const char* _subtest_name,
		const char* _result_name,
		double _real_value,
		double _desired_value)
{
	printf("\t[%s, %s, %s] got: %lf, should be: %lf\n", _test_name, _subtest_name, _result_name, _real_value, _desired_value);
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

static double pgst_modulator_test(unsigned int _modulation)
{
	pgs_block_t* source_bits;
	pgs_block_t* source_blocks;
	pgs_signal_t* modulated_signals;
	pgs_block_t* demodulated_blocks;
	pgs_block_t* target_blocks;
	printf("\t%s\n", pgm_to_string(_modulation));
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
				pgm_get_block_size(_modulation));
	if (unlikely(source_blocks_count == 0))
		pgp_nodata();
	printf("\t\tBroken-out sequence: ");
	pgb_show_blocks(source_blocks, source_blocks_count);
	unsigned long long modulated_signals_count =
		pgm_modulate(&modulated_signals,
				source_blocks,
				source_blocks_count,
				_modulation);
	if (unlikely(modulated_signals_count == 0))
		pgp_nodata();
	unsigned long long demodulated_blocks_count =
		pgm_demodulate(&demodulated_blocks,
				modulated_signals,
				modulated_signals_count,
				_modulation);
	if (unlikely(demodulated_blocks_count == 0))
		pgp_nodata();
	printf("\t\tDemodulated sequence: ");
	pgb_show_blocks(demodulated_blocks, demodulated_blocks_count);
	unsigned long long target_blocks_count =
		pgt_blocks_to_blocks(&target_blocks,
				demodulated_blocks,
				demodulated_blocks_count,
				source_bits_count);
	if (unlikely(target_blocks_count == 0))
		pgp_nodata();
	printf("\t\tJoint sequence: ");
	pgb_show_blocks(target_blocks, target_blocks_count);

	double ber = pgt_get_ber(source_bits,
			target_blocks,
			source_bits_count);

	pgb_destroy_blocks(target_blocks, target_blocks_count);
	pgb_destroy_blocks(demodulated_blocks, demodulated_blocks_count);
	free(modulated_signals);
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
	pgst_show_result_lf("RNG", "uniform distribution", "average", sum, 0.5);
	sum = 0;
	for (unsigned long long i = 0; i < PGST_RNG_ITERATIONS; i++)
		sum += pgr_get_gauss() / (double)PGST_RNG_ITERATIONS;
	pgst_show_result_lf("RNG", "Gaussian distribution", "average", sum, 0.0);

	printf("Performing codecs test…\n");
	pgst_show_result_lf("FEC", pgf_to_string(PGF_NONE), "BER", pgst_fec_test(PGF_NONE), 0.0);
	pgst_show_result_lf("FEC", pgf_to_string(PGF_HAMMING74), "BER", pgst_fec_test(PGF_HAMMING74), 0.0);
	pgst_show_result_lf("FEC", pgf_to_string(PGF_CYCLIC85), "BER", pgst_fec_test(PGF_CYCLIC85), 0.0);
	pgst_show_result_lf("FEC", pgf_to_string(PGF_BCH1557), "BER", pgst_fec_test(PGF_BCH1557), 0.0);
	pgst_show_result_lf("FEC", pgf_to_string(PGF_BCH1575), "BER", pgst_fec_test(PGF_BCH1575), 0.0);

	printf("Performing modulators test…\n");
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_ASK), "BER", pgst_modulator_test(PGM_ASK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_FSK), "BER", pgst_modulator_test(PGM_FSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_BPSK), "BER", pgst_modulator_test(PGM_BPSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_QPSK), "BER", pgst_modulator_test(PGM_QPSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_8PSK), "BER", pgst_modulator_test(PGM_8PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_16PSK), "BER", pgst_modulator_test(PGM_16PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_32PSK), "BER", pgst_modulator_test(PGM_32PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_64PSK), "BER", pgst_modulator_test(PGM_64PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_256PSK), "BER", pgst_modulator_test(PGM_256PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_1024PSK), "BER", pgst_modulator_test(PGM_1024PSK), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_16QAM), "BER", pgst_modulator_test(PGM_16QAM), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_32QAM), "BER", pgst_modulator_test(PGM_32QAM), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_64QAM), "BER", pgst_modulator_test(PGM_64QAM), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_256QAM), "BER", pgst_modulator_test(PGM_256QAM), 0.0);
	pgst_show_result_lf("Modulator", pgm_to_string(PGM_1024QAM), "BER", pgst_modulator_test(PGM_1024QAM), 0.0);
}

