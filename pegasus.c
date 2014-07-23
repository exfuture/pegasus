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
 * pegasus — digital channel simulator, main file
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "pegasus_block.h"
#include "pegasus_channel.h"
#include "pegasus_error.h"
#include "pegasus_fec.h"
#include "pegasus_modulation.h"
#include "pegasus_panic.h"
#include "pegasus_random.h"
#include "pegasus_selftests.h"
#include "pegasus_source.h"
#include "pegasus_tools.h"
#include "pegasus_units.h"

int main(int argc, char** argv)
{
	unsigned int
		pgs_source = PGS_RANDOM,
		pgs_fec = PGF_NONE,
		pgs_modulation = PGM_ASK,
		pgs_channel = PGC_AWGN,
		pgs_error = PGE_BER,
		pgs_units = PGU_DBS,
		pgs_quiet = 0,
		pgs_selftest = 0;
	unsigned long long
		pgs_iterations = 200000,
		pgs_data_sequence_length = 0,
		pgs_data_source_blocks_count = 0,
		pgs_data_encoded_blocks_count = 0,
		pgs_data_premodulated_blocks_count = 0,
		pgs_data_modulated_signals_count = 0,
		pgs_data_noised_signals_count = 0,
		pgs_data_demodulated_blocks_count = 0,
		pgs_data_predecoded_blocks_count = 0,
		pgs_data_decoded_blocks_count = 0,
		pgs_data_target_blocks_count = 0;
	double
		pgs_hsquare_start = 0,
		pgs_hsquare_end = 15,
		pgs_hsquare_step = 0.5;

	pgs_block_t* pgs_data_source = NULL;
	pgs_block_t* pgs_data_source_blocks = NULL;
	pgs_block_t* pgs_data_encoded_blocks = NULL;
	pgs_block_t* pgs_data_premodulated_blocks = NULL;
	pgs_signal_t* pgs_data_modulated_signals = NULL;
	pgs_signal_t* pgs_data_noised_signals = NULL;
	pgs_block_t* pgs_data_demodulated_blocks = NULL;
	pgs_block_t* pgs_data_predecoded_blocks = NULL;
	pgs_block_t* pgs_data_decoded_blocks = NULL;
	pgs_block_t* pgs_data_target_blocks = NULL;

	int opts;

	struct option longopts[] =
	{
		{"source",		required_argument,	NULL, 's'},
		{"fec",			required_argument,	NULL, 'f'},
		{"modulation",		required_argument,	NULL, 'm'},
		{"channel",		required_argument,	NULL, 'c'},
		{"error",		required_argument,	NULL, 'e'},
		{"hsquare-start",	required_argument,	NULL, 'h'},
		{"hsquare-end",		required_argument,	NULL, 'n'},
		{"hsquare-step",	required_argument,	NULL, 't'},
		{"units",		required_argument,	NULL, 'u'},
		{"iterations",		required_argument,	NULL, 'i'},
		{"quiet",		no_argument,		NULL, 'q'},
		{"self-test",		no_argument,		NULL, 'l'},
		{0, 0, 0, 0}
	};

	while ((opts = getopt_long(argc, argv, "sfmcehntuiql", longopts, NULL)) != -1)
		switch (opts)
		{
			case 's':
				if (strcmp(optarg, "random") == 0)
					pgs_source = PGS_RANDOM;
				else if (strcmp(optarg, "predefined") == 0)
					pgs_source = PGS_PREDEFINED;
				else
					pgp_usage(opts, optarg);
				break;
			case 'f':
				if (strcmp(optarg, "none") == 0)
					pgs_fec = PGF_NONE;
				else if (strcmp(optarg, "hamming74") == 0)
					pgs_fec = PGF_HAMMING74;
				else if (strcmp(optarg, "cyclic85") == 0)
					pgs_fec = PGF_CYCLIC85;
				else if (strcmp(optarg, "bch1557") == 0)
					pgs_fec = PGF_BCH1557;
				else if (strcmp(optarg, "bch1575") == 0)
					pgs_fec = PGF_BCH1575;
				else
					pgp_usage(opts, optarg);
				break;

			case 'm':
				if (strcmp(optarg, "ask") == 0)
					pgs_modulation = PGM_ASK;
				else if (strcmp(optarg, "fsk") == 0)
					pgs_modulation = PGM_FSK;
				else if (strcmp(optarg, "bpsk") == 0)
					pgs_modulation = PGM_BPSK;
				else if (strcmp(optarg, "qpsk") == 0)
					pgs_modulation = PGM_QPSK;
				else if (strcmp(optarg, "8psk") == 0)
					pgs_modulation = PGM_8PSK;
				else if (strcmp(optarg, "16psk") == 0)
					pgs_modulation = PGM_16PSK;
				else if (strcmp(optarg, "32psk") == 0)
					pgs_modulation = PGM_32PSK;
				else if (strcmp(optarg, "64psk") == 0)
					pgs_modulation = PGM_64PSK;
				else if (strcmp(optarg, "256psk") == 0)
					pgs_modulation = PGM_256PSK;
				else if (strcmp(optarg, "1024psk") == 0)
					pgs_modulation = PGM_1024PSK;
				else if (strcmp(optarg, "4096psk") == 0)
					pgs_modulation = PGM_4096PSK;
				else if (strcmp(optarg, "16qam") == 0)
					pgs_modulation = PGM_16QAM;
				else if (strcmp(optarg, "32qam") == 0)
					pgs_modulation = PGM_32QAM;
				else if (strcmp(optarg, "64qam") == 0)
					pgs_modulation = PGM_64QAM;
				else if (strcmp(optarg, "256qam") == 0)
					pgs_modulation = PGM_256QAM;
				else if (strcmp(optarg, "1024qam") == 0)
					pgs_modulation = PGM_1024QAM;
				else if (strcmp(optarg, "4096qam") == 0)
					pgs_modulation = PGM_4096QAM;
				else
					pgp_usage(opts, optarg);
				break;
			case 'c':
				if (strcmp(optarg, "awgn") == 0)
					pgs_channel = PGC_AWGN;
				else if (strcmp(optarg, "rayleigh") == 0)
					pgs_channel = PGC_RAYLEIGH;
				else
					pgp_usage(opts, optarg);
				break;
			case 'e':
				if (strcmp(optarg, "ber") == 0)
					pgs_error = PGE_BER;
				else if (strcmp(optarg, "ser") == 0)
					pgs_error = PGE_SER;
				else
					pgp_usage(opts, optarg);
				break;
			case 'h':
				pgs_hsquare_start = atof(optarg);
				break;
			case 'n':
				pgs_hsquare_end = atof(optarg);
				break;
			case 't':
				pgs_hsquare_step = atof(optarg);
				break;
			case 'u':
				if (strcmp(optarg, "dbs") == 0)
					pgs_units = PGU_DBS;
				else if (strcmp(optarg, "times") == 0)
					pgs_units = PGU_TIMES;
				else
					pgp_usage(opts, optarg);
				break;
			case 'i':
				pgs_iterations = atoi(optarg);
				break;
			case 'q':
				pgs_quiet = 1;
				break;
			case 'l':
				pgs_selftest = 1;
				break;
			default:
				fprintf(stderr, "Unknown option: %c\n", opts);
				exit(EX_USAGE);
		}

	pgt_init_threads();
	pgr_init_seed();
	pgf_init_polynomes();
	pgf_init_syndromes();
	pgm_init_tables();

	if (pgs_selftest == 1)
	{
		pgst_run_tests();
		pgf_done_syndromes();
		pgf_done_polynomes();
		exit(EX_OK);
	}

	if (pgs_quiet == 0)
	{
		printf("Source: %s\n", pgs_source_to_string(pgs_source));
		printf("FEC: %s\n", pgf_to_string(pgs_fec));
		printf("Modulation: %s\n", pgm_to_string(pgs_modulation));
		printf("Channel: %s\n", pgc_channel_to_string(pgs_channel));
		printf("Error type: %s\n", pge_to_string(pgs_error));
		printf("h²=[%lf, %lf] %s, with %lf step\n",
				pgs_hsquare_start,
				pgs_hsquare_end,
				pgu_units_to_string(pgs_units),
				pgs_hsquare_step);
		printf("Iterations: %llu\n", pgs_iterations);
	}

	for (double hsquare = pgs_hsquare_start; hsquare <= pgs_hsquare_end; hsquare += pgs_hsquare_step)
	{
		pgs_data_sequence_length =
			pgs_generate(&pgs_data_source,
				pgs_source,
				pgs_modulation,
				pgs_iterations);
		if (unlikely(pgs_data_sequence_length == 0))
			pgp_nodata();

		pgs_data_source_blocks_count =
			pgt_blocks_to_blocks(&pgs_data_source_blocks,
					pgs_data_source,
					1,
					pgf_get_input_block_size(pgs_fec));
		if (unlikely(pgs_data_source_blocks_count == 0))
			pgp_nodata();

		pgs_data_encoded_blocks_count =
			pgf_encode_blocks(&pgs_data_encoded_blocks,
					pgs_data_source_blocks,
					pgs_data_source_blocks_count,
					pgs_fec);
		if (unlikely(pgs_data_encoded_blocks_count == 0))
			pgp_nodata();

		pgs_data_premodulated_blocks_count =
			pgt_blocks_to_blocks(&pgs_data_premodulated_blocks,
					pgs_data_encoded_blocks,
					pgs_data_encoded_blocks_count,
					pgm_get_block_size(pgs_modulation));
		if (unlikely(pgs_data_premodulated_blocks_count == 0))
			pgp_nodata();

		pgs_data_modulated_signals_count =
			pgm_modulate(&pgs_data_modulated_signals,
					pgs_data_premodulated_blocks,
					pgs_data_premodulated_blocks_count,
					pgs_modulation);
		if (unlikely(pgs_data_modulated_signals_count == 0))
			pgp_nodata();

		pgs_data_noised_signals_count =
			pgc_add_noise(&pgs_data_noised_signals,
					pgs_data_modulated_signals,
					pgs_data_modulated_signals_count,
					pgs_channel,
					pgs_units == PGU_TIMES ? hsquare : pgt_dbs_to_times(hsquare));
		if (unlikely(pgs_data_noised_signals_count == 0))
			pgp_nodata();

		pgs_data_demodulated_blocks_count =
			pgm_demodulate(&pgs_data_demodulated_blocks,
					pgs_data_noised_signals,
					pgs_data_noised_signals_count,
					pgs_modulation);
		if (unlikely(pgs_data_demodulated_blocks_count == 0))
			pgp_nodata();

		pgs_data_predecoded_blocks_count =
			pgt_blocks_to_blocks(&pgs_data_predecoded_blocks,
					pgs_data_demodulated_blocks,
					pgs_data_demodulated_blocks_count,
					pgs_fec == PGF_NONE ? pgs_data_sequence_length : pgf_get_output_block_size(pgs_fec));
		if (unlikely(pgs_data_predecoded_blocks_count == 0))
			pgp_nodata();

		pgs_data_decoded_blocks_count =
			pgf_decode_blocks(&pgs_data_decoded_blocks,
					pgs_data_predecoded_blocks,
					pgs_data_predecoded_blocks_count,
					pgs_fec);
		if (unlikely(pgs_data_decoded_blocks_count == 0))
			pgp_nodata();

		pgs_data_target_blocks_count =
			pgt_blocks_to_blocks(&pgs_data_target_blocks,
				pgs_data_decoded_blocks,
				pgs_data_decoded_blocks_count,
				pgs_data_sequence_length);
		if (unlikely(pgs_data_target_blocks_count == 0))
			pgp_nodata();

		if (pgs_error == PGE_BER)
			printf("%lf\t\t%1.16lf\n", hsquare, pgt_get_ber(pgs_data_source,
						pgs_data_target_blocks,
						pgs_data_sequence_length));
		else if (pgs_error == PGE_SER)
		{
			if (pgs_fec == PGF_NONE)
				printf("%lf\t\t%1.16lf\n", hsquare, pgt_get_ser(pgs_data_premodulated_blocks,
							pgs_data_demodulated_blocks,
							pgs_data_premodulated_blocks_count));
			else
				printf("%lf\t\t%1.16lf\n", hsquare, pgt_get_ser(pgs_data_source_blocks,
							pgs_data_decoded_blocks,
							pgs_data_source_blocks_count));
		}

#if defined (_OPENMP)
#pragma omp parallel sections
#endif
	{
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_target_blocks, pgs_data_target_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_decoded_blocks, pgs_data_decoded_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_predecoded_blocks, pgs_data_predecoded_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_demodulated_blocks, pgs_data_demodulated_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			free(pgs_data_noised_signals);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			free(pgs_data_modulated_signals);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_premodulated_blocks, pgs_data_premodulated_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_encoded_blocks, pgs_data_encoded_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_blocks(pgs_data_source_blocks, pgs_data_source_blocks_count);
		}
#if defined (_OPENMP)
#pragma omp section
#endif
		{
			pgb_destroy_block(pgs_data_source);
		}
	}
	}

	pgf_done_syndromes();
	pgf_done_polynomes();

	exit(EX_OK);
}

