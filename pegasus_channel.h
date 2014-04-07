#pragma once

#ifndef PEGASUS_CHANNEL_H
#define PEGASUS_CHANNEL_H

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
 * pegasus — digital channel simulator, channel file header
 */

#include "pegasus_common_types.h"

#define PGC_AWGN	1
#define PGC_RAYLEIGH	2

#define PGC_AWGN_STRING		"Additive white Gaussian noise"
#define PGC_RAYLEIGH_STRING	"Rayleigh"

#define PGC_SIGMA1	sqrt(1.0 / 2.0)
#define PGC_SIGMA2	sqrt(1.0 / (2.0 * _hsquare))

char* pgc_channel_to_string(unsigned int _channel);
unsigned long long pgc_add_noise(pgs_signal_t** _noised_signals,
		pgs_signal_t* _modulated_signals,
		unsigned long long _modulated_signals_count,
		unsigned int _channel,
		double _hsquare);

#endif /* PEGASUS_CHANNEL_H */

