#pragma once

#ifndef PEGASUS_ERROR_H
#define PEGASUS_ERROR_H

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
 * pegasus — digital channel simulator, error file header
 */

#define PGE_BER	1
#define PGE_SER	2

#define PGE_BER_STRING	"Bit error rate"
#define PGE_SER_STRING	"Symbol error rate"

char* pge_to_string(unsigned int _error);

#endif /* PEGASUS_ERROR_H */

