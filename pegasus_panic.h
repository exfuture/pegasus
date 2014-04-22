#pragma once

#ifndef PEGASUS_PANIC_H
#define PEGASUS_PANIC_H

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
 * pegasus — digital channel simulator, panic file header
 */

#include <errno.h>

#define pgp_malloc() \
	__pgp_malloc(__func__, __FILE__, __LINE__, errno)
#define pgp_clock_gettime() \
	__pgp_clock_gettime(__func__, __FILE__, __LINE__, errno)
#define pgp_switch_default() \
	__pgp_switch_default(__func__, __FILE__, __LINE__)
#define pgp_null() \
	__pgp_null(__func__, __FILE__, __LINE__)
#define pgp_range() \
	__pgp_range(__func__, __FILE__, __LINE__)
#define pgp_nodata() \
__pgp_nodata(__func__, __FILE__, __LINE__)


#define PGP_BACKTRACE_SIZE 10

void pgp_usage(char _key, char* _keyvalue);
void __pgp_malloc(const char* _function_name, const char* _file_name, int _line_number, int _errno);
void __pgp_clock_gettime(const char* _function_name, const char* _file_name, int _line_number, int _errno);
void __pgp_switch_default(const char* _function_name, const char* _file_name, int _line_number);
void __pgp_null(const char* _function_name, const char* _file_name, int _line_number);
void __pgp_range(const char* _function_name, const char* _file_name, int _line_number);
void __pgp_nodata(const char* _function_name, const char* _file_name, int _line_number);

#endif /* PEGASUS_PANIC_H */

