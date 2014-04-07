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
 * pegasus — digital channel simulator, channel file
 */

#include <math.h>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <stdlib.h>

#include "pegasus_common_strings.h"
#include "pegasus_common_types.h"

#include "pegasus_panic.h"
#include "pegasus_random.h"

#include "pegasus_channel.h"

char* pgc_channel_to_string(unsigned int _channel)
{
	switch (_channel)
	{
		case PGC_AWGN:
			return PGC_AWGN_STRING;
			break;
		case PGC_RAYLEIGH:
			return PGC_RAYLEIGH_STRING;
			break;
		default:
			pgp_switch_default();
			return PGS_CS_UNKNOWN;
			break;
	}
}

static void pgc_add_awgn_noise(pgs_signal_t* _noised_signal,
		pgs_signal_t* _clean_signal,
		double _hsquare)
{
	_noised_signal->i = _clean_signal->i + PGC_SIGMA2 * pgr_get_gauss();
	_noised_signal->q = _clean_signal->q + PGC_SIGMA2 * pgr_get_gauss();
}

static void pgc_add_rayleigh_noise(pgs_signal_t* _noised_signal,
		pgs_signal_t* _clean_signal,
		double _hsquare)
{
	double h_R, h_I, n_R, n_I, y_R, y_I, divisor, dividend_R, dividend_I;

	h_R = PGC_SIGMA1 * pgr_get_gauss();
	h_I = PGC_SIGMA1 * pgr_get_gauss();
	n_R = PGC_SIGMA2 * pgr_get_gauss();
	n_I = PGC_SIGMA2 * pgr_get_gauss();

	y_R = h_R * _clean_signal->i - h_I * _clean_signal->q + n_R;
	y_I = h_I * _clean_signal->i + h_R * _clean_signal->q + n_I;

	divisor = pow(h_R, 2) + pow(h_I, 2);
	dividend_R = y_R * h_R + y_I * h_I;
	dividend_I = y_I * h_R - y_R * h_I;

	_noised_signal->i = dividend_R / divisor;
	_noised_signal->q = dividend_I / divisor;
}

static void pgc_add_noise_to_signal(pgs_signal_t* _noised_signal,
		pgs_signal_t* _clean_signal,
		unsigned long long _channel,
		double _hsquare)
{
	if (unlikely(_noised_signal == NULL || _clean_signal == NULL))
		pgp_null();

	switch (_channel)
	{
		case PGC_AWGN:
			pgc_add_awgn_noise(_noised_signal, _clean_signal, _hsquare);
			break;
		case PGC_RAYLEIGH:
			pgc_add_rayleigh_noise(_noised_signal, _clean_signal, _hsquare);
			break;
		default:
			pgp_switch_default();
			break;
	}
}

unsigned long long pgc_add_noise(pgs_signal_t** _noised_signals,
		pgs_signal_t* _modulated_signals,
		unsigned long long _modulated_signals_count,
		unsigned int _channel,
		double _hsquare)
{
	if (unlikely(_modulated_signals == NULL))
		return 0;

	*_noised_signals = malloc(sizeof(pgs_signal_t) * _modulated_signals_count);
	if (unlikely(*_noised_signals == NULL))
		pgp_malloc();

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (unsigned long long i = 0; i < _modulated_signals_count; i++)
		pgc_add_noise_to_signal(&((pgs_signal_t*)(*_noised_signals))[i],
				&_modulated_signals[i],
				_channel,
				_hsquare);

	return _modulated_signals_count;

}

