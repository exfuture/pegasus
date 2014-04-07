#pragma once

#ifndef PEGASUS_MODULATION_H
#define PEGASUS_MODULATION_H

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
 * pegasus — digital channel simulator, modulation file header
 */

#define PGM_ASK		1
#define PGM_FSK		2
#define PGM_BPSK	3
#define PGM_QPSK	4
#define PGM_8PSK	5
#define PGM_16PSK	6
#define PGM_32PSK	7
#define PGM_64PSK	8
#define PGM_256PSK	9
#define PGM_1024PSK	10
#define PGM_16QAM	11
#define PGM_32QAM	12
#define PGM_64QAM	13
#define PGM_256QAM	14
#define PGM_1024QAM	15

#define PGM_ASK_STRING		"Amplitude-shift keying"
#define PGM_FSK_STRING		"Frequency-shift keying"
#define PGM_BPSK_STRING		"Binary phase-shift keying"
#define PGM_QPSK_STRING		"Quadrature phase-shift keying"
#define PGM_8PSK_STRING		"8-phase shift keying"
#define PGM_16PSK_STRING	"16-phase shift keying"
#define PGM_32PSK_STRING	"32-phase shift keying"
#define PGM_64PSK_STRING	"64-phase shift keying"
#define PGM_256PSK_STRING	"256-phase shift keying"
#define PGM_1024PSK_STRING	"1024-phase shift keying"
#define PGM_16QAM_STRING	"16-quadrature-amplitude modulation"
#define PGM_32QAM_STRING	"32-quadrature-amplitude modulation"
#define PGM_64QAM_STRING	"64-quadrature-amplitude modulation"
#define PGM_256QAM_STRING	"256-quadrature-amplitude modulation"
#define PGM_1024QAM_STRING	"1024-quadrature-amplitude modulation"

#define PGM_UNKNOWN_BLOCK_SIZE	0
#define PGM_ASK_BLOCK_SIZE	1
#define PGM_FSK_BLOCK_SIZE	1
#define PGM_BPSK_BLOCK_SIZE	1
#define PGM_QPSK_BLOCK_SIZE	2
#define PGM_8PSK_BLOCK_SIZE	3
#define PGM_16PSK_BLOCK_SIZE	4
#define PGM_32PSK_BLOCK_SIZE	5
#define PGM_64PSK_BLOCK_SIZE	6
#define PGM_256PSK_BLOCK_SIZE	8
#define PGM_1024PSK_BLOCK_SIZE	10
#define PGM_16QAM_BLOCK_SIZE	4
#define PGM_32QAM_BLOCK_SIZE	5
#define PGM_64QAM_BLOCK_SIZE	6
#define PGM_256QAM_BLOCK_SIZE	8
#define PGM_1024QAM_BLOCK_SIZE	10

#define PGM_UNKNOWN_VOLUME	0
#define PGM_ASK_VOLUME		(1ULL << PGM_ASK_BLOCK_SIZE)
#define PGM_FSK_VOLUME		(1ULL << PGM_FSK_BLOCK_SIZE)
#define PGM_BPSK_VOLUME		(1ULL << PGM_BPSK_BLOCK_SIZE)
#define PGM_QPSK_VOLUME		(1ULL << PGM_QPSK_BLOCK_SIZE)
#define PGM_8PSK_VOLUME		(1ULL << PGM_8PSK_BLOCK_SIZE)
#define PGM_16PSK_VOLUME	(1ULL << PGM_16PSK_BLOCK_SIZE)
#define PGM_32PSK_VOLUME	(1ULL << PGM_32PSK_BLOCK_SIZE)
#define PGM_64PSK_VOLUME	(1ULL << PGM_64PSK_BLOCK_SIZE)
#define PGM_256PSK_VOLUME	(1ULL << PGM_256PSK_BLOCK_SIZE)
#define PGM_1024PSK_VOLUME	(1ULL << PGM_1024PSK_BLOCK_SIZE)
#define PGM_16QAM_VOLUME	(1ULL << PGM_16QAM_BLOCK_SIZE)
#define PGM_32QAM_VOLUME	(1ULL << PGM_32QAM_BLOCK_SIZE)
#define PGM_64QAM_VOLUME	(1ULL << PGM_64QAM_BLOCK_SIZE)
#define PGM_256QAM_VOLUME	(1ULL << PGM_256QAM_BLOCK_SIZE)
#define PGM_1024QAM_VOLUME	(1ULL << PGM_1024QAM_BLOCK_SIZE)

#define PGM_ASK_ANGLE_STEP	0
#define PGM_FSK_ANGLE_STEP	(M_PI / 4.0)
#define PGM_BPSK_ANGLE_STEP	(M_PI / 2.0)
#define PGM_QPSK_ANGLE_STEP	(M_PI / 4.0)
#define PGM_8PSK_ANGLE_STEP	(M_PI / 8.0)
#define PGM_16PSK_ANGLE_STEP	(M_PI / 16.0)
#define PGM_32PSK_ANGLE_STEP	(M_PI / 32.0)
#define PGM_64PSK_ANGLE_STEP	(M_PI / 64.0)
#define PGM_256PSK_ANGLE_STEP	(M_PI / 256.0)
#define PGM_1024PSK_ANGLE_STEP	(M_PI / 1024.0)

#define PGM_ASK_PHASE_SHIFT	0
#define PGM_FSK_PHASE_SHIFT	0
#define PGM_BPSK_PHASE_SHIFT	0
#define PGM_QPSK_PHASE_SHIFT	(M_PI / 4.0)
#define PGM_8PSK_PHASE_SHIFT	0
#define PGM_16PSK_PHASE_SHIFT	0
#define PGM_32PSK_PHASE_SHIFT	0
#define PGM_64PSK_PHASE_SHIFT	0
#define PGM_256PSK_PHASE_SHIFT	0
#define PGM_1024PSK_PHASE_SHIFT	0

#define PGM_16QAM_BOUND		3
#define PGM_32QAM_BOUND		5
#define PGM_64QAM_BOUND		7
#define PGM_256QAM_BOUND	15
#define PGM_1024QAM_BOUND	31

#define PGM_16QAM_NORMALIZATION		(1.0 / (PGM_16QAM_BOUND * sqrt(2.0)))
#define PGM_32QAM_NORMALIZATION		(1.0 / (PGM_32QAM_BOUND * sqrt(2.0)))
#define PGM_64QAM_NORMALIZATION		(1.0 / (PGM_64QAM_BOUND * sqrt(2.0)))
#define PGM_256QAM_NORMALIZATION	(1.0 / (PGM_256QAM_BOUND * sqrt(2.0)))
#define PGM_1024QAM_NORMALIZATION	(1.0 / (PGM_1024QAM_BOUND * sqrt(2.0)))

#define PGM_ASK_MINIMUM		0.5
#define PGM_FSK_MINIMUM		sin(PGM_FSK_ANGLE_STEP)
#define PGM_BPSK_MINIMUM	sin(PGM_BPSK_ANGLE_STEP)
#define PGM_QPSK_MINIMUM	sin(PGM_QPSK_ANGLE_STEP)
#define PGM_8PSK_MINIMUM	sin(PGM_8PSK_ANGLE_STEP)
#define PGM_16PSK_MINIMUM	sin(PGM_16PSK_ANGLE_STEP)
#define PGM_32PSK_MINIMUM	sin(PGM_32PSK_ANGLE_STEP)
#define PGM_64PSK_MINIMUM	sin(PGM_64PSK_ANGLE_STEP)
#define PGM_256PSK_MINIMUM	sin(PGM_256PSK_ANGLE_STEP)
#define PGM_1024PSK_MINIMUM	sin(PGM_1024PSK_ANGLE_STEP)
#define PGM_16QAM_MINIMUM	PGM_16QAM_NORMALIZATION
#define PGM_32QAM_MINIMUM	PGM_32QAM_NORMALIZATION
#define PGM_64QAM_MINIMUM	PGM_64QAM_NORMALIZATION
#define PGM_256QAM_MINIMUM	PGM_256QAM_NORMALIZATION
#define PGM_1024QAM_MINIMUM	PGM_1024QAM_NORMALIZATION

pgs_signal_t pgm_ask_table[PGM_ASK_VOLUME];
pgs_signal_t pgm_fsk_table[PGM_FSK_VOLUME];
pgs_signal_t pgm_bpsk_table[PGM_BPSK_VOLUME];
pgs_signal_t pgm_qpsk_table[PGM_QPSK_VOLUME];
pgs_signal_t pgm_8psk_table[PGM_8PSK_VOLUME];
pgs_signal_t pgm_16psk_table[PGM_16PSK_VOLUME];
pgs_signal_t pgm_32psk_table[PGM_32PSK_VOLUME];
pgs_signal_t pgm_64psk_table[PGM_64PSK_VOLUME];
pgs_signal_t pgm_256psk_table[PGM_256PSK_VOLUME];
pgs_signal_t pgm_1024psk_table[PGM_1024PSK_VOLUME];
pgs_signal_t pgm_16qam_table[PGM_16QAM_VOLUME];
pgs_signal_t pgm_32qam_table[PGM_32QAM_VOLUME];
pgs_signal_t pgm_64qam_table[PGM_64QAM_VOLUME];
pgs_signal_t pgm_256qam_table[PGM_256QAM_VOLUME];
pgs_signal_t pgm_1024qam_table[PGM_1024QAM_VOLUME];

void pgm_init_tables();
char* pgs_modulation_to_string(unsigned int _modulation);
unsigned long long pgm_get_block_size(unsigned int _modulation);
unsigned long long pgm_get_volume(unsigned int _modulation);
unsigned long long pgm_modulate(pgs_signal_t** _modulated_signals,
		pgs_block_t* _premodulated_blocks,
		unsigned long long _premodulated_blocks_count,
		unsigned int _modulation);
unsigned long long pgm_demodulate(pgs_block_t** _demodulated_blocks,
		pgs_signal_t* _modulated_signals,
		unsigned long long _modulated_signals_count,
		unsigned int _modulation);

#endif /* PEGASUS_MODULATION_H */

