#pragma once

#ifndef PEGASUS_SELFTESTS_H
#define PEGASUS_SELFTESTS_H

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
 * pegasus — digital channel simulator, selftests file header
 */

#define PGST_START_TEST()	all_tests_count++

#define PGST_RNG_ITERATIONS	1000000
#define PGST_SOURCE_LENGTH	30

void pgst_run_tests();

#endif /* PEGASUS_SELFTESTS_H */

