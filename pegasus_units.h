#pragma once

#ifndef PEGASUS_UNITS_H
#define PEGASUS_UNITS_H

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
 * pegasus — digital channel simulator, units file header
 */

#define PGU_DBS		1
#define PGU_TIMES	2

#define PGU_DBS_STRING		"decibels"
#define PGU_TIMES_STRING	"times"

char* pgu_units_to_string(unsigned int _units);

#endif /* PEGASUS_UNITS_H */

